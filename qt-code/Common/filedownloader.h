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

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QJsonObject>
#include <QVector>

/**
* Download files from an url and retrieve them as strings, JSON-objects, byte arrays
* or writes them directly to a file.
* Works asynchronously.
*
*/
class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    virtual ~FileDownloader();

    /**
     * Download a file from the given url.
     * Emits the signal downloaded() once its
     * finished.
     */
    void download(const QUrl& url);
    /**
     * Retrieve the file that was downloaded
     * last as byte array.
     */
    QByteArray getDownloadAsData() const;
    /**
     * Retrieve the file that was downloaded
     * last as string.
     */
    QString getDownloadAsString() const;
    /**
     * Retrieve the file that was downloaded
     * last as JSON object.
     */
    QJsonObject getDownloadAsJson() const;
    /**
     * Writes the downloaded file to a file
     * at the given path.
     */
    void writeDownloadToFile(QString path) const;
    /**
     * Delete the downloaded data from memory.
     */
    void clearDownload();

signals:
    /**
     * Emited when the download is finished.
     */
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData; //TODO: Problem on stack for large data?

};

#endif // FILEDOWNLOADER_H
