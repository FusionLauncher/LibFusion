#include "flogger.h"
#include <QFile>
#include <QTextStream>
#include "libfusion.h"


void FLogger::append(QString fileName, QString message)
{
    QDir d(LibFusion::getWorkingDir().absolutePath() + QDir::separator() + "Logs");
    if(!d.exists())
        d.mkpath(".");

    QString date = "[" + QDateTime::currentDateTime().toString(Qt::ISODate) + "] ";

    QFile file(d.absolutePath() + QDir::separator() + fileName);
    if (file.open(QIODevice::ReadOnly|QIODevice::Append)) {
        QTextStream stream(&file);
        stream << date << message << endl;
        file.close();
    }

}

void FLogger::clear(QString fileName)
{
    QFile log(LibFusion::getWorkingDir().absolutePath() + QDir::separator() + "Logs" + QDir::separator() + fileName);
    if(log.exists())
        log.remove();
}

