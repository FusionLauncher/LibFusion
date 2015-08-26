#include "fdbupdater.h"
#include <QFile>
#include <QByteArray>
#include "fdb.h"
#include "libfusion.h"

FDBUpdater::FDBUpdater(QObject *parent, FDB *db) : QObject(parent)
{
    this->db = db;
}

bool FDBUpdater::checkForDBUpdate()
{
    qDebug() << "Checking for update...";
    QDir workingDir = LibFusion::getWorkingDir();
  //  QFile updateFile(workingDir.absolutePath() + QDir::separator() + "dbUpdate");
    QFile updateFile("dbUpdate");
    if(updateFile.exists())
    {
        if(!updateFile.open(QIODevice::ReadOnly))
        {
            return false;
        }
        QByteArray line;
        foundVersion = false;
        while(!foundVersion && !updateFile.atEnd())
        {
            line = updateFile.readLine();
            if(!line.startsWith("#") && !line.isEmpty() && line != "\n")
            {
                qDebug() << "Found update content" << line;
                if(line.startsWith("_"))
                {
                    qDebug() << "Found feature or release";
                    foundVersion = true;
                    latestVersion = line;
                }
            }
            qDebug() << "Found comment, ignoring...";
        }
        qDebug() << "Current version: " << db->getTextPref("dbVersion");

        if(!foundVersion || line == db->getTextPref("dbVersion"))
        {
            qDebug() << "No update found";
        }
        else
        {
            qDebug() << "Update found!" << line;
            return true;
        }
    }
    else
    {
        qDebug() << "Couldn't find a dbUpdate file.";
        return false;
    }
    return false;
}

bool FDBUpdater::updateDB()
{
    qDebug() << "Updating!";
    QDir workingDir = LibFusion::getWorkingDir();
 //   QFile updateFile(workingDir.absolutePath() + QDir::separator() + "dbUpdate");
    QFile updateFile("dbUpdate");

    if(updateFile.exists())
    {
        qDebug() << "dbUpdate-File Opend...";
        if(!updateFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open";
            return false;
        }

        QByteArray line;
        QString currentVersion = db->getTextPref("dbVersion",0);
        QStack<QString> stack;
        line = updateFile.readLine();
        qDebug() << "New version:" << line << ", old version:" << currentVersion;
        QString newVersion;

        if(line.startsWith("_"))
        {
            newVersion = line;
        }
        while(line != currentVersion && !updateFile.atEnd())
        {
            if(!(line.startsWith("_") || line.startsWith("#") || line == "\n" || line.isEmpty()))
            {
                stack.push(line);
            }
            line = updateFile.readLine();
        }
        bool failed = false;
        if(!stack.isEmpty())
        {
            while(!stack.isEmpty())
            {
                QString query = stack.pop();
                qDebug() << "Running:" << query;
                failed |= db->runQuery(QSqlQuery(query));
            }
        }
        if(!failed)
        {
            db->updateTextPref("dbVersion", newVersion);
            db->endTransaction();
            qDebug() << "DB update successful.";
        }
        else
        {
            db->rollbackTransaction();
            qDebug() << "Failed to update DB. The client may not work correctly.";
            return false;
        }


    }
    else
    {
        qDebug() << "Couldn't find a dbUpdate file.";
        return false;
    }
    return true;
}

void FDBUpdater::initVersion()
{
    checkForDBUpdate();
    if(latestVersion.isEmpty())
    {
        qDebug() << "Didn't find a version";
        return;
    }
    qDebug() << "Setting version to" << latestVersion;
    db->addTextPref("dbVersion", latestVersion);
}
