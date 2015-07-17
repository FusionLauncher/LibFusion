#include "fcuworker.h"

FCUWorker::FCUWorker()
{

}

//Downloads current linux client.
void FCUWorker::downloadClient(QString url, QString dest)
{

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgress(qint64,qint64)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(linuxFinished(QNetworkReply*)));

    /*qDebug() << "Attempting to download linux client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinishedLinux(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(clientLinuxUrl)));*/
}

void FCUWorker::finishedDL(QNetworkReply *reply)
{
    reply->deleteLater();

    QByteArray ba = reply->readAll();
    QFile file(clientLinuxDirectory);

    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out << ba;
}

void FCUWorker::updateDownloadProgress(qint64 current, qint64 total)
{

    this->totalProgress = total;
    this->currentProgress = current;
}

qint64 FCUWorker::getTotalProgress()
{

    return this->totalProgress;
}

qint64 FCUWorker::getCurrentProgress()
{

    return this->currentProgress;
}
