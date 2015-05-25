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
        scanFolder(folder.at(i));
    }

    db.endTransaction();
}



void FCrawler::scanFolder(QDir folder) {
    FGameType folderType = getType(folder);
    if(folderType==FGameType::Steam) {
        getSteamGames(folder);
    }
}



FGameType FCrawler::getType(QDir folder) {

    //TODO: Proper Type-Check
    folder.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.entryList();

    if(steamFiles.length()>0)
        return FGameType::Steam;

    return FGameType::unknown;
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
