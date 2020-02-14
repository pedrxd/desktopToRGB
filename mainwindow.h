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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>
#include "homeassistant.h"
#include "colorshot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_checkButton_clicked();
    void on_checkStatus_response(bool status);
    void on_updateColor();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_minBrightnessBox_valueChanged(int arg1);

    void on_maxBrightnessBox_valueChanged(int arg1);

    void on_updatesBox_valueChanged(int arg1);

    void on_maxxBox_valueChanged(int arg1);

    void on_minxBox_valueChanged(int arg1);

    void on_maxyBox_valueChanged(int arg1);

    void on_minyBox_valueChanged(int arg1);

    void on_insideRadio_clicked();

    void on_outsideRadio_clicked();

signals:
    void checkConnectivity();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
    HomeAssistant ha;
    ColorShot colorshot;
    QTimer *timer;

};
#endif // MAINWINDOW_H
