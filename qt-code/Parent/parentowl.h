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


#ifndef PARENTOWL_H
#define PARENTOWL_H

#include <QObject>
#include "statefiles.h"

/**
* Manages the state of the files on the parent/server. Used by the parent console application.
*/
class ParentOwl : public QObject
{
    Q_OBJECT
public:
    /**
     * @param basePath The path (on this computer) to the directory with the files that should be synchronized.
     * @param remotePath The url with which the file directory is reached by the client.
     * @param indexPath The path where the index file is created.
     * @param parent Qt parent
     */
    explicit ParentOwl(QString basePath, QString remotePath, QString indexPath, QObject *parent = 0);
    /**
     * Updates the interal state of the files and creates the index file of the parent file version.
     */
    void update();

signals:

public slots:

private:
    void rebuildStateFiles();

    StateFiles m_CurrentState;
    QString m_BasePath;
    QString m_RemotePath;
    QString m_IndexPath;

};

#endif // PARENTOWL_H
