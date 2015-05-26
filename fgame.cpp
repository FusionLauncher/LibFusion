#include "fgame.h"
#include <QProcess>
#include <QDesktopServices>


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
        process->start(gamePath+'/'+gameExe, gameArgs);
    }
    return true;
}
