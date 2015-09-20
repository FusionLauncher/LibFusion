#include "libfusion.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

LibFusion::LibFusion()
{

}

QDir LibFusion::getWorkingDir()
{
    QString p = QDir::currentPath();

    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        p = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    #else
        p = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    #endif

    int idx = p.lastIndexOf("/");
    if(idx==-1)
        idx = p.lastIndexOf("\\");

    p = p.left(idx+1);
    p += "FusionClient";
    return QDir(p);
}

bool LibFusion::makeSureWorkingDirExists()
{
    if(!getWorkingDir().exists())
    {
        return getWorkingDir().mkpath(".");
    }
    return true;
}
