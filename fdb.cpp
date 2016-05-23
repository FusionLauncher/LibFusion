#include "fdb.h"
#include "fwatchedfolder.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <fgame.h>
#include <fdb.h>
#include <libfusion.h>
#include "fdbupdater.h"
#include <QFile>

FDB::FDB(QObject *parent)
{

}

bool FDB::init()
{
    if(!LibFusion::makeSureWorkingDirExists())
    {
        qCCritical(fLibDB) << ("Unable to create/access working Dir!");
	    return false;
    }

    QDir workingDir = LibFusion::getWorkingDir();
    QFile dbFile(workingDir.absolutePath() + QDir::separator() + "fusion.db");

    bool createDB = false;
    if(!dbFile.exists())
    {
        qCDebug(fLibDB) << ("Database will be created.");
        createDB = true;
    }
    bool initSuccessful = true;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");

    db.setDatabaseName(QFileInfo(dbFile).absoluteFilePath());
    bool ok = db.open();
    if(!ok)
    {
        QSqlError e = db.lastError();

        qCCritical(fLibDB) << ("Unable to open Database!");
        qCCritical(fLibDB) << (e.databaseText());
        qCCritical(fLibDB) << (e.driverText());
        qCCritical(fLibDB) << (e.nativeErrorCode());
        qCCritical(fLibDB) << (QString::number(e.type()));
        return false;
    }

    QSqlQuery query;
    FDBUpdater updater(this, this);
    if(createDB)
    {
        qCDebug(fLibDB) << ("Creating database.");
        query.exec("CREATE TABLE IF NOT EXISTS prefs(key TINYTEXT NOT NULL, valuetype TINYINT NOT NULL, number TINYINT NOT NULL, text VARCHAR(255) NOT NULL)");
        //(later) if clientToken doesnt exists, show login and run registerClient(), if no account, //run register()
        //(later) if lang is not set, set it to the default system language
        query.exec("CREATE TABLE IF NOT EXISTS games(id INTEGER PRIMARY KEY ASC, gameName TEXT NOT NULL, gameType TINYINT NOT NULL , gameDirectory TEXT NOT NULL, relExecutablePath TEXT NOT NULL, gameCommand TEXT, gameArgs TEXT, gameLauncher INTEGER, savegameDir TEXT, lastLaunched datetime DEFAULT NULL)");
        query.exec("CREATE TABLE IF NOT EXISTS watchedFolders ( `id` INTEGER PRIMARY KEY ASC, `path` VARCHAR(255),forLauncher TINY INT DEFAULT '0', launcherID INT DEFAULT NULL );");
        query.exec("CREATE TABLE IF NOT EXISTS launchers(id INTEGER PRIMARY KEY ASC, launcherName TEXT NOT NULL, launcherPath TEXT NOT NULL, launcherArgs TEXT NOT NULL, suffix TEXT)");


        qCDebug(fLibDB) << ("Database created.");
        updater.initVersion();
    }

    if(updater.checkForDBUpdate())
    {
        qCDebug(fLibDB) << ("Found an update, updating db!");
        initSuccessful = updater.updateDB();
    }

    return initSuccessful;
}

bool FDB::addGame(FGame game)
{
    QSqlQuery gameQuery;
    if(!launcherExists(game.getLauncher()))
    {
        addLauncher(game.getLauncher());
    }

    gameQuery.prepare("INSERT INTO games(gameName, gameType, gameDirectory, relExecutablePath, gameCommand, gameArgs, gameLauncher) VALUES (:gameName, :gameType, :gameDirectory, :relExecutablePath, :gameCommand, :gameArgs, :gameLauncher)");
    gameQuery.bindValue(":gameName", game.getName());
    gameQuery.bindValue(":gameType", game.getType());
    gameQuery.bindValue(":gameDirectory", game.getPath());
    gameQuery.bindValue(":relExecutablePath", game.getExe());
    gameQuery.bindValue(":gameCommand", game.getCommand());
    gameQuery.bindValue(":gameArgs", game.getArgs());

    if(game.doesUseLauncher())
        gameQuery.bindValue(":gameLauncher", game.getLauncher().getDbId());
    else
        gameQuery.bindValue(":gameLauncher", -1);


    qCDebug(fLibDB) << ("Game Added: " + game.getName().toLatin1());
    return tryExecute(&gameQuery);
}

