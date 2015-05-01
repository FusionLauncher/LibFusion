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
    QString getType();
    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QString val);
    void setType(QString val);
private:
    QString gameName;
    QString gameExe;
    QString gamePath;
    QString gameArgs;
    QString gameType;
};

#endif // FGAME_H
