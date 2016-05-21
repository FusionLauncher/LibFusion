#include <QProcess>
#include <QPixmap>
#include <QDesktopServices>

#include "fgame.h"
#include "libfusion.h"
#include "flogging.h"


FGame::FGame (QString gName, FGameType gType, QString gDir, QString exePath, QStringList args) {
    this->gameName = gName;
    this->gameType = gType;
    this->gamePath = gDir;
    this->gameExe = exePath;
    this->gameArgs = args;
    launcherEnabled = false;
    syncEnabled = false;
}

FGame::FGame()
{
    launcherEnabled = false;
    syncEnabled = false;
}


QString FGame::getName() {
    return this->gameName;
}


QString FGame::FGameArtToStr(FGameArt imgType ) {
    switch (imgType) {
    case FArtBanner:
        return "Banner";
        break;
    case FArtBox:
        return "Box";
        break;
    case FArtClearart:
        return "Clearart";
        break;
    case FArtFanart:
        return "Fanart";
        break;
    default:
        return "";
        break;
    }
}

QString FGame::FGameTypeToStr(FGameType type)
{
    switch(type) {

    case unknown:
        return "Unknown";
        break;
    case Executable:
        return "Executable";
        break;
    case Steam:
        return "Steam";
        break;
    case Origin:
        return "Origin";
        break;
    case Uplay:
        return "Uplay";
        break;
    case Galaxy:
        return "GOG Galaxy";
        break;
    default:
        return "Unknown";
        break;
    }
}

QString FGame::getArt(FGameArt imgType, bool fromCache, int size, FGameSizeConstrain fsc) {

    if(fromCache) {
        return cachedImage(size, fsc, imgType);
    }
    else
    {
        QString ca = "";

        if(QFile::exists(getArtworkDir()+ QDir::separator() + FGameArtToStr(imgType) + ".png"))
            ca = getArtworkDir()+ QDir::separator() + FGameArtToStr(imgType) + ".png";
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + FGameArtToStr(imgType) + ".jpg"))
            ca =  getArtworkDir()+ QDir::separator() + FGameArtToStr(imgType) + ".jpg";

        QFile f(ca);
        if(ca != "")
            return f.fileName();
        else
            return "";
    }
}

QString FGame::cachedImage(int size, FGameSizeConstrain fsc, FGameArt imgType ) {
    QString fscI;

    if(fsc == FWidth)
        fscI = "w_";
    else
        fscI = "h_";

    QString fName(QString::number(dbId) + "_" + FGameArtToStr(imgType) + "_" + fscI + QString::number(size) + ".png");
    QString dir = getCacheDir();
    QString cached = dir + "/" + fName;
    if(QFile(cached).exists())
        return cached;
    else {
        QString unCached = getArt(imgType);
        if(unCached == "")
            return unCached;


        QPixmap p(unCached);
        if(fsc == FWidth)
            p= p.scaledToWidth(size, Qt::SmoothTransformation);
        else
            p = p.scaledToHeight(size, Qt::SmoothTransformation);

        QFile file(cached);
        file.open(QIODevice::WriteOnly);
        p.save(&file, "png", 90);
        file.close();
        qCDebug(fLibGame) << "Resized " << cached;
        return cached;
    }
}


QString FGame::getExe()
{
    return this->gameExe;
}

FGameType FGame::getType() {
    return this->gameType;
}

void FGame::setType(FGameType val) {
    this->gameType = val;
}

QString FGame::getPath()
{
    return this->gamePath;
}

QString FGame::getArtworkDir()
{
    QDir path = LibFusion::getWorkingDir();
    return QDir::cleanPath(path.absolutePath() + QDir::separator() + QString::number(dbId) + QDir::separator() + "Artwork");
}


QString FGame::getCacheDir()
{

    QDir path = LibFusion::getWorkingDir();
    QString dir(QDir::cleanPath(path.absolutePath() + QDir::separator() + "artCache"));

    if(!(QDir(dir)).exists())
        (QDir(dir)).mkdir(dir);

    return dir;
}
QDir FGame::getSavegameDir() const
{
    return savegameDir;
}

void FGame::setSavegameDir(const QDir &value)
{
    savegameDir = value;
    syncEnabled = true;
}

void FGame::setSavegameDir(const QString &value)
{
    if(value!="") {
        savegameDir = QDir(value);
        syncEnabled = true;
    } else
        syncEnabled = false;
}

