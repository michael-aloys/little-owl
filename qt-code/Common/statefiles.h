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

#ifndef STATEFILES_H
#define STATEFILES_H

#include <QJsonObject>
#include <QVector>
#include <utility>
#include "filereference.h"

/**
 * Representation of the synchronized directory. Stores a list
 * of FileReference objects and can be used to compare to
 * other StateFiles objects. Can also be serialized to JSON.
 *
 * The StateFiles object has a basePath. This is the path
 * of the directory.
 */
class StateFiles
{
public:
    StateFiles() {}
    /**
     * @brief StateFiles
     * @param basePath
     * @param fillWithDirectory true if the object should be filled
     * with all the files that are in basePath.
     */
    StateFiles(const QString basePath, bool fillWithDirectory);
    StateFiles(const QJsonObject json);
    /**
     * Add another file
     */
    void addFileReference(FileReference fileReference);
    /**
     * Fill this object with all the files that are in m_BasePath.
     */
    void fillWithDirectory();
    QVector<FileReference> getFileReferences();
    QString getBasePath() const;
    QJsonObject toJson() const;
    void changeBasePath(const QString &newBase);
    void sortByHash();
    /**
     * Compares to StateFiles objects. Returns the differnces
     * as pair <Files only found in this StateFiles, Files only
     * found in the other StateFiles>
     */
    std::pair<StateFiles, StateFiles> difference(StateFiles &other);
    /**
     * @return number of files
     */
    int getSize() const;
    bool isEmpty() const;

    static constexpr const char* JSON_KEY_BASE_PATH = "basepath";
    static constexpr const char* JSON_KEY_FILES = "files";

private:
    QVector<FileReference> m_FileReferences;
    QString m_BasePath;
};

#endif // STATEFILES_H
