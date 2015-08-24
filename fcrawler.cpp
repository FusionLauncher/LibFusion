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

    QList<FWatchedFolder> folder =db.getWatchedFoldersList();
    for(int i=0;i<folder.length();++i) {
        FWatchedFolder f = folder[i];

        if(f.forLauncher) {
              scanforLauncher(f);

        } else {
            FGameType folderType = getType(folder.at(i));
            if(folderType==FGameType::Steam) {
                getSteamGames(f);
            } else if (folderType==FGameType::Galaxy) {
                getGalaxyGames(f);
            }
        }
    }



    //Origin only on Windows
    #if OS == Windows
        getOriginGames();
    #endif

    db.endTransaction();
}

void FCrawler::scanforLauncher(FWatchedFolder folder) {
    QStringList fileEndings = db.getLauncher(folder.getLauncherID()).getFileEndings().split(",", QString::SkipEmptyParts);

    for(int i=0;i<fileEndings.length();++i)
        fileEndings[i] = "*." + fileEndings[i];

    QDir target =  folder.getDirectory();
    target.setNameFilters(fileEndings);
    QStringList launcherFiles = target.entryList();
    for(QString foundGame : launcherFiles) {
        qDebug() << "Found " << foundGame << " in " << target.absolutePath();
        FGame g;
        g.setName(foundGame.left(foundGame.length()-4));
        g.setExe(foundGame);
        g.setLauncher(db.getLauncher(folder.getLauncherID()));
        g.setType(ROM);
        g.setPath(target.absolutePath());
        if(!db.gameExists(g)){
           if(!db.addGame(g))
               qDebug() << "Error on insert Game "<< g.getName();;
        }
        else
            qDebug() << "Game exists: " << g.getName();
    }


}

FGameType FCrawler::getType(FWatchedFolder folder) {

    //TODO: Proper Type-Check
    folder.getDirectory().setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.getDirectory().entryList();

    if(steamFiles.length()>0)
        return FGameType::Steam;

    qDebug() << "Check if Galaxy-Folder";

    QDir f = folder.getDirectory().absolutePath() + QDir::separator() + "!Downloads";
    if(f.exists())
      return FGameType::Galaxy;

    return FGameType::unknown;
}

void FCrawler::getGalaxyGames(FWatchedFolder folder) {
    QStringList subfolders = folder.getDirectory().entryList();

       qDebug() << "Scanning Galaxy-Dir";

    for(int j=0;j<subfolders.length();++j) {
        if(subfolders[j]=="!Downloads")
            continue;

        //Get Info-File
        QStringList filters;
        filters << "goggame-*.info";

        QDir dir(folder.getDirectory().absolutePath()  + QDir::separator() + subfolders[j]);
        QStringList InfoFile = dir.entryList(filters);
        if(InfoFile.length()==1) {
            QFile oLogFile;
            oLogFile.setFileName(folder.getDirectory().absolutePath() + QDir::separator() + subfolders[j] + QDir::separator() + InfoFile[0]);
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
                        g.setExe(val);
                        pathFound = true;
                   //     qDebug() << "Path: " << val;
                    }
                    else if(line.contains("\"workingDir\"") && !workingdirFound){
                        val = line.mid(28, line.lastIndexOf("\"")-28);
                        g.setPath(folder.getDirectory().absolutePath() + QDir::separator() + subfolders[j] + QDir::separator() + val);
                        workingdirFound = true;
                   //     qDebug() << "workingDir: " << val;
                    }
                    else if(line.contains("\"arguments\"") && !argumentFound){
                        val  =line.mid(28, line.lastIndexOf("\"")-28);
                        argumentFound = true;
                    //    qDebug() << "arguments: " << val;
                    }

                    if(pathFound && nameFound && argumentFound && workingdirFound)
                        break;
            }

            if(pathFound && nameFound && workingdirFound) //This should be enough
            {
                if(!db.gameExists(g))
                {
                   if(!db.addGame(g))
                   {
                       qDebug() << "Error on insert Game!";
                   }
                }
                else
                {
                    qDebug() << "Game exists: " << g.getName();
                }
            }
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



void FCrawler::getSteamGames(FWatchedFolder folder) {
    QDir SteamAppsDir = folder.getDirectory();
    SteamAppsDir.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = SteamAppsDir.entryList();
    for(QString file : steamFiles) {

        //Get File-Content
        QFile f;
        f.setFileName(SteamAppsDir.absoluteFilePath(file));
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
