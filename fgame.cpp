#include "fgame.h"

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
