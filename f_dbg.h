#ifndef F_DBG
#define F_DBG

#include "qdebug.h"

//Globally enable DBG-Outputs
#define DBG_PRINT_ENABLED


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
    #define DBG_CRWL(x) qDebug() << x
#else
    #define DBG_CRWL(x)
#endif

#ifdef DBG_CRWL2_EN
    #define DBG_CRWL2(x) qDebug() << x
#else
    #define DBG_CRWL2(x)
#endif



#ifdef DBG_DB_EN
    #define DBG_DB(x) qDebug() << x
#else
    #define DBG_DB(x)
#endif

#ifdef DBG_DBU_EN
    #define DBG_DBU(x) qDebug() << x
#else
    #define DBG_DBU(x)
#endif






#endif // F_DBG

