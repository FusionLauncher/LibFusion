#include "fgame.h"
#include <QProcess>
#include <QDesktopServices>
#include <QPixmap>
#include <QStandardPaths>


FGame::FGame (QString gName, FGameType gType, QString gDir, QString exePath, QStringList args) {
    this->gameName = gName;
    this->gameType = gType;
    this->gamePath = gDir;
    this->gameExe = exePath;
    this->gameArgs = args;
}

FGame::FGame()
{
}


QString FGame::getName() {
    return this->gameName;
}

QPixmap* FGame::getBoxart() {
    if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.png"))
        Boxart = new QPixmap(getArtworkDir()+ QDir::separator() + "boxart.png");
    else if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.jpg"))
        Boxart = new QPixmap(getArtworkDir()+ QDir::separator() + "boxart.jpg");
    else
        Boxart = new QPixmap(":/gfx/FusionLogo.png");


    return Boxart;
}

QString FGame::getExe()
{
    return this->gameExe;
}

FGameType FGame::getType() {
    return this->gameType;
}

void FGame::setType(FGameType val) {
    this->gameType = val;
}

QString FGame::getPath()
{
    return this->gamePath;
}

QString FGame::getArtworkDir()
{
    QDir path(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory));
    return QDir::cleanPath(path.absolutePath() + QDir::separator() + QString::number(dbId) + QDir::separator() + "Artwork");
}

QStringList FGame::getArgs()
{
    return this->gameArgs;
}

void FGame::setName(QString val)
{
    this->gameName = val;
}

void FGame::setExe(QString val)
{
    this->gameExe = val;
}

void FGame::setPath(QString val)
{
    this->gamePath = val;
}

void FGame::setArgs(QStringList val)
{
    this->gameArgs = val;
}

bool FGame::execute()
{

   if(gameType == FGameType::Steam) {
         return QDesktopServices::openUrl ( "steam://rungameid/" + gameExe );
   } else if (gameType == FGameType::Origin) {
        return QDesktopServices::openUrl ( "origin://launchgame/" + gameExe );
   } else {
        if(gameExe.isEmpty() || gamePath.isEmpty() || !QFile(gamePath+'/'+gameExe).exists())
        {
            return false;
        }

        QProcess *process = new QProcess();
        process->setWorkingDirectory(gamePath);
        process->start(gamePath+'/'+gameExe, gameArgs);
    }
    return true;
}
