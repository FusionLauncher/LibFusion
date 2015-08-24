#include "flauncher.h"

FLauncher::FLauncher()
{
    dbId = -1;
}

FLauncher::FLauncher(QString name, QString path, QString args)
{
    this->name = name, this->path = path, this->args = args;
}

QString FLauncher::getName()
{
    return name;
}

void FLauncher::setName(QString newName)
{
    name = newName;
}

QString FLauncher::getPath()
{
    return path;
}

void FLauncher::setPath(QString newPath)
{
    path = newPath;
}

QString FLauncher::getArgs()
{
    return args;
}

QString FLauncher::getFileEndings()
{
    return FileEndings;
}

void FLauncher::setArgs(QString newArgs)
{
    args = newArgs;
}

int FLauncher::getDbId()
{
    return dbId;
}

void FLauncher::setDbId(int newId)
{
    dbId = newId;
}

void FLauncher::setFileEndings(QString suffixes)
{
    FileEndings = suffixes;
}