bool FDB::removeGameById(int id)
{
    QSqlQuery removalQuery;
    removalQuery.prepare("DELETE FROM games WHERE id = :id");
    removalQuery.bindValue(":id", id);

    return tryExecute(&removalQuery);
}


FGame* FDB::createGameFromQuery(QSqlQuery query)
{
    FGame* game = new FGame();
    game->setName(query.value(0).toString());
    game->setPath(query.value(2).toString());
    game->setExe(query.value(3).toString());
    game->setType((FGameType)query.value(1).toInt());
    game->setCommand(query.value(4).toString());
    game->setArgs(query.value(5).toStringList());
    game->dbId = query.value(7).toInt();
    QString lp = query.value(9).toString();
    game->setGameLastPlayed(QDateTime::fromString(lp, "yyyy-MM-dd HH:mm:ss"));

    bool getLauncherOK;
    int launcherID = query.value(6).toInt(&getLauncherOK);
    if(getLauncherOK && launcherID != -1)
    {
        FLauncher launcher = getLauncher(launcherID);
        game->setLauncher(launcher);
    }
    game->setSavegameDir(query.value(8).toString());
    return game;
}

FGame* FDB::getGame(int id)
{
    QSqlQuery gameQuery;
    gameQuery.prepare("SELECT gameName, gameType, gameDirectory, relExecutablePath, gameCommand, gameArgs, gameLauncher, id, savegameDir, lastLaunched FROM games WHERE id = :id");
    gameQuery.bindValue(":id", id);
    tryExecute(&gameQuery);

    if(! gameQuery.next())
    {
        return NULL;
    }
    return createGameFromQuery(gameQuery);
}


QList<FGame*> FDB::getGameList()
{
    QList<FGame*> gameList;
    QSqlQuery libraryQuery;
    FGame game;
    libraryQuery.prepare("SELECT gameName, gameType, gameDirectory, relExecutablePath, gameCommand, gameArgs, gameLauncher, id, savegameDir, lastLaunched FROM games ORDER BY gameName ASC");
    tryExecute(&libraryQuery);
    while(libraryQuery.next())
    {
        gameList.append(createGameFromQuery(libraryQuery));
    }
    return gameList;
}

int FDB::getGameCount()
{
    QSqlQuery countQuery;
    countQuery.prepare("SELECT count() FROM games");
    tryExecute(&countQuery);
    if(!countQuery.next())
    {
        return 0;
    }
    qCDebug(fLibDB) << "Games found: " << countQuery.value(0).toString().toLatin1();
    return countQuery.value(0).toInt();

}



QString FDB::getTextPref(QString pref, QString defaultValue)
{
    QString val = getTextPref(pref);

    //"" not an optimal result for "not found".
    //have to find something better here, maybe add a exception
    if(val != "" || defaultValue == "")
        return val;
     else
        addTextPref(pref, defaultValue);

    return defaultValue;
}

QString FDB::getTextPref(QString pref)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("SELECT (text) FROM prefs WHERE key = :key AND valuetype = 1");
    prefQuery.bindValue(":key", pref);
    tryExecute(&prefQuery);
    if(!prefQuery.next())
    {
        return QString();
    }
    else
    {
        return prefQuery.value(0).toString();
    }
}

bool FDB::addTextPref(QString pref, QString value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("INSERT INTO prefs(key, valuetype, number, text) VALUES (:key, 1, 0, :value)");
    prefQuery.bindValue(":key", pref);
    prefQuery.bindValue(":value", value);
    return tryExecute(&prefQuery);
}

bool FDB::updateTextPref(QString pref, QString value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET text = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);
    bool res = tryExecute(&prefQuery);


    if(prefQuery.numRowsAffected() == 0) {
        qCDebug(fLibDB) << ("Added Text-Pref:" + pref);
        return addTextPref(pref, value);
    } else {
        return res;
    }

}

