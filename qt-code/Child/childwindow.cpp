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

#include "childwindow.h"
#include "ui_childwindow.h"
#include <QUrl>
#include "childowl.h"
#include "filedownloader.h"
#include "filereference.h"

ChildWindow::ChildWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::MainWindow)
{

    loadSettings();

    m_pUi->setupUi(this);
    setWindowTitle(getStringSetting(SETTINGS_NAME_NEXT_NAME) + " - LittleOwl Update Utility");
    QObject::connect(m_pChildOwl,SIGNAL(checkedUpdates(bool,bool)),this,SLOT(checkedUpdates(bool,bool)));
    QObject::connect(m_pChildOwl,SIGNAL(installedUpdates()),this,SLOT(installedUpdates()));

    m_pChildOwl->checkUpdates();
}

ChildWindow::~ChildWindow()
{
    delete m_pUi;
    delete m_pSettings;
    delete m_pChildOwl;
}

void ChildWindow::loadSettings()
{
    m_pSettings = new QSettings(SETTINGS_FILE, QSettings::IniFormat);
    QString localBasePath = getStringSetting(SETTINGS_NAME_LOCAL_BASE);
    QString remoteIndexPath = getStringSetting(SETTINGS_NAME_REMOTE_INDEX);
    QString nextProgramPath = getStringSetting(SETTINGS_NAME_NEXT_PATH);
    m_pChildOwl = new ChildOwl(localBasePath,remoteIndexPath, nextProgramPath);
}

QString ChildWindow::getStringSetting(QString name)
{
    return m_pSettings->value(name).toString();
}

void ChildWindow::prepareFinishedState()
{
    m_UpdateAvailable = false;
    m_pUi->skipButton->setEnabled(false);
    m_pUi->updateButton->setText("Start " + getStringSetting(SETTINGS_NAME_NEXT_NAME));
    m_pUi->updateButton->setEnabled(true);
}

void ChildWindow::checkedUpdates(bool checkSuccessful, bool updateAvailable)
{
    qDebug() << updateAvailable;
    m_UpdateAvailable = updateAvailable;
    if(!checkSuccessful)
    {
        m_pUi->feedbackLabel->setText(tr("Not able to check for updates"));
        prepareFinishedState();
        return;
    }

    if(!updateAvailable)
    {
        m_pUi->feedbackLabel->setText(tr("No update available"));
        prepareFinishedState();
        return;
    }

    //else update available
    m_pUi->feedbackLabel->setText("An update is available");
    m_pUi->skipButton->setText("Do not install update");
    m_pUi->updateButton->setEnabled(true);
    m_pUi->updateButton->setText("Install update");
}

void ChildWindow::installedUpdates()
{
    m_pUi->feedbackLabel->setText("Update successful");
    prepareFinishedState();
}

void ChildWindow::on_updateButton_clicked()
{
    if(m_UpdateAvailable)
    {
        m_pUi->feedbackLabel->setText("Downloading and installing updates");
        m_pUi->skipButton->setEnabled(false);
        m_pUi->updateButton->setEnabled(false);
        m_pUi->updateButton->setText("Please wait");
        m_pChildOwl->update();
    }
    else
    {
        m_pChildOwl->startNextProgram();
        QApplication::quit();
    }
}

void ChildWindow::on_skipButton_clicked()
{
    m_pChildOwl->startNextProgram();
    QApplication::quit();
}
