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

#include "childowl.h"
#include "filedownloader.h"
#include <QProcess>

//TODO: Create directories

ChildOwl::ChildOwl(QString basePath, QString parentIndexUrl, QString nextProgramPath, QObject *parent):
    QObject(parent),
    m_pFileDownloader(new FileDownloader),
    m_ChildState(basePath,true),
    m_NextProgramPath(nextProgramPath),
    m_CurrentPhase(Phase::IDLE),
    m_ParentIndexUrl(parentIndexUrl)
{
    //Slots can not be easily unconnected, so one slot is used that then
    //redirects to the currently needed method
    connect(m_pFileDownloader, SIGNAL (downloaded()), this, SLOT (downloadFinished()));
}

ChildOwl::~ChildOwl()
{
    delete m_pFileDownloader;
}

void ChildOwl::checkUpdates() {
    m_OnlyCheck = true;
    updateChildStateFile();
}

void ChildOwl::update() {
    m_OnlyCheck = false;
    //TODO: do nothing if currently already doing something
    m_DownloadIndex = -1; //reset download index for downloadFilesFromParent()
    updateChildStateFile();
}

ChildOwl::Phase ChildOwl::getCurrentPhase()
{
    return m_CurrentPhase;
}

void ChildOwl::downloadFinished() {
    switch(m_CurrentPhase) {
    case Phase::DOWNLOAD_PARENT_STATE: //TODO: remove states here, make a marker for download only, make states better for UI feedback
        compareParentChildState();
        break;
    case Phase::LOAD_NEW_FILES:
        downloadFilesFromParent();
        break;
    default:
        break;
        //qWarning() << "The current phase should have no download";
    }
}

void ChildOwl::updateChildStateFile()
{
    qDebug() << "Updating own";
    m_CurrentPhase = Phase::UPDATING_CHILD;

    m_ChildState.fillWithDirectory();
    qDebug() << m_ChildState.getSize();
    downloadParentStateFile();
}


void ChildOwl::downloadParentStateFile()
{
    qDebug() << "Downloading parent state";
    m_CurrentPhase = Phase::DOWNLOAD_PARENT_STATE;

    m_pFileDownloader->download(m_ParentIndexUrl);
}

void ChildOwl::compareParentChildState()
{
    qDebug() << "Comparing";
    m_CurrentPhase = Phase::COMPARE;

    QJsonObject json = m_pFileDownloader->getDownloadAsJson();
    StateFiles parentState(json);
    m_DifferenceStates = m_ChildState.difference(parentState);

    if(m_OnlyCheck) {
        bool updatesAvailable = !(m_DifferenceStates.first.isEmpty() && m_DifferenceStates.second.isEmpty());
        emit checkedUpdates(true, updatesAvailable);
    } else {
        removeOldFilesOnChild();
    }
}

void ChildOwl::removeOldFilesOnChild()
{
    qDebug() << "Removing";
    m_CurrentPhase = Phase::DELETE_OLD;

    //remove files that are only on child
    QVector<FileReference> onlyOnChild = m_DifferenceStates.first.getFileReferences();
    for(auto iter = onlyOnChild.begin(); iter != onlyOnChild.end(); ++iter) {
        qDebug() << "Removing " << iter->getFilePath();
        //Commented out, just in case, during development
        /*if(!QFile::remove(iter->getFilePath())) {
            qDebug() << "Failed removing " << iter->getFilePath();
        }*/
    }

    downloadFilesFromParent();
}

void ChildOwl::downloadFilesFromParent()
{
    m_CurrentPhase = Phase::LOAD_NEW_FILES;

    if(m_DownloadIndex != -1)
    {
        FileReference lastDownloadReference = m_DifferenceStates.second.getFileReferences()[m_DownloadIndex];//TODO:do not create copy
        //remote to local
        lastDownloadReference.changePathPrefix(m_DifferenceStates.second.getBasePath(),m_ChildState.getBasePath());
        qDebug() << "Writing " << lastDownloadReference.getFilePath();
        m_pFileDownloader->writeDownloadToFile(lastDownloadReference.getFilePath());
    }

    ++m_DownloadIndex;
    if(m_DownloadIndex == m_DifferenceStates.second.getFileReferences().size()) {
        m_CurrentPhase = Phase::IDLE;
        emit installedUpdates();
        qDebug() << "Done";
        return;
    }

    FileReference nextDownload = m_DifferenceStates.second.getFileReferences()[m_DownloadIndex];
    m_pFileDownloader->download(nextDownload.getFilePath());
    qDebug() << "Downloading " << nextDownload.getFilePath();

}

void ChildOwl::startNextProgram()
{
    QProcess::startDetached(m_NextProgramPath);
}
