#include "mainwindow.h"
#include "dashboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    Dashboard db;
    db.show();
    return a.exec();
}