bool FDB::deletePref(QString pref)
{
    QSqlQuery delQuery;
    delQuery.prepare("DELETE FROM prefs WHERE key = :key");
    delQuery.bindValue(":key", pref);
    return tryExecute(&delQuery);
}


int FDB::getIntPref(QString pref, int defaultValue)
{
    int val = getIntPref(pref);

    //-1 not an optimal result for "not found".
    //have to find something better here, maybe add a exception
    if(val != -1 || defaultValue == -1)
        return val;
     else
        addIntPref(pref, defaultValue);

    return defaultValue;
}

int FDB::getIntPref(QString pref)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("SELECT (number) FROM prefs WHERE key = :key AND valuetype = 2");
    prefQuery.bindValue(":key", pref);
    tryExecute(&prefQuery);
    if(!prefQuery.next())
    {
        return -1;
    }
    else
    {
        return prefQuery.value(0).toInt();
    }
}

bool FDB::addIntPref(QString pref, int value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("INSERT INTO prefs(key, valuetype, number, text) VALUES (:key, 2, :value, '')");
    prefQuery.bindValue(":key", pref);
    prefQuery.bindValue(":value", value);
    return tryExecute(&prefQuery);
}

bool FDB::updateIntPref(QString pref, int value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET number = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);

    bool res = tryExecute(&prefQuery);

    if(prefQuery.numRowsAffected() == 0) {
        qCDebug(fLibDB) << ("Added Int-Pref:" + pref);
        return addIntPref(pref, value);
    } else {
        return res;
    }
}



bool FDB::getBoolPref(QString pref, bool defaultValue)
{
    bool val = defaultValue;
    try  {
          val = getBoolPref(pref);
    } catch(int i) {

            addBoolPref(pref, defaultValue);
            qCDebug(fLibDB) << ("added Pref: " + pref);
    }

    return val;
}

bool FDB::updateGame(FGame *g)
{
    QSqlQuery q;
    q.prepare("UPDATE games SET gameName = :gName, gameDirectory = :gDir, relExecutablePath = :exec, gameLauncher = :gLauncher, savegameDir = :gSavegameDir WHERE id = :gID");
    q.bindValue(":gName", g->getName());
    q.bindValue(":gDir", g->getPath());
    q.bindValue(":exec", g->getExe());
    if(g->doesUseLauncher())
    {
        q.bindValue(":gLauncher", g->getLauncher().getDbId());
    }
    else
    {
        q.bindValue(":gLauncher", QVariant(QVariant::String));
    }
	
	if(g->savegameSyncEndabled())
        q.bindValue(":gSavegameDir", g->getSavegameDir().absolutePath());
    else
        q.bindValue(":gSavegameDir", QVariant(QVariant::String));
		
    q.bindValue(":gID", g->dbId);
    return tryExecute(&q);

}

bool FDB::updateLastLaunched(FGame *g)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery q;
    q.prepare("UPDATE games SET lastLaunched = :dateTime WHERE id = :gID");
    q.bindValue(":gID", g->dbId);
    q.bindValue(":dateTime", currentDateTime);
    return tryExecute(&q);

}

QList<FGame *> FDB::getLatestLaunchedGames(int limit)
{
    QList<FGame*> gameList;
    QSqlQuery q;
    FGame game;
    q.prepare("SELECT gameName, gameType, gameDirectory, relExecutablePath, gameCommand, gameArgs, gameLauncher, id, savegameDir, lastLaunched FROM games  ORDER BY lastLaunched DESC LIMIT :limit");
    q.bindValue(":limit", limit);

    tryExecute(&q);
    while(q.next())
    {
        gameList.append(createGameFromQuery(q));
    }
    return gameList;
}

