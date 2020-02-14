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

#ifndef HOMEASSISTANT_H
#define HOMEASSISTANT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class HomeAssistant : public QObject
{
    Q_OBJECT
public:
    explicit HomeAssistant(QObject *parent = nullptr);
    bool setUrl(QString url);
    void setToken(QString token);
    void setPort(int puerto);
    void setQNetworkManager(QNetworkAccessManager *manager);

    void executeService(QString domain, QString service, QJsonObject data);
    bool isCorrect() {return _conectionCorrect;}
public slots:
    void checkConnectivity();

signals:
    void connectivityStatus(bool online);

private:
    QUrl _url;
    QString _token;
    QNetworkAccessManager *_manager;
    bool _conectionCorrect;
    QJsonObject createGetRequest(QString path);
    void createPostRequest(QString path, QJsonObject data);

};

#endif // HOMEASSISTANT_H
