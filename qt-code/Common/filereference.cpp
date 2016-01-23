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

#include "filereference.h"
#include <QDebug>

FileReference::FileReference(QString filePath, QString fileHash): m_FilePath(filePath), m_FileHash(fileHash)
{
}
//TODO: Include file name in hash
FileReference::FileReference(QJsonObject json)
{
    m_FilePath = json[FileReference::JSON_KEY_PATH].toString();
    m_FileHash = json[FileReference::JSON_KEY_HASH].toString();
}

QString FileReference::getFilePath() const
{
    return m_FilePath;
}

QString FileReference::getFileHash() const
{
    return m_FileHash;
}


QJsonObject FileReference::toJson() const
{
    QJsonObject json;
    json.insert(FileReference::JSON_KEY_PATH,m_FilePath);
    json.insert(FileReference::JSON_KEY_HASH,m_FileHash);
    return json;
}

void FileReference::changePathPrefix(const QString& oldPrefix, const QString& newPrefix)
{
    if(m_FilePath.startsWith(oldPrefix) && oldPrefix.length() > 0)
    {
        m_FilePath.replace(0,oldPrefix.length(),newPrefix);
    }
    else
    {
        qWarning() << "Changing Path Prefix: Given old prefix does not match the actual prefix or is empty";
        qDebug() << "Prefix: " << oldPrefix;
        qDebug() << "Full: " << m_FilePath;
    }

}

bool FileReference::operator==(const FileReference &other)
{
    return m_FileHash == other.m_FileHash;
}

bool FileReference::operator!=(const FileReference &other)
{
    return ! (*this == other);
}

bool operator<(FileReference & ref1, FileReference & ref2)
{
    return FileReference::lessThanByHash(ref1,ref2);
}

bool FileReference::lessThanByHash(const FileReference &e1, const FileReference &e2) {
    return e1.m_FileHash < e2.m_FileHash;
}


