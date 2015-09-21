#include "fdbupdater.h"
#include <QFile>
#include <QByteArray>
#include "fdb.h"
#include "f_dbg.h"
#include "libfusion.h"

FDBUpdater::FDBUpdater(QObject *parent, FDB *db) : QObject(parent)
{
    this->db = db;
}

bool FDBUpdater::checkForDBUpdate()
{
    DBG_DBU("Checking for update...");
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
                DBG_DBU("Found update content" + line);
                if(line.startsWith("_"))
                {
                    DBG_DBU("Found feature or release");
                    foundVersion = true;
                    latestVersion = line;
                }
            }
            DBG_DBU("Found comment, ignoring...");
        }
        DBG_DBU("Current version: " + db->getTextPref("dbVersion"));

        if(!foundVersion || line == db->getTextPref("dbVersion"))
        {
            DBG_DBU("No update found");
        }
        else
        {
            DBG_DBU("Update found!" + line);
            return true;
        }
    }
    else
    {
        DBG_DBU("Couldn't find a dbUpdate file.");
        return false;
    }
    return false;
}

bool FDBUpdater::updateDB()
{
    DBG_DBU("Updating!");
    QDir workingDir = LibFusion::getWorkingDir();
 //   QFile updateFile(workingDir.absolutePath() + QDir::separator() + "dbUpdate");
    QFile updateFile("dbUpdate");

    if(updateFile.exists())
    {
        DBG_DBU("dbUpdate-File Opend...");
        if(!updateFile.open(QIODevice::ReadOnly))
        {
            DBG_DBU("Couldn't open");
            return false;
        }

        QByteArray line;
        QString currentVersion = db->getTextPref("dbVersion",0);
        QStack<QString> stack;
        line = updateFile.readLine();
        DBG_DBU("New version:" + line + ", old version:" + currentVersion);
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
                DBG_DBU("Running:" + query);
                failed |= db->runQuery(QSqlQuery(query));
            }
        }
        if(!failed)
        {
            db->updateTextPref("dbVersion", newVersion);
            db->endTransaction();
            DBG_DBU("DB update successful.");
        }
        else
        {
            db->rollbackTransaction();
            DBG_DBU("Failed to update DB. The client may not work correctly.");
            return false;
        }


    }
    else
    {
        DBG_DBU("Couldn't find a dbUpdate file.");
        return false;
    }
    return true;
}

void FDBUpdater::initVersion()
{
    checkForDBUpdate();
    if(latestVersion.isEmpty())
    {
        DBG_DBU("Didn't find a version");
        return;
    }
    DBG_DBU("Setting version to " + latestVersion);
    db->addTextPref("dbVersion", latestVersion);
}
