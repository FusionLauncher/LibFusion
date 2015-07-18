#include "fgame.h"
#include <QProcess>
#include <QPixmap>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QStandardPaths>
#endif


FGame::FGame (QString gName, FGameType gType, QString gDir, QString exePath, QStringList args) {
    this->gameName = gName;
    this->gameType = gType;
    this->gamePath = gDir;
    this->gameExe = exePath;
    this->gameArgs = args;
}

FGame::FGame()
{
}


QString FGame::getName() {
    return this->gameName;
}

QString FGame::getBoxart(bool fromCache, int size, FGameSizeConstrain fsc) {
        if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.png"))
            return (getArtworkDir()+ "/boxart.png");
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.jpg"))
            return (getArtworkDir()+ "/boxart.jpg");
        else
            return (":/gfx/FusionLogo.png");
}

QString FGame::getClearart(bool fromCache, int size, FGameSizeConstrain fsc ) {
        QString ca = "";

        if(QFile::exists(getArtworkDir()+ QDir::separator() + "clearlogo.png"))
            ca = getArtworkDir()+ "/clearlogo.png";
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "clearlogo.jpg"))
             ca =  getArtworkDir()+"/clearlogo.jpg";


        return ca;

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
            qDebug() << "Resized " << cached;
            return cached;
      }
}

QString FGame::getFanart(bool fromCache, int size, FGameSizeConstrain fsc) {

    QString ca = "";

    if(QFile::exists(getArtworkDir()+ QDir::separator() + "fanart.png"))
        ca = getArtworkDir()+ "/fanart.png";
    else if(QFile::exists(getArtworkDir()+ QDir::separator() + "fanart.jpg"))
         ca =  getArtworkDir()+"/fanart.jpg";

    return ca;
}

QString FGame::getBanner(bool fromCache, int size, FGameSizeConstrain fsc)
{

        QString ca = "";

        if(QFile::exists(getArtworkDir()+ QDir::separator() + "banner.png"))
            ca = getArtworkDir()+ "/banner.png";
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "banner.jpg"))
             ca =  getArtworkDir()+"/banner.jpg";
        return ca;

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
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        QDir path(QCoreApplication::applicationDirPath());
    #elif (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        QDir path(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory));
    #else
        QDir path(QStandardPaths::locate(QStandardPaths::DataLocation, QString(), QStandardPaths::LocateDirectory));
    #endif

 //   qDebug(path.absolutePath().toLatin1());
    return QDir::cleanPath(path.absolutePath() + QDir::separator() + QString::number(dbId) + QDir::separator() + "Artwork");
}


QString FGame::getCacheDir()
{
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        QDir path(QCoreApplication::applicationDirPath());
    #elif (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        QDir path(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory));
    #else
        QDir path(QStandardPaths::locate(QStandardPaths::DataLocation, QString(), QStandardPaths::LocateDirectory));
    #endif

 //   qDebug(path.absolutePath().toLatin1());
    QString dir(QDir::cleanPath(path.absolutePath() + QDir::separator() + "artCache"));

    if(!(QDir(dir)).exists())
        (QDir(dir)).mkdir(dir);

    return dir;
}

QStringList FGame::getArgs()
{
    return this->gameArgs;
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

bool FGame::execute()
{

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
   } else {
        if(gameExe.isEmpty() || gamePath.isEmpty() || !QFile(gamePath+'/'+gameExe).exists())
        {
            return false;
        }

        QProcess *process = new QProcess();
        process->setWorkingDirectory(gamePath);
        process->start(gamePath+'/'+gameExe, gameArgs);
    }
    return true;
}
