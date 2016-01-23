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

#ifndef VERSIONEDFILEREFERENCE_H
#define VERSIONEDFILEREFERENCE_H

#include <QJsonObject>

/**
 * Representation of a file. Stores for each file its path
 * and a hash of the file. Can be used to compare files.
 */
class FileReference
{
public:
    FileReference() {}
    FileReference(QString filePath, QString fileHash);
    FileReference(QJsonObject json);
    QString getFilePath() const;
    QString getFileHash() const;
    QJsonObject toJson() const;

    /**
     * Changes the front part of the path of the file by replacing oldPrefix
     * with nexPrefix.
     */
    void changePathPrefix(const QString& oldPrefix, const QString& newPrefix);

    bool operator==(const FileReference &other);
    bool operator!=(const FileReference &other);
    static bool lessThanByHash(const FileReference &e1, const FileReference &e2);

    static constexpr const char* JSON_KEY_PATH = "path";
    static constexpr const char* JSON_KEY_HASH = "hash";

private:
    QString m_FilePath;
    QString m_FileHash;

};

bool operator<(FileReference & ref1, FileReference & ref2);

#endif // VERSIONEDFILEREFERENCE_H
