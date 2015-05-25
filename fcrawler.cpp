#include "fcrawler.h"

FCrawler::FCrawler()
{

}

FCrawler::~FCrawler()
{

}

void FCrawler::scanAllFolders()
{
    QList<QDir> folder =db.getWatchedFoldersList();
    for(int i=0;i<folder.length();++i) {
        scanFolder(folder.at(i));
    }
}



void FCrawler::scanFolder(QDir folder) {
    FGameType folderType = getType(folder);
    if(folderType==FGameType::Steam) {


    }
}



FGameType FCrawler::getType(QDir folder) {
    //TODO: Proper Type-Check

    folder.setNameFilters(QStringList()<<"*.acf");
    QStringList steamFiles = folder.entryList();
    if(steamFiles.length()>0)
        return FGameType::Steam;

    return FGameType::unknown;
}
