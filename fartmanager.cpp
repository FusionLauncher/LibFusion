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
    QString gName = g->getName().replace("™", "");
    QString url = "http://thegamesdb.net/api/GetGame.php?platform="+platform+"&exactname=" + gName;
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


    xml = new QXmlStreamReader(data);
    while(!xml->atEnd())
    {
        xml->readNext();

        if(xml->isStartElement())
        {
            QString name = xml->name().toString();
            if(name=="Game")
            {
                processGame();
            }
        }
    }

      //Only one Found, assume its the right one
      if(Games.length()==1)
      {
        QDir artworkpath(game->getArtworkDir());
        QString baseImgUrl = "http://thegamesdb.net/banners/";
        if(Games[0]->clearartURL != NULL)
        {
            emit startedDownload();
            QUrl clearLogo(baseImgUrl + Games[0]->clearartURL);
            QString clearartTarget = QDir::cleanPath(artworkpath.absolutePath() + QDir::separator() + "clearlogo.png");
            FFileDownloader *clearartDownloader = new FFileDownloader(clearLogo, clearartTarget);
            connect(clearartDownloader, SIGNAL(downloaded()), this, SLOT(on_downloadFinished()));
        }
        if(Games[0]->boxartURL != NULL)
        {
            emit startedDownload();
            QUrl clearLogo(baseImgUrl + Games[0]->boxartURL);
            QString clearartTarget = QDir::cleanPath(artworkpath.absolutePath() + QDir::separator() + "boxart.jpg");
            FFileDownloader *clearartDownloader = new FFileDownloader(clearLogo, clearartTarget);
            connect(clearartDownloader, SIGNAL(downloaded()), this, SLOT(on_downloadFinished()));
        }

        if(Games[0]->bannerURL != NULL)
        {
            emit startedDownload();
            QUrl clearLogo(baseImgUrl + Games[0]->bannerURL);
            QString clearartTarget = QDir::cleanPath(artworkpath.absolutePath() + QDir::separator() + "banner.png");
            FFileDownloader *clearartDownloader = new FFileDownloader(clearLogo, clearartTarget);
            connect(clearartDownloader, SIGNAL(downloaded()), this, SLOT(on_downloadFinished()));
        }
      } else if(Games.length()==0) {

          emit finishedDownload();
      }
}


void FArtManager::on_downloadFinished() {
    emit finishedDownload();
}

void FArtManager::processGame()
{    
    TheGameDBStorage *gameDBStore = new TheGameDBStorage();

    while(!xml->atEnd()) {
          xml->readNext();

          if(xml->isStartElement())
          {
              QString name = xml->name().toString();

              if(name=="GameTitle")
                gameDBStore->gameName = xml->readElementText();
              else if (name=="id")
                 gameDBStore->gameID = xml->readElementText();
              else if(name=="baseImgUrl")
                  gameDBStore->baseImgUrl = xml->readElementText();
              else if(name=="clearlogo")
                  gameDBStore->clearartURL = xml->readElementText();
              else if(name=="boxart")
                  gameDBStore->boxartURL = xml->readElementText();
              else if(name=="banner")
                  gameDBStore->bannerURL = xml->readElementText();
              else if (name=="Similar")
                  xml->skipCurrentElement();
          }
          else if(xml->isEndElement())
          {
              QString name = xml->name().toString();
              if(name=="Game") {
                   Games.append(gameDBStore);
                return;
              }
          }
    }
}

