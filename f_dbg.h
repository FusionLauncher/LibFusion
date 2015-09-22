#ifndef F_DBG
#define F_DBG

#include "qdebug.h"
#include "flogger.h"

//Globally enable DBG-Outputs
#define DBG_PRINT_ENABLED

//FLogger::append("Main.log", "FusionCLient started");
//#define LOG_DBGS

//Enable Debug for Sections
#ifdef DBG_PRINT_ENABLED

    //Carlwer in fcrawler.cpp
   #define DBG_CRWL_EN
   #define DBG_CRWL2_EN

    //Database in fdb.cpp
   #define DBG_DB_EN

    //Database-Updates in fdbupdater.cpp
   #define DBG_DBU_EN

#endif


//Macros for Each Debug-Section below.
//Doesn't need to be edited after declaration
#ifdef DBG_CRWL_EN
    #ifdef LOG_DBGS
        #define DBG_CRWL(x) FLogger::append("Crawler.log", x)
    #else
        #define DBG_CRWL(x) qDebug() << x
    #endif
#else
    #define DBG_CRWL(x)
#endif



#ifdef DBG_CRWL2_EN
    #ifdef LOG_DBGS
        #define DBG_CRWL2(x) FLogger::append("Crawler.log", x)
    #else
        #define DBG_CRWL2(x) qDebug() << x
    #endif
#else
    #define DBG_CRWL2(x)
#endif



#ifdef DBG_DB_EN
    #ifdef LOG_DBGS
        #define DBG_DB(x) FLogger::append("DB.log", x)
    #else
        #define DBG_DB(x) qDebug() << x
    #endif
#else
    #define DBG_DB(x)
#endif

#ifdef DBG_DBU_EN
    #ifdef LOG_DBGS
        #define DBG_DBU(x) FLogger::append("DB.log", x)
    #else
        #define DBG_DBU(x) qDebug() << x
    #endif
#else
    #define DBG_DBU(x)
#endif






#endif // F_DBG