bool FDB::updateLauncher(FLauncher launcher)
{
    QSqlQuery q;
    q.prepare("UPDATE launchers SET launcherName = :lName, launcherPath = :lPath, launcherArgs = :lArgs, suffix = :lSuffix WHERE id = :lId");
    q.bindValue(":lName", launcher.getName());
    q.bindValue(":lPath", launcher.getPath());
    q.bindValue(":lArgs", launcher.getArgs());
    q.bindValue(":lId", launcher.getDbId());
    q.bindValue(":lSuffix", launcher.getFileEndings());
    return tryExecute(&q);
}


bool FDB::getBoolPref(QString pref)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("SELECT (number) FROM prefs WHERE key = :key AND valuetype = 3");
    prefQuery.bindValue(":key", pref);
    tryExecute(&prefQuery);

    if(!prefQuery.next())
        throw 20;
    else
        return prefQuery.value(0).toBool();
}

bool FDB::addBoolPref(QString pref, bool value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("INSERT INTO prefs(key, valuetype, number, text) VALUES (:key, 3, :value, '')");
    prefQuery.bindValue(":key", pref);
    prefQuery.bindValue(":value", value);
    return tryExecute(&prefQuery);
}

bool FDB::updateBoolPref(QString pref, bool value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET number = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);

    bool res = tryExecute(&prefQuery);

    if(prefQuery.numRowsAffected() == 0) {
        qCDebug(fLibDB) << ("Added Int-Pref:" + pref);
        return addBoolPref(pref, value);
    } else {
        return res;
    }
}


bool FDB::updateWatchedFolders(QList<FWatchedFolder> data)
{
    QSqlQuery updateQuery;
    updateQuery.prepare("DELETE FROM watchedFolders");
    tryExecute(&updateQuery);

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO watchedFolders (path, launcherID, forLauncher) VALUES(:folder, :launcherID, :forLauncher)");

    for(FWatchedFolder dir : data) {
        insertQuery.bindValue(":folder", dir.getDirectory().absolutePath());
        insertQuery.bindValue(":launcherID", dir.getLauncherID());
        insertQuery.bindValue(":forLauncher", dir.forLauncher);
        tryExecute(&insertQuery);
        insertQuery.finish();
        qCDebug(fLibDB) << ("Add Lib: " + dir.getDirectory().absolutePath().toLatin1());
    }

    return true;
}

QList<FWatchedFolder> FDB::getWatchedFoldersList() {
    QList<FWatchedFolder> result;
    QSqlQuery folderqQueue;

    folderqQueue.exec("SELECT path, id, launcherID, forLauncher FROM watchedFolders ORDER BY path ASC");
    while(folderqQueue.next())
    {
        FWatchedFolder folder;
        folder.setDirectory(QDir(folderqQueue.value(0).toString()));
        folder.setDbID(folderqQueue.value(1).toInt());

        folder.forLauncher = folderqQueue.value(3).toBool();

        if(folder.forLauncher)
            folder.setLauncherID(folderqQueue.value(2).toInt());

        result.append(folder);
    }
    return result;

}

bool FDB::watchedFolderExists(FWatchedFolder *wf)
{
    QSqlQuery q;
    q.prepare("SELECT count(*) as c FROM watchedFolders WHERE path = :wfPath AND launcherID = :wfLauncherID AND forLauncher = :wfforLauncher");
    q.bindValue(":wfLauncherID", wf->getLauncherID());
    q.bindValue(":wfPath", wf->getDirectory().absolutePath());
    q.bindValue(":wfforLauncher", wf->forLauncher);
    tryExecute(&q);
    q.next();
    return q.value(0).toInt()>0;
}

bool FDB::addWatchedFolder(FWatchedFolder wf)
{
    QSqlQuery q;
    q.prepare("INSERT INTO watchedFolders (path, launcherID, forLauncher) VALUES (:wfPath, :wfLauncherID, :wfforLauncher);");
    q.bindValue(":wfLauncherID", wf.getLauncherID());
    q.bindValue(":wfPath", wf.getDirectory().absolutePath());
    q.bindValue(":wfforLauncher", wf.forLauncher?1:0);
    return tryExecute(&q);
}


