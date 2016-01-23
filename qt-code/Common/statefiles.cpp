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

#include "statefiles.h"
#include <QJsonArray>
#include <QDirIterator>
#include <QDebug>
#include <QtAlgorithms>
#include "hasher.h"

StateFiles::StateFiles(QString basePath, bool fill)
{
    m_BasePath = basePath;
    if(fill) {
        fillWithDirectory();
    }
}

StateFiles::StateFiles(const QJsonObject json)
{
    m_BasePath = json[StateFiles::JSON_KEY_BASE_PATH].toString();
    QJsonArray j_FileReferences = json[StateFiles::JSON_KEY_FILES].toArray();
    for(auto iter = j_FileReferences.begin(); iter != j_FileReferences.end(); ++iter)
    {
        addFileReference(FileReference(iter->toObject()));
    }
}

void StateFiles::addFileReference(FileReference fileReference)
{
    m_FileReferences.push_back(fileReference);
}

void StateFiles::fillWithDirectory()
{
    m_FileReferences.clear();
    Hasher hasher;
    QDirIterator dirIterator(m_BasePath, QDir::Files, QDirIterator::Subdirectories);
    while (dirIterator.hasNext()) {
        QByteArray hashedFile = hasher.hash(dirIterator.next());
        FileReference fileRef(dirIterator.filePath(),hashedFile);
        addFileReference(fileRef);
    }
}

QVector<FileReference> StateFiles::getFileReferences()
{
    return m_FileReferences;
}

QString StateFiles::getBasePath() const
{
    return m_BasePath;
}

QJsonObject StateFiles::toJson() const
{    
    QJsonArray jArray;
    for(FileReference fileRef: m_FileReferences)
    {
        jArray.push_back(fileRef.toJson());
    }

    QJsonObject json;
    json.insert(StateFiles::JSON_KEY_FILES,jArray);
    json.insert(StateFiles::JSON_KEY_BASE_PATH,m_BasePath);

    return json;
}

void StateFiles::changeBasePath(const QString& newBase)
{
    for(auto iter = m_FileReferences.begin(); iter != m_FileReferences.end(); ++iter)
    {
        iter->changePathPrefix(m_BasePath,newBase);
    }
    m_BasePath = newBase;
}

std::pair<StateFiles,StateFiles> StateFiles::difference(StateFiles& other)
{
    //TODO: Also compare file name without base
    StateFiles onlyThis(m_BasePath, false);
    StateFiles onlyOther(other.m_BasePath, false);

    //sorting once, so that comparison is one go through lists
    sortByHash();
    other.sortByHash();

    auto thisIter = m_FileReferences.begin();
    auto otherIter = other.m_FileReferences.begin();

    //while elements in both lists
    while(thisIter != m_FileReferences.end() && otherIter != other.m_FileReferences.end())
    {
        if(thisIter->getFileHash() < otherIter->getFileHash())
        {
            onlyThis.addFileReference(*thisIter);
            ++thisIter;
        }
        else if (otherIter->getFileHash() < thisIter->getFileHash())
        {
            onlyOther.addFileReference(*otherIter);
            ++otherIter;
        }
        else //equal
        {
            ++thisIter;
            ++otherIter;
        }
    }
    //other list is empty
    while(thisIter != m_FileReferences.end())
    {
        onlyThis.addFileReference(*thisIter);
        ++thisIter;
    }
    //this list is empty
    while(otherIter != other.m_FileReferences.end())
    {
        onlyOther.addFileReference(*otherIter);
        ++otherIter;
    }
    return std::pair<StateFiles,StateFiles>(onlyThis,onlyOther);
}

void StateFiles::sortByHash()
{
    qSort(m_FileReferences.begin(), m_FileReferences.end(), FileReference::lessThanByHash);
}

bool StateFiles::isEmpty() const
{
    return m_FileReferences.size() == 0;
}

int StateFiles::getSize() const
{
    return m_FileReferences.size();
}
