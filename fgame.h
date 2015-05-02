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
    QStringList getArgs();
    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QStringList val);
private:
    QString gameName;
    QString gameExe;
    QString gamePath;
    QStringList gameArgs;
};

#endif // FGAME_H
