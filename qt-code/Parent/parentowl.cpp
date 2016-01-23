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

#include "parentowl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "filewriter.h"
#include "statefiles.h"

ParentOwl::ParentOwl(QString basePath, QString remotePath, QString indexPath, QObject *parent) :
    QObject(parent), m_BasePath(basePath), m_RemotePath(remotePath), m_IndexPath(indexPath)
{
    //TODO: Make sure to remove unnecessary / at the end of the paths
}

void ParentOwl::update()
{
    rebuildStateFiles();
    m_CurrentState.changeBasePath(m_RemotePath);
    QJsonDocument json(m_CurrentState.toJson());
    FileWriter::writeFile(m_IndexPath,json.toJson());
}

void ParentOwl::rebuildStateFiles()
{
    m_CurrentState = StateFiles(m_BasePath, true);
}