bool FDB::tryExecute(QSqlQuery *q) {
    bool queryOK = q->exec();

    if(!queryOK) {
        QString queryStr = q->lastQuery();
        QSqlError e = q->lastError();

        QString boundValues;
        QMapIterator<QString, QVariant> i(q->boundValues());
        while (i.hasNext()) {
            i.next();
            boundValues += i.key() + ": '" + i.value().toString() + "'; ";
        }

        qCWarning(fLibDB) << ("####################");
        qCWarning(fLibDB) << (e.databaseText());
        qCWarning(fLibDB) << (e.driverText());
        qCWarning(fLibDB) << (queryStr);
        qCWarning(fLibDB) << (boundValues);
    }

    return queryOK;

}

//These two speed up mainly insert-queries, sice SQLite does not write to disk after every command, but after the endTransaction()

//DON'T FROGET TO RUN endTransaction() AFTERWARDS!
//DON'T FROGET TO RUN endTransaction() AFTERWARDS!
bool FDB::beginTransaction()
{
    QSqlQuery q;
    return q.exec("BEGIN TRANSACTION;");
}

bool FDB::endTransaction()
{
    QSqlQuery q;
    return q.exec("END TRANSACTION;");
}

bool FDB::rollbackTransaction()
{
    QSqlQuery q;
    return q.exec("ROLLBACK TRANSACTION;");
}

//TODO: Assume we can remove this
bool FDB::runQuery(QSqlQuery q)
{
    //return q.exec();
    return false;
}

bool FDB::gameExists(FGame game)
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM games WHERE relExecutablePath = :Exe");
    query.bindValue(":Exe", game.getExe());
    tryExecute(&query);
    query.next();

    if(query.value(0).toInt()>0)
        return true;
    else
        return false;

}

bool FDB::launcherExists(FLauncher launcher)
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM launchers WHERE id = :id");
    query.bindValue(":id", launcher.getDbId());
    tryExecute(&query);
    query.next();
    return query.value(0).toInt()>0;
}

QDir FDB::getSavegameDir()
{
    return QDir();
}


int FDB::addLauncher(FLauncher launcher)
{
    QSqlQuery query;
    query.prepare("INSERT INTO launchers(launcherName, launcherPath, launcherArgs, suffix) VALUES (:name, :path, :args, :suffix)");
    query.bindValue(":name", launcher.getName());
    query.bindValue(":path", launcher.getPath());
    query.bindValue(":args", launcher.getArgs());
    query.bindValue(":suffix", launcher.getFileEndings());
    return tryExecute(&query);
}

bool FDB::updateLaunchers(QList<FLauncher> launchers)
{
    for(int i=0;i<launchers.length();++i) {
        if(launchers[i].getDbId() == -1) { //we need to insert it
            addLauncher(launchers[i]);
        } else {
            updateLauncher(launchers[i]);
        }
    }
}

FLauncher FDB::getLauncher(int dbId)
{
    FLauncher launcher;
    QSqlQuery query;
    query.prepare("SELECT launcherName, launcherPath, launcherArgs, suffix FROM launchers WHERE id = :id");
    query.bindValue(":id", dbId);
    tryExecute(&query);
    qCDebug(fLibDB) << ("Getting launcher" + dbId);
    if(!query.next())
    {
        qCCritical(fLibDB) << ("Didn't find the launcher..");
        return FLauncher();
    }
    launcher.setDbId(dbId);
    launcher.setName(query.value(0).toString());
    launcher.setPath(query.value(1).toString());
    launcher.setArgs(query.value(2).toString());
    launcher.setFileEndings(query.value(3).toString());
    return launcher;
}

QList<FLauncher> FDB::getLaunchers()
{
    QList<FLauncher> list;
    QSqlQuery query;
    query.prepare("SELECT launcherName, launcherPath, launcherArgs, id, suffix FROM launchers");
    tryExecute(&query);
    while(query.next())
    {
        FLauncher launcher;
        launcher.setName(query.value(0).toString());
        launcher.setPath(query.value(1).toString());
        launcher.setArgs(query.value(2).toString());
        launcher.setDbId(query.value(3).toInt());
        launcher.setFileEndings(query.value(4).toString());
        list.append(launcher);
    }
    return list;
}
