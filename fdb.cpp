#include "fdb.h"
#include "fwatchedfolder.h"

#include <QSqlDatabase>
#include <QSqlQuery>
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
	    return false;
    }
    QDir workingDir = LibFusion::getWorkingDir();
    QFile dbFile(workingDir.absolutePath() + QDir::separator() + "fusion.db");

    bool createDB = false;
    if(!dbFile.exists())
    {
        createDB = true;
    }
    bool initSuccessful = true;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");

    db.setDatabaseName(QFileInfo(dbFile).absoluteFilePath());
    bool ok = db.open();
    //ui->label->setText(ok ? "Connected!" : "Failed");
    if(!ok)
    {
        //show a popup window / alert about the error
        return false;
    }
    QSqlQuery query;
    FDBUpdater updater(this, this);
    if(createDB)
    {
        qDebug() << "Creating database.";
        query.exec("CREATE TABLE IF NOT EXISTS prefs(key TINYTEXT NOT NULL, valuetype TINYINT NOT NULL, number TINYINT NOT NULL, text VARCHAR(255) NOT NULL)");
        //(later) if clientToken doesnt exists, show login and run registerClient(), if no account, //run register()
        //(later) if lang is not set, set it to the default system language
        query.exec("CREATE TABLE IF NOT EXISTS games(id INTEGER PRIMARY KEY ASC, gameName TEXT NOT NULL, gameType TINYINT NOT NULL , gameDirectory TEXT NOT NULL, relExecutablePath TEXT NOT NULL, gameCommand TEXT, gameArgs TEXT, gameLauncher INTEGER)");
        query.exec("CREATE TABLE IF NOT EXISTS watchedFolders ( `id` INTEGER PRIMARY KEY ASC, `path` VARCHAR(255) );");
        query.exec("CREATE TABLE IF NOT EXISTS launchers(id INTEGER PRIMARY KEY ASC, launcherName TEXT NOT NULL, launcherPath TEXT NOT NULL, launcherArgs TEXT NOT NULL)");
        updater.initVersion();
    }
    if(updater.checkForDBUpdate())
    {
        qDebug() << "Found an update, updating!";
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


    qDebug("Game Added: " + game.getName().toLatin1());
    return gameQuery.exec();
}

bool FDB::removeGameById(int id)
{
    QSqlQuery removalQuery;
    removalQuery.prepare("DELETE FROM games WHERE id = :id");
    removalQuery.bindValue(":id", id);
    removalQuery.exec();
    //TODO: return false in case of an error
    return true;
}

FGame* FDB::getGame(int id)
{
    QSqlQuery gameQuery;
    gameQuery.prepare("SELECT gameName, gameType, gameDirectory, relExecutablePath, gameCommand, gameArgs, gameLauncher, savegameDir FROM games WHERE id = :id");
    gameQuery.bindValue(":id", id);
    gameQuery.exec();

    if(! gameQuery.next())
    {
        return NULL;
    }

    FGame *game = new FGame();
    game->setName(gameQuery.value(0).toString());
    game->setType((FGameType)gameQuery.value(1).toInt());
    game->setPath(gameQuery.value(2).toString());
    game->setExe(gameQuery.value(3).toString());
    game->setCommand(gameQuery.value(4).toString());
    game->setArgs(gameQuery.value(5).toStringList());


    bool getLauncherOK;
    int launcherID = gameQuery.value(6).toInt(&getLauncherOK);
    if(getLauncherOK)
    {
        FLauncher launcher = getLauncher(launcherID);
        game->setLauncher(launcher);
    }
    game->setSavegameDir(gameQuery.value(7).toString());

    game->dbId = id;
    return game;
}

QList<FGame> FDB::getGameList()
{
    QList<FGame> gameList;
    QSqlQuery libraryQuery;
    FGame game;
    libraryQuery.exec("SELECT gameName, gameType, gameDirectory, relExecutablePath, id, gameCommand, gameArgs, gameLauncher, savegameDir FROM games ORDER BY gameName ASC");
    while(libraryQuery.next())
    {
        game.setName(libraryQuery.value(0).toString());
        game.setType((FGameType)libraryQuery.value(1).toInt());
        game.setPath(libraryQuery.value(2).toString());
        game.setExe(libraryQuery.value(3).toString());
        game.dbId = libraryQuery.value(4).toInt();
        game.setCommand(libraryQuery.value(5).toString());
        game.setArgs(libraryQuery.value(6).toStringList());

        bool convertOK;
        int launcherID = libraryQuery.value(7).toInt(&convertOK);
        if(convertOK)
        {
            FLauncher launcher = getLauncher(launcherID);
            game.setLauncher(launcher);
        }

        game.setSavegameDir(libraryQuery.value(8).toString());
        gameList.append(game);
    }
    return gameList;
}

