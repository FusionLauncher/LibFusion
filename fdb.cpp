#include <QSqlDatabase>
#include <QSqlQuery>
#include <fgame.h>
#include <fdb.h>

FDB::FDB(QObject *parent)
{

}

bool FDB::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("fusion.db");
    bool ok = db.open();
    //ui->label->setText(ok ? "Connected!" : "Failed");
    if(!ok)
    {
        //show a popup window / alert about the error
        return false;
    }
    QSqlQuery query;
    QSqlQuery queryPreferences;
    QSqlQuery queryGames;

    query.exec("CREATE TABLE IF NOT EXISTS prefs(key TINYTEXT NOT NULL, valuetype TINYINT NOT NULL, number TINYINT NOT NULL, text VARCHAR(255) NOT NULL)");
    queryPreferences.exec("SELECT * FROM prefs;");
    while(queryPreferences.next())
    {
        //get values here
    }
    //(later) if clientToken doesnt exists, show login and run registerClient(), if no account, //run register()
    //(later) if lang is not set, set it to the default system language
    //Then get game list
    query.exec("CREATE TABLE IF NOT EXISTS games(id INTEGER PRIMARY KEY ASC, gameName TEXT NOT NULL, gameType TINYINT NOT NULL , gameDirectory TEXT NOT NULL, relExecutablePath TEXT NOT NULL)");
    //queryGames.exec("SELECT * FROM games");
    //TODO: add everything to list


    query.exec("CREATE TABLE IF NOT EXISTS watchedFolders ( `id` INTEGER PRIMARY KEY ASC, `path` VARCHAR(255) );");

    return true;
}

bool FDB::addGame(FGame game)
{
    QSqlQuery gameQuery;
    gameQuery.prepare("INSERT INTO games(gameName, gameType, gameDirectory, relExecutablePath) VALUES (:gameName, :gameType, :gameDirectory, :relExecutablePath)");
    gameQuery.bindValue(":gameName", game.getName());
    gameQuery.bindValue(":gameType", game.getType());
    gameQuery.bindValue(":gameDirectory", game.getPath());
    gameQuery.bindValue(":relExecutablePath", game.getExe());
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
    gameQuery.prepare("SELECT gameName, gameType, gameDirectory, relExecutablePath FROM games WHERE id = :id");
    gameQuery.bindValue(":id", id);
    gameQuery.exec();
    if(! gameQuery.next())
    {
        return NULL;
    }
    FGame *game = new FGame();
    game->setName(gameQuery.value(0).toString());
    //TODO: get the game type
    game->setPath(gameQuery.value(2).toString());
    game->setExe(gameQuery.value(3).toString());
    game->dbId = id;
    return game;
}

QList<FGame> FDB::getGameList()
{
    QList<FGame> gameList;
    QSqlQuery libraryQuery;
    FGame game;
    libraryQuery.exec("SELECT gameName, gameType, gameDirectory, relExecutablePath, id FROM games ORDER BY gameName ASC");
    while(libraryQuery.next())
    {
        qDebug("Getting game!");
        game.setName(libraryQuery.value(0).toString());
        //TODO: get the game type
        game.setPath(libraryQuery.value(2).toString());
        game.setExe(libraryQuery.value(3).toString());
        game.dbId = libraryQuery.value(4).toInt();
        game.setType((FGameType)libraryQuery.value(1).toInt());
        gameList.append(game);
    }
    return gameList;
}

void FDB::resetDatabase()
{
    QSqlQuery resetQuery;
    resetQuery.exec("DROP TABLE games"); //WOO DROPPING TABLES YAY
    resetQuery.exec("CREATE TABLE IF NOT EXISTS games(id INTEGER PRIMARY KEY ASC, gameName TEXT NOT NULL, gameType TINYINT NOT NULL , gameDirectory TEXT NOT NULL, relExecutablePath TEXT NOT NULL)");
    qDebug("Game database reset");
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

bool FDB::updateWatchedFolders(QList<QDir> data)
{
    QSqlQuery updateQuery;
    updateQuery.prepare("DELETE FROM watchedFolders");
    updateQuery.exec();

        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO watchedFolders (path) VALUES(:folder)");

    for(QDir dir : data) {
        insertQuery.bindValue(":folder", dir.absolutePath());
        insertQuery.exec();
        qDebug("Add Lib: " + dir.absolutePath().toLatin1());
    }

    return true;
}

QList<QDir> FDB::getWatchedFoldersList() {
    QList<QDir> result;
    QSqlQuery folderqQueue;
    folderqQueue.exec("SELECT path FROM watchedFolders ORDER BY path ASC");
    while(folderqQueue.next())
    {
        qDebug("Getting Folders!");
        result.append(QDir(folderqQueue.value(0).toString()));
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

bool FDB::gameExists(FGame game)
{
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM games WHERE gameName = :Name AND relExecutablePath = :Exe");
    query.bindValue(":Name", game.getName());
    query.bindValue(":Exe", game.getExe());
    query.exec();
    query.next();

    if(query.value(0).toInt()>0)
        return true;
    else
        return false;

}
