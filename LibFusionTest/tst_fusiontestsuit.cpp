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

void FusionTestSuit::addGame()
{

}

QTEST_APPLESS_MAIN(FusionTestSuit)

#include "tst_fusiontestsuit.moc"
