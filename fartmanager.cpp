#include "fartmanager.h"
#include "ffiledownloader.h"



FArtManager::FArtManager()
{
    m_manager = new QNetworkAccessManager();
    FArtManager *receiver = this;
    QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)), receiver, SLOT(dataReady(QNetworkReply*)));
}

void FArtManager::getGameData(FGame *g, QString platform = "pc") {
    game = g;
    QString url = "http://thegamesdb.net/api/GetGame.php?platform="+platform+"&name=" + game->getName();
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void FArtManager::dataReady(QNetworkReply *pReply)
{
    if(pReply->error() != QNetworkReply::NoError) {
        qDebug() << "QNetworkReply-Error: " << pReply->error();
        return;
    }

   QByteArray data=pReply->readAll();
   if(data.size() == 0) {
       qDebug() << "No data from ";
       return;
   }

   QString webData(data);


   QXmlStreamReader xml(data);
      while(!xml.atEnd()) {
            xml.readNext();
            if(xml.isStartElement()) {
                QString name = xml.name().toString();

                if(name=="GameTitle"||name=="id")
                    qDebug() << "element name: " << name  << ", text: " << xml.readElementText();
                else if(name=="baseImgUrl")
                    baseImgUrl = xml.readElementText();
                else if(name=="clearlogo")
                    clearartURL = xml.readElementText();
                else if(name=="boxart")
                    boxartURL = xml.readElementText();
              //  else
                //    xml.skipCurrentElement();
            }
      }

      QDir artworkpath(game->getArtworkDir());

      if(clearartURL != NULL) {
        QUrl clearLogo(baseImgUrl + clearartURL);
        QString clearartTarget = QDir::cleanPath(artworkpath.absolutePath() + QDir::separator() + "clearlogo.png");
        FFileDownloader *clearartDownloader = new FFileDownloader(clearLogo, clearartTarget);
      }
      if(boxartURL != NULL){
          QUrl clearLogo(baseImgUrl + boxartURL);
          QString clearartTarget = QDir::cleanPath(artworkpath.absolutePath() + QDir::separator() + "boxart.png");
          FFileDownloader *clearartDownloader = new FFileDownloader(clearLogo, clearartTarget);
        }
}


void FArtManager::processData()
{
    qDebug() << "Proc Data: ";
    xml.readNext();

        return;
    while (xml.readNextStartElement()) {
        qDebug() << xml.name() << ":" << xml.text();
    }

}

