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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "childowl.h"

namespace Ui {
class MainWindow;
}

/**
* Window application that checks for updates and installs updates.
* Once the update procedure is finished, the user can start the
* actual program.
*
* Several settings can be set in the config.ini file.
*/
class ChildWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChildWindow(QWidget *parent = 0);
    ~ChildWindow();

private slots:
    void checkedUpdates(bool checkSuccessful, bool updateAvailable);
    void installedUpdates();
    void on_updateButton_clicked();
    void on_skipButton_clicked();

private:
    void loadSettings();
    /**
     * Get a String value for the setting with
     * the given name from the settings file.
     */
    QString getStringSetting(QString name);
    /**
     * Set the UI so that the user can start
     * the next program.
     */
    void prepareFinishedState();
    Ui::MainWindow *m_pUi;
    QSettings* m_pSettings;
    ChildOwl* m_pChildOwl;
    bool m_UpdateAvailable;

    const QString SETTINGS_FILE = "config.ini";
    const QString SETTINGS_NAME_LOCAL_BASE = "path/localBase";
    const QString SETTINGS_NAME_REMOTE_INDEX = "path/remoteIndex";
    const QString SETTINGS_NAME_NEXT_PATH = "path/nextProgram";
    const QString SETTINGS_NAME_NEXT_BUTTON = "gui/textNext";

};

#endif // MAINWINDOW_H
