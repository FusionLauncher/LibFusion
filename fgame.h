#ifndef QTCORE
#include <QtCore>
#endif

#ifndef FGAME_H
#define FGAME_H

enum FGameType {Executable, Steam, Origin, Uplay, unknown};

class FGame
{
public:
    FGame (QString gName, FGameType gType, QString gDir, QString exePath, QStringList args);
    FGame();
    QString getName();
    QString getExe();
    QString getPath();
    QStringList getArgs();
<<<<<<< HEAD
    FGameType getType();
=======
>>>>>>> upstream/master
    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QStringList val);
<<<<<<< HEAD
    void setType(FGameType val);
=======
>>>>>>> upstream/master
private:
    QString gameName;
    FGameType gameType;
    QString gamePath;
<<<<<<< HEAD
    QString gameExe;
=======
>>>>>>> upstream/master
    QStringList gameArgs;
};

#endif // FGAME_H
