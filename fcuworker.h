#ifndef FCUWORKER_H
#define FCUWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class FCUWorker : public QThread
{
public:
    FCUWorker();
    qint64 getCurrentProgress();
    qint64 getTotalProgress();

public slots:
    void finishedDL();
};

#endif // FCUWORKER_H
