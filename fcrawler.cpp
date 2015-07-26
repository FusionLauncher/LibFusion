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
        } else if (folderType==FGameType::Galaxy) {
            getGalaxyGames(folder.at(i));
        }
    }

    //Origin only on Windows
    #if OS == Windows
        getOriginGames();
    #endif

    db.endTransaction();
}


FGameType FCrawler::getType(QDir folder) {

    //TODO: Proper Type-Check
    folder.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.entryList();

    if(steamFiles.length()>0)
        return FGameType::Steam;

    folder = folder.absolutePath() + QDir::separator() + "!Downloads";
    if(folder.exists())
      return FGameType::Galaxy;

    return FGameType::unknown;
}

void FCrawler::getGalaxyGames(QDir folder) {
    QStringList subfolders = folder.entryList();

    for(int j=0;j<subfolders.length();++j) {
        if(subfolders[j]=="!Downloads")
            continue;

        //Get Info-File
        QStringList filters;
        filters << "goggame-*.info";

        QDir dir(folder.absolutePath()  + QDir::separator() + subfolders[j]);
        QStringList InfoFile = dir.entryList(filters);
        if(InfoFile.length()==1) {
            QFile oLogFile;
            oLogFile.setFileName(folder.absolutePath() + QDir::separator() + subfolders[j] + QDir::separator() + InfoFile[0]);
            oLogFile.open(QIODevice::ReadOnly|QIODevice::Text);
            QString fileContent = oLogFile.readAll();
            QStringList fileLines = fileContent.split("\n");
            oLogFile.close();

            FGame g;
            g.setType(FGameType::Galaxy);
            bool nameFound = false;
            bool pathFound = false;
            bool workingdirFound = false;
            bool argumentFound = false;

            for(int i=0;i<fileLines.length();++i){
                QString line = fileLines[i].replace(QString("\t"), QString("    "));
                    QString val;
                    if(line.contains("\"name\"") && !nameFound) {
                        val =line.mid(26, line.lastIndexOf("\"")-26);
                        g.setName(val);
                 //       qDebug() << "Name: " << val;
                        nameFound = true;
                    }
                    else if(line.contains("\"path\"") && !pathFound){
                        val = line.mid(27, line.lastIndexOf("\"")-27);
                        // eg: ScummVM\\schumm.exe
                        val = val.right(val.length() - val.lastIndexOf("\\")-1);
                        g.setExe(val);
                        pathFound = true;
                   //     qDebug() << "Path: " << val;
                    }
                    else if(line.contains("\"workingDir\"") && !workingdirFound){
                        val = line.mid(28, line.lastIndexOf("\"")-28);
                        g.setPath(folder.absolutePath() + QDir::separator() + subfolders[j] + QDir::separator() + val);
                        workingdirFound = true;
                   //     qDebug() << "workingDir: " << val;
                    }
                    else if(line.contains("\"arguments\"") && !argumentFound){
                        val  =line.mid(28, line.lastIndexOf("\"")-28);
                        argumentFound = true;
                        g.setArgs(QStringList(val.replace("\\\"", "")));
                    //    qDebug() << "arguments: " << val;
                    }

                    if(pathFound && nameFound && argumentFound && workingdirFound)
                        break;
            }

            if(pathFound && nameFound && workingdirFound) //This should be enough
                if(!db.gameExists(g)){
                   if(!db.addGame(g))
                       qDebug() << "Error on insert Game!";
                }
                else
                    qDebug() << "Game exists: " << g.getName();
        }
        else
            continue;
    }
}


void FCrawler::getOriginGames() {

    QString file = "C:\\ProgramData\\Origin\\Logs\\Client_Log.txt";
    QList<QString> doneIDs;

    QFile oLogFile;
    oLogFile.setFileName(file);

    if(!oLogFile.exists())
        return;

    oLogFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QString fileContent = oLogFile.readAll();
    QStringList fileLines = fileContent.split("\n");
    oLogFile.close();

    //Example-String:
    // 457    May 26 17:59:46.363 Event     Origin::Downloader::ContentProtocolPackage::InitializeVerified              7460      [Game Title: Mass Effect 2 Digital Art Books][ProductID:OXX-MASS:57404]CRC Calculation Complete.
    for(int i=0;i<fileLines.length();++i){
        QString line = fileLines[i];
        if(line.contains("Finished: ")) {

            int gamenameLength = line.indexOf(", ctid:", 135)-135;
            QString gameName = line.mid(135, gamenameLength);

            QString gameID = line.right(7);

            if(!doneIDs.contains(gameID)) {
                FGame g;
                g.setPath("-");
                g.setName(gameName);
                g.setType(FGameType::Origin);
                g.setExe(gameID);

                doneIDs.append(gameID);

                if(!db.gameExists(g)){
                   if(!db.addGame(g))
                       qDebug() << "Error on insert Game!";
                }
                else
                    qDebug() << "Game exists: " << g.getName();
            }
        }

    }
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
