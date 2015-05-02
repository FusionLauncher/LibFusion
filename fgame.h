#ifndef QTCORE
#include <QtCore>
#endif

#ifndef FGAME_H
#define FGAME_H

class FGame
{
public:
    FGame();
    QString getName();
    QString getExe();
    QString getPath();
    QString getArgs();
    QString getAbsPath();
    FGameType getType();
    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QString val);
    void setType(FGameType val);
private:
    QString gameName;
    QString gameExe;
    QString gamePath;
    QString gameArgs;
    FGameType gameType;
};

#endif // FGAME_H
