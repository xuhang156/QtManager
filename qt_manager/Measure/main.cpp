#include "MainWindow.h"
#include "LoginDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QApplication>

bool openDataBase()
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/measure.db");
    if (!db.open()) {
        QMessageBox::critical(NULL, "Title", "Failed to open measure.db", QMessageBox::Yes);
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");

    mmind::setupGui(&a, "");
    qRegisterMetaType<mmind::SearchInfo>("mmind::SearchInfo");

    bool openDBFlag = openDataBase();
    LoginDialog login;
    MainWindow w;

    if (!openDBFlag || !login.exec()) {
        return -1;
    }
    w.show();
    return a.exec();
}
