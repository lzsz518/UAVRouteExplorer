#include <QFile>
#include "mainwindow.h"
#include "dashboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString qss;
    QFile qssFile(":/QSS/style.qss");
    qssFile.open(QFile::ReadOnly);

    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        a.setStyleSheet(qss);
        qssFile.close();
    }
    MainWindow w;
    w.show();
//    Dashboard db;
//    db.show();
    return a.exec();
}
