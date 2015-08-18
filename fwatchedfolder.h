#ifndef FWATCHEDFOLDER_H
#define FWATCHEDFOLDER_H


#include "flauncher.h"
#include "libfusion_global.h"

#include <QDir>

class LIBFUSIONSHARED_EXPORT FWatchedFolder
{
public:
    FWatchedFolder();

    QDir getDirectory() const;
    void setDirectory(const QDir &value);
    int getDbID() const;
    void setDbID(int value);

    bool forLauncher;

    int getLauncherID() const;
    void setLauncherID(int value);

private:
    QDir Directory;
    int dbID;
    int LauncherID;
};

#endif // FWATCHEDFOLDER_H
