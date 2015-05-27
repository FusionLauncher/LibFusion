#include "fcrawler.h"
#include "fgame.h"

#include <QVariantMap>

FCrawler::FCrawler()
{

}

FCrawler::~FCrawler()
{

}

void FCrawler::scanAllFolders()
{
    db.beginTransaction();

    QList<QDir> folder =db.getWatchedFoldersList();
    for(int i=0;i<folder.length();++i) {
        FGameType folderType = getType(folder.at(i));
        if(folderType==FGameType::Steam) {
            getSteamGames(folder.at(i));
        }
    }

    db.endTransaction();
}


FGameType FCrawler::getType(QDir folder) {

    //TODO: Proper Type-Check
    folder.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.entryList();

    if(steamFiles.length()>0)
        return FGameType::Steam;

    return FGameType::unknown;
}


QList<FGame> FCrawler::getOriginGames() {

    QString file = "C:\\ProgramData\\Origin\\Logs\\Client_Log.txt";
    QList<QString> doneIDs;
    QList<FGame> result;

    QFile oLogFile;
    oLogFile.setFileName(file);

    if(!oLogFile.exists())
        return result;

    oLogFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QString fileContent = oLogFile.readAll();
    QStringList fileLines = fileContent.split("\n");
    oLogFile.close();

    //Example-String:
    // 457    May 26 17:59:46.363 Event     Origin::Downloader::ContentProtocolPackage::InitializeVerified              7460      [Game Title: Mass Effect 2 Digital Art Books][ProductID:OXX-MASS:57404]CRC Calculation Complete.
    for(int i=0;i<fileLines.length();++i){
        QString line = fileLines[i];
        if(line.contains("][ProductID:")) {

            int gamenameLength = line.indexOf(']', 138)-138;
            QString gameName = line.mid(138, gamenameLength);

            int idStart = 138+gamenameLength+12;
            int idLength = line.indexOf(']', idStart)-idStart;
            QString gameID = line.mid(idStart, idLength);

            if(!doneIDs.contains(gameID)) {
                FGame g;
                g.setPath("-");
                g.setName(gameName);
                g.setType(FGameType::Origin);
                g.setExe(gameID);

                doneIDs.append(gameID);

                result.append(g);

            }
        }

    }

    return result;
}



void FCrawler::getSteamGames(QDir folder) {
    folder.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.entryList();
    for(QString file : steamFiles) {

        //Get File-Content
        QFile f;
        f.setFileName(folder.absoluteFilePath(file));
        f.open(QIODevice::ReadOnly|QIODevice::Text);
        QString fileContent = f.readAll();
        f.close();

        QStringList fileLines = fileContent.replace("\"", "").split("\n");

        //Create Game
        FGame g;
        g.setName(fileLines[4].split("\t")[3]);
        g.setType(FGameType::Steam);
        g.setExe(fileLines[2].split("\t")[3]);
        g.setPath("-");

        if(!db.gameExists(g)){
           if(!db.addGame(g))
               qDebug() << "Error on insert Game!";
        }
        else
            qDebug() << "Game exists: " << g.getName();
    }
}
