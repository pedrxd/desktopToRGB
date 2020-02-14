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

#ifndef COLORSHOT_H
#define COLORSHOT_H

#include <QtDebug>
#include <QColor>
#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QGuiApplication>
#include <array>
#include <cmath>

class ColorShot : public QObject
{
    Q_OBJECT
public:
    explicit ColorShot(QObject *parent = nullptr);
    void getColorScreen(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &brightness);


    void setJumpPixel(int npixel) {_jumpEveryPixels = npixel;}
    int getJumpPixel() {return _jumpEveryPixels;}

    void setMode(bool inside) {_inside = inside;}
    bool getMode() {return _inside;}

    void setMaxX(int i) {_maxx = i;}
    void setMinX(int i) {_minx = i;}
    void setMaxY(int i) {_maxy = i;}
    void setMinY(int i) {_miny = i;}
    void setMaxBrightness(unsigned char i) {_maxBrightness = i;}
    void setMinBrightness(unsigned char i) {_minBrightness = i;}

    int getMaxX() {return _maxx;}
    int getMaxY() {return _maxy;}
    int getMinX() {return _minx;}
    int getMinY() {return _miny;}
    int getMaxBrightness() {return _maxBrightness;}
    int getMinBrightness() {return _minBrightness;}

signals:

private:
    int aproximeColor(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &brightness);
    int _maxx;
    int _minx;
    int _maxy;
    int _miny;
    bool _inside;
    int _jumpEveryPixels;
    unsigned char _maxBrightness;
    unsigned char _minBrightness;
};

#endif // COLORSHOT_H
