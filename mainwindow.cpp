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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_updateColor()));
    connect(&ha, SIGNAL(connectivityStatus(bool)), this, SLOT(on_checkStatus_response(bool)));
    connect(this, SIGNAL(checkConnectivity()), &ha, SLOT(checkConnectivity()));
    connect(this, SIGNAL(checkConnectivity()), &ha, SLOT(checkConnectivity()));


    ui->maxxBox->setRange(1,QGuiApplication::primaryScreen()->geometry().width());
    ui->minxBox->setRange(0,QGuiApplication::primaryScreen()->geometry().width()-1);

    ui->maxyBox->setRange(1,QGuiApplication::primaryScreen()->geometry().height());
    ui->minyBox->setRange(0,QGuiApplication::primaryScreen()->geometry().width()-1);

    //Set limits
    ui->minxBox->setValue(0);
    ui->minyBox->setValue(0);
    ui->maxxBox->setValue(QGuiApplication::primaryScreen()->geometry().width());
    ui->maxyBox->setValue(QGuiApplication::primaryScreen()->geometry().height());


}

void MainWindow::on_updateColor()
{
    unsigned char red, green, blue, brightness;
    //Obtenemos color
    colorshot.getColorScreen(red, green, blue, brightness);

    //Creamos el json
    QJsonObject object;
    object.insert("entity_id", ui->entityIdBox->text());
    object.insert("transition", ui->transitionBox->value());
    object.insert("brightness", (int)brightness);

    QJsonArray jarray;
    jarray.append(QJsonValue((int)red));
    jarray.append(QJsonValue((int)green));
    jarray.append(QJsonValue((int)blue));

    object.insert("rgb_color", jarray);

    //Enviamos color
    ha.executeService("light","turn_on", object);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkStatus_response(bool status)
{
    if(status)
    {
        ui->checkLabel->setText("Connection correct!");
        ui->checkButton->setIcon(QIcon(":/icons/checked.svg"));
    }else
    {
        ui->checkLabel->setText("Can't connect to the server");
        ui->checkButton->setIcon(QIcon(":/icons/cancel.svg"));
    }
}

void MainWindow::on_checkButton_clicked()
{
    if(ui->urlInput->text().isEmpty())
    {
        ui->checkLabel->setText("Url can not be empty");
        ui->checkButton->setIcon(QIcon(":/icons/cancel.svg"));
    }else if(ui->tokenInput->text().isEmpty())
    {
        ui->checkLabel->setText("Token can not be empty");
        ui->checkButton->setIcon(QIcon(":/icons/cancel.svg"));
    }else
    {
        if(ha.setUrl(ui->urlInput->text()))
        {
            ha.setToken(ui->tokenInput->text());
            ha.setQNetworkManager(&manager);
            emit checkConnectivity();
        }else
        {
            ui->checkLabel->setText("URL incorrect format");
            ui->checkButton->setIcon(QIcon(":/icons/cancel.svg"));
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    if(!timer->isActive() )
    {
        if(ha.isCorrect())
        {
            timer->start(ui->updatesBox->value());
            ui->startButton->setDisabled(true);
            ui->stopButton->setDisabled(false);
        }else
        {
            on_checkButton_clicked();
        }
    }
}

void MainWindow::on_stopButton_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->startButton->setDisabled(false);
        ui->stopButton->setDisabled(true);
    }
}

void MainWindow::on_minBrightnessBox_valueChanged(int arg1)
{
    ui->maxBrightnessBox->setMinimum(arg1+1);
    colorshot.setMinBrightness(arg1);
}

void MainWindow::on_maxBrightnessBox_valueChanged(int arg1)
{
    ui->minBrightnessBox->setMaximum(arg1-1);
    colorshot.setMaxBrightness(arg1);
}

void MainWindow::on_updatesBox_valueChanged(int arg1)
{
    if(timer->isActive())
    {
        timer->stop();
        timer->start(arg1);
    }
}

void MainWindow::on_maxxBox_valueChanged(int arg1)
{
    ui->minxBox->setMaximum(arg1-1);
    colorshot.setMaxX(arg1);
}

void MainWindow::on_minxBox_valueChanged(int arg1)
{
    ui->maxxBox->setMinimum(arg1+1);
    colorshot.setMinX(arg1);
}

void MainWindow::on_maxyBox_valueChanged(int arg1)
{
    ui->minyBox->setMaximum(arg1-1);
    colorshot.setMaxY(arg1);
}

void MainWindow::on_minyBox_valueChanged(int arg1)
{
    ui->maxyBox->setMinimum(arg1+1);
    colorshot.setMinY(arg1);
}

void MainWindow::on_insideRadio_clicked()
{
    colorshot.setMode(true);
}

void MainWindow::on_outsideRadio_clicked()
{
    colorshot.setMode(false);
}
