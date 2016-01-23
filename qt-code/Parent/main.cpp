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


#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QCommandLineParser>
#include "parentowl.h"

/**
 * Std-output in analogy to qDebug()
 */
QTextStream& qOut()
{
    static QTextStream r(stdout);
    return r;
}

/**
* Console application to create the index file. The created index file is placed
* on the server and retrieved by the client to compare it to the local client version.
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("LittleOwl - Parent");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main","Index file generator for LittleOwl."));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption filesDirectoryOption(QStringList() << "f" << "files",
                                             QCoreApplication::translate("main", "The path (on this computer) to the directory with the files that should be synchronized."),
                                            "files");
    parser.addOption(filesDirectoryOption);

    QCommandLineOption remoteDirectoryOption(QStringList() << "r" << "remote",
                                             QCoreApplication::translate("main", "The url with which the file directory is reached by the client."),
                                             "remote");
    parser.addOption(remoteDirectoryOption);

    QCommandLineOption indexPathOption(QStringList() << "i" << "index",
                                             QCoreApplication::translate("main", "The path where the index file is created."),
                                       "index");
    parser.addOption(indexPathOption);

    parser.process(a);

    QString filesDirectory = parser.value(filesDirectoryOption);
    QString remoteDirectory = parser.value(remoteDirectoryOption);
    QString indexPath = parser.value(indexPathOption);

    qOut() << "Creating the index file using the following parameters: \n";
    qOut() << "File directory: " << filesDirectory << "\n";
    qOut() << "Remote directory: " << remoteDirectory << "\n";
    qOut() << "Index file: " << indexPath << "\n";

    if(filesDirectory.size() == 0 || remoteDirectory.size() == 0 || indexPath.size() == 0)
    {
        qCritical() << "The arguments must not be empty. Use --help for more details.";
        return 0;
    }

    ParentOwl owl(filesDirectory,remoteDirectory,indexPath);
    owl.update();

    qOut() << "\nIndex file was created. You can find it at " << indexPath << ".\n";

    return 0;
}