bool FGame::savegameSyncEndabled()
{
    return syncEnabled;
}

bool FGame::syncData()
{
    /*
   // if(!syncEnabled)
     //   return true;

    try {
        FFileSync f;
      //  f.sync(savegameDir,5);
    } catch (FException& e) {
        qWarning() << e.what() << e.Message;
        }
*/
    return true;
}

QDateTime FGame::getGameLastPlayed() const
{
    return gameLastPlayed;
}

void FGame::setGameLastPlayed(const QDateTime &value)
{
    gameLastPlayed = value;
}


QStringList FGame::getArgs()
{
    return this->gameArgs;
}

QString FGame::getCommand()
{
    return this->gameCommand;
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

void FGame::setCommand(QString val)
{
    this->gameCommand = val;
}

void FGame::setLauncher(FLauncher launcher)
{
    this->launcher = launcher;
    launcherEnabled = true;
}

void FGame::disableLauncher()
{
    launcherEnabled = false;
}

bool FGame::doesUseLauncher()
{
    return launcherEnabled;
}

FLauncher FGame::getLauncher()
{
    return this->launcher;
}

QStringList FGame::createStringListFromArguments(QString args)
{
    QStringList list = args.split(" ");
    return list;
}

bool FGame::execute()
{
    syncData();

    qCDebug(fLibGame) << "Game type: " << gameType;
    if(gameType == FGameType::Steam) {
#ifdef _WIN32
        QString cmd("start steam://rungameid/" + gameExe);
        system(cmd.toStdString().c_str());
#else
        QStringList args;
        args << "-applaunch" << gameExe;
        QProcess::startDetached("steam", args);
#endif

    } else if (gameType == FGameType::Origin) {
        QString cmd("start origin://launchgame/" + gameExe);
        system(cmd.toStdString().c_str());
    }
    else if (gameType == FGameType::Executable || gameType == FGameType::Galaxy || gameType == FGameType::ROM  )
    {
        if(gameExe.isEmpty() || gamePath.isEmpty() || !QFile(gamePath+'/'+gameExe).exists())
        {
            qCDebug(fLibGame) << "gameExe.isEmpty() || gamePath.isEmpty() || !QFile(gamePath+'/'+gameExe).exists()";
            return false;
        }
        if(launcherEnabled)
        {
            qCDebug(fLibGame) << "Launcher:" << launcher.getName() << ", path:" << launcher.getPath() << ", args:" << launcher.getArgs() << ", id:" << launcher.getDbId();
            QProcess *process = new QProcess();
            process->setWorkingDirectory(gamePath);
            if(!launcher.getArgs().isEmpty())
            {
                qCDebug(fLibGame) << "Found some arguments!";
                QStringList argList = createStringListFromArguments(launcher.getArgs());
                qCDebug(fLibGame) << "Launcher ArgList: " << argList;
                QStringList::iterator i;
                for(i = argList.begin(); i != argList.end(); i++)
                {
                    i->replace("$GAMENAME", gameName);
                    i->replace("$GAMECOMMAND", gameCommand);
                    i->replace("$GAMEPATH", gamePath);
                    i->replace("$GAMEEXE", gameExe);
                }
                process->start(launcher.getPath(), argList);
            }
            else
            {
                qCDebug(fLibGame) << "Didn't find arguments.";
                process->start(launcher.getPath(), QStringList(gameExe));
            }
        }
        else
        {
            QProcess *process = new QProcess();
            process->setWorkingDirectory(gamePath);
            qCDebug(fLibGame) << "Command: " << gameCommand;
            if(!gameCommand.isEmpty())
            {
                QStringList newGameArgs = createStringListFromArguments(gameArgs.at(0));
                qCDebug(fLibGame) << "Found a command!";
                QStringList::iterator i;
                for(i = newGameArgs.begin(); i != newGameArgs.end(); i++)
                {
                    i->replace("$GAMENAME", gameName);
                    i->replace("$GAMECOMMAND", gameCommand);
                    i->replace("$GAMEPATH", gamePath);
                    i->replace("$GAMEEXE", gameExe);
                }
                process->start(gameCommand, newGameArgs);
            }
            else
            {
                qCDebug(fLibGame) << "Didn't find command, running executable";
                process->start(gamePath+'/'+gameExe, gameArgs);
            }
        }
    }
    return true;
}

