#include <QString>
#include <QtTest>

#include <libfusion.h>
#include <fdb.h>
#include <fgame.h>

class FusionTestSuit : public QObject
{
    Q_OBJECT

public:
    FusionTestSuit();
private:
    FDB db;
private Q_SLOTS:
    void dbCreate();
    void gameCreate();
    void DatabasePrefs();
    void addGame();
};

FusionTestSuit::FusionTestSuit()
{
    QLoggingCategory::setFilterRules("*=false\n");
}

void FusionTestSuit::dbCreate()
{
    QDir workingDir = LibFusion::getWorkingDir();
    QFile dbFile(workingDir.absolutePath() + QDir::separator() + "fusion.db");

    if(dbFile.exists()) {
        qDebug() << "Deleted Existing Databse-File";
        dbFile.remove();
    }

    //Database does not exist now
    QVERIFY(!dbFile.exists());

    QVERIFY(db.init());

    //Check if DB-File is created
    QVERIFY(dbFile.exists());
}

void FusionTestSuit::gameCreate()
{
    //Test Constructor
    FGame g("TestGame1", Executable, "TestGaeDirectory", "exePath", QStringList() << QString("ArgsofTestGame"));
    QCOMPARE(g.getName(), QString("TestGame1"));
    QCOMPARE(g.getType(), Executable);
    QCOMPARE(g.getArgs(), QStringList() << QString("ArgsofTestGame"));
    QCOMPARE(g.getPath(), QString("TestGaeDirectory"));
    QCOMPARE(g.getExe(), QString("exePath"));

    //Test for getters and Setters
    g.setName("newGamesName");
    QCOMPARE(g.getName(), QString("newGamesName"));

    g.setPath("testPathforNewGame");
    QCOMPARE(g.getPath(), QString("testPathforNewGame"));

    g.setExe("newGameExe");
    QCOMPARE(g.getExe(), QString("newGameExe"));

    g.setType(Steam);
    QCOMPARE(g.getType(), Steam);

    g.setArgs(QStringList() << "Check g.setArgs");
    QCOMPARE(g.getArgs(), QStringList() << "Check g.setArgs");
}



void FusionTestSuit::DatabasePrefs()
{
    QCOMPARE(db.getTextPref("nonExistent"), QString());
    QCOMPARE(db.getTextPref(""), QString());
    QCOMPARE(db.getTextPref("nonExistent", "defaultValue"), QString("defaultValue"));

    QVERIFY(db.addTextPref("newPref", "newPrefs' value"));
    QCOMPARE(db.getTextPref("newPref"), QString("newPrefs' value"));
    QCOMPARE(db.getTextPref("newPref", "with defaultVal"), QString("newPrefs' value"));

//delete Pref, getPref should now return an empty string
    QVERIFY(db.deletePref("newPref"));
    QCOMPARE(db.getTextPref("newPref"), QString());

   //#################
   //INT-PREFS
    QCOMPARE(db.getIntPref("nonExistent"), -1);
    QCOMPARE(db.getIntPref(""), -1);
    //get Default-value
    QCOMPARE(db.getIntPref("nonExistent", 14),14); 

    // create Prefs
    QVERIFY(db.addIntPref("newPref", 33));
    QCOMPARE(db.getIntPref("newPref"), 33); 
    QCOMPARE(db.getIntPref("newPref", 53), 33);

//delete Pref, getPref should now return an empty string
    QVERIFY(db.deletePref("newPref"));
    QCOMPARE(db.getIntPref("newPref"), -1);

    QVERIFY(db.addIntPref("newPref1", 0));
    QCOMPARE(db.getIntPref("newPref1"), 0); 
    QVERIFY(db.addIntPref("newPref2", -13));
    QCOMPARE(db.getIntPref("newPref2"), -13); 
    QVERIFY(db.addIntPref("newPref3", 214356570));
    QCOMPARE(db.getIntPref("newPref3"), 214356570); 

}

void FusionTestSuit::addGame()
{

}

QTEST_APPLESS_MAIN(FusionTestSuit)

#include "tst_fusiontestsuit.moc"
