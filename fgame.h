#ifndef QTCORE
#include <QtCore>
#endif


#ifndef FGAME_H
#define FGAME_H



enum FGameType {unknown, Executable, Steam, Origin, Uplay, Galaxy};
enum FGameSizeConstrain {FHeight, FWidth};
enum FGameArt { FArtBox, FArtClearart, FArtBanner, FArtFanart};
#include "flauncher.h"


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
    QString getCommand();
    FLauncher getLauncher();

    void setName(QString val);
    void setExe(QString val);
    void setPath(QString val);
    void setArgs(QStringList val);
    void setType(FGameType val);
    void setCommand(QString val);
    void setLauncher(FLauncher launcher);
    void disableLauncher();
    bool doesUseLauncher();
    bool execute();

    int dbId;

    //Artwork-Stuff
    QString getBoxart(bool fromCache = false, int size=0, FGameSizeConstrain fsc = FWidth);
    QString getClearart(bool fromCache = false, int size=0, FGameSizeConstrain fsc = FWidth);
    QString getFanart(bool fromCache = false, int size=0, FGameSizeConstrain fsc = FWidth);
    QString getBanner(bool fromCache = false, int size=0, FGameSizeConstrain fsc = FWidth);

    QString getArt(FGameArt imgType, bool fromCache = false, int size = 0, FGameSizeConstrain fsc = FWidth);
    static QString FGameArtToStr(FGameArt imgType);
    static QString FGameTypeToStr(FGameType type);
    static QString getCacheDir();
protected:
    QString gameName;
    FGameType gameType;
    QString gamePath;
    QString gameExe;
    QStringList gameArgs;
    QString gameCommand;
    FLauncher launcher;
    bool launcherEnabled;
private:
    QString cachedImage(int size, FGameSizeConstrain fsc, FGameArt imgType);
    QStringList createStringListFromArguments(QString args);
};

#endif // FGAME_H
