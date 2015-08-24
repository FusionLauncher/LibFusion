#ifndef FLAUNCHER_H
#define FLAUNCHER_H

#include <QObject>


#include "libfusion_global.h"
class LIBFUSIONSHARED_EXPORT  FLauncher
{
public:
    explicit FLauncher();
    FLauncher(QString name, QString path, QString args);
    QString getName();
    QString getPath();
    QString getArgs();
    QString getFileEndings();
    int getDbId();

    void setName(QString newName);
    void setPath(QString newPath);
    void setArgs(QString newArgs);
    void setDbId(int newId);
    void setFileEndings(QString suffixes);

signals:

public slots:

protected:
    QString name;
    QString FileEndings;
    QString path;
    QString args;
    int dbId;
};

#endif // FLAUNCHER_H
