/*
 *  This file is part of desktopToRGB.
 *
 *  desktopToRGB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  desktopToRGB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with desktopToRGB.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "colorshot.h"

#define N_COLORS 13
std::array<std::array<unsigned char, 3>, N_COLORS> colors =
        {{
                {255,255,255},
                {255, 51, 51},
                {255,153,51},
                {255,255,51},
                {153,255,51},
                {51,255,51},
                {51,255,153},
                {51,255,255},
                {51,153,255},
                {51,51,255},
                {153,51,255},
                {255,51,255},
                {255,51,153}
        }};

ColorShot::ColorShot(QObject *parent) : QObject(parent)
{
    setJumpPixel(16);
    setMinX(0);
    setMinY(0);
    setMaxX(QGuiApplication::primaryScreen()->geometry().width());
    setMaxY(QGuiApplication::primaryScreen()->geometry().height());
    setMaxBrightness(200);
    setMinBrightness(100);
    setMode(true);
}

int ColorShot::aproximeColor(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &brightness )
{
    //Aproximate allways color to 255
    int maxBrightness;
    red < green ? maxBrightness = green : maxBrightness = red;
    if(maxBrightness < blue) maxBrightness = blue;

    red = ((double)red/maxBrightness)*(255);
    green = ((double)green/maxBrightness)*(255);
    blue = ((double)blue/maxBrightness)*(255);
    brightness = ((double)maxBrightness/255.0)*((double)getMaxBrightness()-getMinBrightness())+getMinBrightness();

    int minHeuristic = pow(abs(colors[0][0] - red),2) + pow(abs(colors[0][1] - green),2) + pow(abs(colors[0][2] - blue),2);;
    int minIndex = 0;

    //Give a aproximation to colors
    for (int i = 1; i < N_COLORS; ++i) {
        int heuristic = pow(abs(colors[i][0] - red),2) + pow(abs(colors[i][1] - green),2) + pow(abs(colors[i][2] - blue),2);
        if(heuristic < minHeuristic)
        {
            minIndex = i;
            minHeuristic = heuristic;
        }
    }
    return minIndex;
}

void ColorShot::getColorScreen(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &brightness)
{
    std::array<long,N_COLORS> frequency{0};
    long brightnessMediam = 0;
    int nPixel = 0;

    //We take the screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QImage image;
    image = screen->grabWindow(0).toImage();

    //ToDo: Apply only on the selected area
    if(_inside)
    {
    for(int x = _minx; x < _maxx; x+=getJumpPixel())
        for(int y = _miny; y < _maxy; y+=getJumpPixel())
        {
            QColor color = QColor(image.pixel(x,y));
            unsigned char red = color.red();
            unsigned char green = color.green();
            unsigned char blue = color.blue();

            //Obtain most aproximate color
            int bestColor = aproximeColor(red,green,blue, brightness);
            brightnessMediam+=brightness;
            //Add frequency for that color
            frequency[bestColor]++;
            nPixel++;
        }
    }else
    {
        int width = screen->geometry().width();
        int height = screen->geometry().height();
        for(int x = 0; x < width; x+=getJumpPixel())
            for(int y = 0; y < height; y+=getJumpPixel())
            {
                if(y > _miny && y < _maxy && x > _minx && x < _maxx)
                {
                    y = _maxy;
                }
                QColor color = QColor(image.pixel(x,y));
                unsigned char red = color.red();
                unsigned char green = color.green();
                unsigned char blue = color.blue();

                //Obtain most aproximate color
                int bestColor = aproximeColor(red,green,blue, brightness);
                brightnessMediam+=brightness;
                //Add frequency for that color
                frequency[bestColor]++;
                nPixel++;
            }
    }
    //Now select the most frequency color and return it
    int indexColor = 0;
    int maxFrequency = frequency[0];

    for(int i = 1; i < N_COLORS; i++)
    {
        if(frequency[i] > maxFrequency)
        {
            indexColor = i;
            maxFrequency = frequency[i];
        }
    }

    brightness = brightnessMediam/nPixel;
    red = colors[indexColor][0];
    green = colors[indexColor][1];
    blue = colors[indexColor][2];

    qDebug() << "ColorScreen: " << "[" << red << "," << green << "," << blue << "]";
}
