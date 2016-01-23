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

#ifndef CHILDOWL_H
#define CHILDOWL_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QVector>
#include <utility>
#include "statefiles.h"
#include "filedownloader.h"

/**
* Manages the state of the files on the child/client. Used by the child window application.
*/
class ChildOwl: public QObject
{
    Q_OBJECT
public:
    explicit ChildOwl(QString basePath, QString parentIndexUrl, QString nextProgramPath, QObject *parent = 0);
    virtual ~ChildOwl();
    enum class Phase {IDLE,UPDATING_CHILD,DOWNLOAD_PARENT_STATE,COMPARE,DELETE_OLD,LOAD_NEW_FILES};
    /**
     * Check if an update exists on the server.
     */
    void checkUpdates();
    /**
     * Download and install the update from the server.
     */
    void update();
    /**
     * Start the program that should follow the updater.
     */
    void startNextProgram();
    Phase getCurrentPhase(); //TODO: Better feedback into UI

signals:
    void checkedUpdates(bool checkSuccessful, bool updateAvailable);
    void installedUpdates();

private slots:
    void downloadFinished();

private:
    /**
     * Update StateFiles of local files
     */
    void updateChildStateFile();
    /**
     * Download StateFiles from remote parent
     */
    void downloadParentStateFile();
    void compareParentChildState();
    void removeOldFilesOnChild();
    void downloadFilesFromParent();

    FileDownloader *m_pFileDownloader;
    StateFiles m_ChildState;
    QString m_NextProgramPath;
    QString m_NextProgramName;
    std::pair<StateFiles,StateFiles> m_DifferenceStates;
    Phase m_CurrentPhase;
    QVector<FileReference> m_OnlyOnParent;
    int m_DownloadIndex;
    QUrl m_ParentIndexUrl;
    bool m_OnlyCheck;
};

#endif // CHILDOWL_H
