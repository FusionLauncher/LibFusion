#ifndef FLAUNCHER_H
#define FLAUNCHER_H

#include <QObject>

class FLauncher
{
public:
    explicit FLauncher();
    FLauncher(QString name, QString path, QString args);
    QString getName();
    QString getPath();
    QString getArgs();
    int getDbId();
    void setName(QString newName);
    void setPath(QString newPath);
    void setArgs(QString newArgs);
    void setDbId(int newId);

signals:

public slots:

protected:
    QString name;
    QString path;
    QString args;
    int dbId;
};

#endif // FLAUNCHER_H
