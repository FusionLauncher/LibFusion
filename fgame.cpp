#include "fgame.h"

FGame::FGame()
{
    this->gameName = "Unknown";
    this->gameExe = "Unknown";
    this->gamePath = "Unknown";
    this->gameArgs = "Unknown";
    this->gameType = "Unknown";
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

QString FGame::getArgs()
{
    return this->gameArgs;
}

QString FGame::getAbsPath()
{
    QString RawString = "%1/%2 %3";
    return RawString.arg(this->getPath(), this->getExe(), this->getArgs());
}

QString FGame::getType()
{
    return this->gameType;
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

void FGame::setArgs(QString val)
{
    this->gameArgs = val;
}

void FGame::setType(QString val)
{
    this->gameType = val;
}
