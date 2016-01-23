/** Copyright (c) 2016 Michael A. Hedderich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "filedownloader.h"
#include <QJsonDocument>
#include "filewriter.h"

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));
}

FileDownloader::~FileDownloader() { }

void FileDownloader::download(const QUrl& url) {
    qDebug() << "Downloading from " << url;
    QNetworkRequest request(url);
    QNetworkReply* reply = m_WebCtrl.get(request);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
}

void FileDownloader::slotError(QNetworkReply::NetworkError) {
    //TODO: Proper error managmenet
    qDebug() << "An error";
}

void FileDownloader::slotSslErrors(QList<QSslError>) {
    //TODO: Proper error managmenet
    qDebug() << "SSL error";
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    int statusCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != 200) {
        //TODO: react on other status codes
    }
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::getDownloadAsData() const {
    return m_DownloadedData;
}

QString FileDownloader::getDownloadAsString() const {
    return QString(m_DownloadedData);
}

QJsonObject FileDownloader::getDownloadAsJson() const {
    return QJsonDocument::fromJson(m_DownloadedData).object();
}

void FileDownloader::writeDownloadToFile(QString path) const {
    FileWriter::writeFile(path,m_DownloadedData);
}

void FileDownloader::clearDownload()
{
    m_DownloadedData.clear();
}
