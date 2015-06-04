
#ifndef QTCORE
#include <QtCore>
#endif

#ifndef FGAME_H
#define FGAME_H

enum FGameType {unknown, Executable, Steam, Origin, Uplay};

#include "libfusion_global.h"
class LIBFUSIONSHARED_EXPORT FGame
{
public:
    FGame (QString gName, FGameType gType, QString gDir, QString exePath, QStringList args);
    FGame();

    QString getName();
    QString getExe();
    QString getPath();
    QString getArtworkDir();
    QStringList getArgs();
    FGameType getType();

    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QStringList val);
    void setType(FGameType val);
    bool execute();

    int dbId;

    QString getBoxart();
    QString getClearart();
    QString getFanart();

private:
    QString gameName;
    FGameType gameType;
    QString gamePath;
    QString gameExe;
    QStringList gameArgs;
};

#endif // FGAME_H
