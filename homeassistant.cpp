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

#include "homeassistant.h"

HomeAssistant::HomeAssistant(QObject *parent) : QObject(parent)
{
    _conectionCorrect = false;
}

bool HomeAssistant::setUrl(QString url)
{
    _url.setUrl(url);
    return _url.isValid();
}
void HomeAssistant::setToken(QString token)
{
    _token = token;
}
void HomeAssistant::setPort(int puerto)
{
    _url.setPort(puerto);
}
void HomeAssistant::setQNetworkManager(QNetworkAccessManager *manager)
{
    _manager = manager;
}

void HomeAssistant::executeService(QString domain, QString service, QJsonObject data)
{
    QString fullpath = "/api/services/" + domain + "/" + service;
    createPostRequest(fullpath, data);
}


void HomeAssistant::checkConnectivity()
{
    QNetworkRequest request;
    QUrl url(_url);

    url.setPath("/api/");
    request.setUrl(url);

    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer ").append(_token));

    QNetworkReply *reply = _manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        bool status = false;
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray response = reply->readAll();
            QJsonParseError jsonError;
            QJsonDocument loadDoc = QJsonDocument::fromJson(response, &jsonError); // parse and capture the error flag

             if(jsonError.error != QJsonParseError::NoError){
             {
                qDebug() << "Error: " << jsonError.errorString();
                _conectionCorrect = false;
             }
               //return;
            }else
             {

                 QJsonValue value = loadDoc.object().value("message");
                 if(value.toString() == "API running.")
                 {
                     status = true;
                     _conectionCorrect = true;
                 }
             }
        }else
        {
            _conectionCorrect = false;
            qDebug("Error: " + reply->errorString().toLatin1());
        }
       emit connectivityStatus(status);
    });
}

void HomeAssistant::createPostRequest(QString path, QJsonObject data)
{
    QNetworkRequest request;
    QJsonDocument json (data);

    QUrl url = QUrl(_url);
    url.setPath(path);

    request.setUrl(url);
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer ").append(_token));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json");

    _manager->post(request, json.toJson(QJsonDocument::JsonFormat::Compact));
}
