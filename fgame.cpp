#include "fgame.h"
#include <QProcess>
#include <QDesktopServices>
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
    this->Boxart = NULL;
    this->Clearart = NULL;
    this->Banner = NULL;
}


QString FGame::getName() {
    return this->gameName;
}

QPixmap* FGame::getBoxart() {

    if(Boxart==NULL) {
        if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.png"))
            Boxart = new QPixmap(getArtworkDir()+ QDir::separator() + "boxart.png");
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "boxart.jpg"))
            Boxart = new QPixmap(getArtworkDir()+ QDir::separator() + "boxart.jpg");
        else
            Boxart = new QPixmap(":/gfx/FusionLogo.png");
    }

    return Boxart;
}

QString FGame::getClearart() {

    QString ca = "";
    if(Clearart==NULL) {
        if(QFile::exists(getArtworkDir()+ QDir::separator() + "clearlogo.png"))
            ca = getArtworkDir()+ "/clearlogo.png";
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "clearlogo.jpg"))
             ca =  getArtworkDir()+"/clearlogo.jpg";
    }

    return ca;
}

QString FGame::getFanart() {

    QString ca = "";
        if(QFile::exists(getArtworkDir()+ QDir::separator() + "fanart.png"))
            ca = getArtworkDir()+ "/fanart.png";
        else if(QFile::exists(getArtworkDir()+ QDir::separator() + "fanart.jpg"))
             ca =  getArtworkDir()+"/fanart.jpg";


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
        QDir path(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    #else
        QDir path(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory));
    #endif

 //   qDebug(path.absolutePath().toLatin1());
    return QDir::cleanPath(path.absolutePath() + QDir::separator() + QString::number(dbId) + QDir::separator() + "Artwork");
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
         return QDesktopServices::openUrl ( "steam://rungameid/" + gameExe );
   } else if (gameType == FGameType::Origin) {
        return QDesktopServices::openUrl ( "origin://launchgame/" + gameExe );
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
