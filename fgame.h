#ifndef FGAME_H
#define FGAME_H

#include <QtCore>

class FGame
{
public:
    FGame();
    QString getName();
    QString getExe();
    QString getPath();
    QString getArgs();
    QString getAbsPath();
    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QString val);
private:
    QString gameName;
    QString gameExe;
    QString gamePath;
    QString gameArgs;
};

#endif // FGAME_H