bool FDB::updateGame(FGame *g)
{
    QSqlQuery q;
    q.prepare("UPDATE games SET gameName = :gName, gameDirectory = :gDir, relExecutablePath = :exec, gameLauncher = :gLauncher, savegameDir = :gSavegameDir WHERE id = :gID");
    q.bindValue(":gName", g->getName());
    q.bindValue(":gDir", g->getPath());
    q.bindValue(":exec", g->getExe());
    if(g->savegameSyncEndabled())
        q.bindValue(":gSavegameDir", g->getSavegameDir().absolutePath());
    else
        q.bindValue(":gLauncher", QVariant(QVariant::String));

    if(g->doesUseLauncher())
        q.bindValue(":gLauncher", g->getLauncher().getDbId());
    else
        q.bindValue(":gLauncher", QVariant(QVariant::String));

    q.bindValue(":gID", g->dbId);
    return q.exec();
}

int FDB::getGameCount()
{
    QSqlQuery countQuery;
    countQuery.exec("SELECT count() FROM games");
    if(!countQuery.next())
    {
        return 0;
    }
    qDebug("Games found: "+countQuery.value(0).toString().toLatin1());
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
    prefQuery.exec();
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
    return prefQuery.exec();
}

bool FDB::updateTextPref(QString pref, QString value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET text = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);
    bool res = prefQuery.exec();


    if(prefQuery.numRowsAffected() == 0) {
        qDebug() << "Added Text-Pref:" << pref;
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
    return delQuery.exec();
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
    prefQuery.exec();
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
    bool res = prefQuery.exec();
    return res;
}

bool FDB::updateIntPref(QString pref, int value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET number = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);

    bool res = prefQuery.exec();

    if(prefQuery.numRowsAffected() == 0) {
        qDebug() << "Added Int-Pref:" << pref;
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
            qDebug() << "added Pref: " << pref;

    }

    return val;
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
    return q.exec();
}


bool FDB::getBoolPref(QString pref)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("SELECT (number) FROM prefs WHERE key = :key AND valuetype = 3");
    prefQuery.bindValue(":key", pref);
    prefQuery.exec();

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
    bool res = prefQuery.exec();
    return res;
}

bool FDB::updateBoolPref(QString pref, bool value)
{
    QSqlQuery prefQuery;
    prefQuery.prepare("UPDATE prefs SET number = :value WHERE key = :key");
    prefQuery.bindValue(":value", value);
    prefQuery.bindValue(":key", pref);

    bool res = prefQuery.exec();

    if(prefQuery.numRowsAffected() == 0) {
        qDebug() << "Added Int-Pref:" << pref;
        return addBoolPref(pref, value);
    } else {
        return res;
    }
}


bool FDB::updateWatchedFolders(QList<FWatchedFolder> data)
{
    QSqlQuery updateQuery;
    updateQuery.prepare("DELETE FROM watchedFolders");
    updateQuery.exec();

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO watchedFolders (path, launcherID, forLauncher) VALUES(:folder, :launcherID, :forLauncher)");

    for(FWatchedFolder dir : data) {
        insertQuery.bindValue(":folder", dir.getDirectory().absolutePath());
        insertQuery.bindValue(":launcherID", dir.getLauncherID());
        insertQuery.bindValue(":forLauncher", dir.forLauncher);
        insertQuery.exec();
        qDebug("Add Lib: " + dir.getDirectory().absolutePath().toLatin1());
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

bool FDB::runQuery(QSqlQuery q)
{
    //return q.exec();
    qDebug() << "RUN!";
    return false;
}

bool FDB::gameExists(FGame game)
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM games WHERE relExecutablePath = :Exe");
    query.bindValue(":Exe", game.getExe());
    query.exec();
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
    query.exec();
    query.next();
    return query.value(0).toInt()>0;
}


int FDB::addLauncher(FLauncher launcher)
{
    QSqlQuery query;
    query.prepare("INSERT INTO launchers(launcherName, launcherPath, launcherArgs, suffix) VALUES (:name, :path, :args, :suffix)");
    query.bindValue(":name", launcher.getName());
    query.bindValue(":path", launcher.getPath());
    query.bindValue(":args", launcher.getArgs());
    query.bindValue(":suffix", launcher.getFileEndings());
    return query.exec();
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
    query.exec();
    qDebug() << "Getting launcher" << dbId;
    if(!query.next())
    {
        qDebug() << "Didn't find the launcher..";
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
    query.exec();
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
