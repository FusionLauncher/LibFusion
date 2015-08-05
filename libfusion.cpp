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
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    #else
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    #endif
}

bool LibFusion::makeSureWorkingDirExists()
{
    if(!getWorkingDir().exists())
    {
        return getWorkingDir().mkpath(".");
    }
    return true;
}
