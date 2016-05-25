#include <QFile>
#include <QByteArray>

#include "flogging.h"
#include "fdbupdater.h"

#include "fdb.h"
#include "libfusion.h"

FDBUpdater::FDBUpdater(QObject *parent, FDB *db) : QObject(parent)
{
    this->db = db;
}

bool FDBUpdater::checkForDBUpdate()
{
    qCDebug(fLib) << ("Checking for update...");
    QDir workingDir = LibFusion::getWorkingDir();
  //  QFile updateFile(workingDir.absolutePath() + QDir::separator() + "dbUpdate");
    QFile updateFile("dbUpdate");
    if (updateFile.exists())
    {
        if (!updateFile.open(QIODevice::ReadOnly))
            return false;

        QByteArray line;
        foundVersion = false;

        while (!foundVersion && !updateFile.atEnd())
        {
            line = updateFile.readLine();
            if (!line.startsWith("#") && !line.isEmpty() && line != "\n")
            {
                qCDebug(fLibDBUpdater) << ("Found update content" + line);
                if(line.startsWith("_"))
                {
                    qCDebug(fLibDBUpdater) << ("Found feature or release");
                    foundVersion = true;
                    latestVersion = line;
                }
            }
            qCDebug(fLibDBUpdater) << ("Found comment, ignoring...");
        }
        qCDebug(fLibDBUpdater) << ("Current version: " + db->getTextPref("dbVersion"));

        if (!foundVersion || line == db->getTextPref("dbVersion"))
        {
            qCDebug(fLibDBUpdater) << ("No update found");
        }
        else
        {
            qCDebug(fLibDBUpdater) << ("Update found!" + line);
            return true;
        }
    }
    else
    {
        qWarning(fLibDBUpdater) << ("Couldn't find a dbUpdate file.");
        return false;
    }
    return false;
}

bool FDBUpdater::updateDB()
{
    qCDebug(fLibDBUpdater) << ("Updating!");
    QDir workingDir = LibFusion::getWorkingDir();
 //   QFile updateFile(workingDir.absolutePath() + QDir::separator() + "dbUpdate");
    QFile updateFile("dbUpdate");

    if (updateFile.exists())
    {
        qCDebug(fLibDBUpdater) << ("dbUpdate-File Opend...");
        if(!updateFile.open(QIODevice::ReadOnly))
        {
            qCWarning(fLibDBUpdater) << ("Couldn't open");
            return false;
        }

        QByteArray line;
        QString currentVersion = db->getTextPref("dbVersion",0);
        QStack<QString> stack;
        line = updateFile.readLine();
        qCDebug(fLibDBUpdater) << ("New version:" + line + ", old version:" + currentVersion);
        QString newVersion;

        if (line.startsWith("_"))
            newVersion = line;

        while (line != currentVersion && !updateFile.atEnd())
        {
            if(!(line.startsWith("_") || line.startsWith("#") || line == "\n" || line.isEmpty()))
                stack.push(line);

            line = updateFile.readLine();
        }

        bool failed = false;

        if (!stack.isEmpty())
        {
            while(!stack.isEmpty())
            {
                QString query = stack.pop();
                qCDebug(fLibDBUpdater) << ("Running:" + query);
                failed |= db->runQuery(QSqlQuery(query));
            }
        }

        if (!failed)
        {
            db->updateTextPref("dbVersion", newVersion);
            db->endTransaction();
            qCDebug(fLibDBUpdater) << ("DB update successful.");
        }
        else
        {
            db->rollbackTransaction();
            qCritical(fLibDBUpdater) << ("Failed to update DB. The client may not work correctly.");
            return false;
        }
    }
    else
    {
        qCWarning(fLibDBUpdater) << ("Couldn't find a dbUpdate file.");
        return false;
    }

    return true;
}

void FDBUpdater::initVersion()
{
    checkForDBUpdate();

    if (latestVersion.isEmpty())
    {
        qCWarning(fLibDBUpdater) << ("Didn't find a version");
        return;
    }
    qCDebug(fLibDBUpdater) << ("Setting version to " + latestVersion);
    db->addTextPref("dbVersion", latestVersion);
}
