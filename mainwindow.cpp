#include <QResizeEvent>
#include <QSettings>
#include <QProcess>
#include "aboutdialog.h"
#include "dashboard.h"
#include "mainwindow.h"
#include "augmentationparameter.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(&mdiarea);
    connect(ui->actionNewPathExploer,&QAction::triggered,this,&MainWindow::slotNewPathExploer);
    connect(ui->action_Cascade,&QAction::triggered,this,&MainWindow::slotViewCascade);
    connect(ui->action_Tile,&QAction::triggered,this,&MainWindow::slotViewTile);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::slotExit);
    connect(ui->action_About,&QAction::triggered,this,&MainWindow::slotAbout);
    connect(ui->actionAug,&QAction::triggered,this,&MainWindow::slotAugmentation);

    setWindowIcon(QIcon(":/Pic/UAV.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotNewPathExploer()
{
    Dashboard *db = new Dashboard;
    if(db!=nullptr)
    {
        mdiarea.addSubWindow(db);
        db->show();
    }
}

void MainWindow::slotViewCascade()
{
    mdiarea.cascadeSubWindows();
}

void MainWindow::slotViewTile()
{
    mdiarea.tileSubWindows();
}

void MainWindow::slotExit()
{
    close();
}

void MainWindow::slotAbout()
{
    AboutDialog ad;
    ad.exec();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    mdiarea.setBackground(QBrush(QPixmap(tr(":/Pic/Rainbow-4.jpg")).scaled(
                     event->size().width(),
                     event->size().height())));
}

void MainWindow::slotAugmentation()
{
    AugmentationParameter ap;
    if(ap.exec()==QDialog::Accepted)
    {
        QSettings aug_ini("./AugmentationParameter.ini",QSettings::IniFormat);
        aug_ini.setValue("Settings/Epoch",QString("%1").arg(ap.GetEpoch()));
        aug_ini.setValue("Settings/Frames",QString("%1").arg(ap.GetFrames()));
//        QProcess process;
//        process.execute("py C:/Work_Test/build-UAVRouteExplorer-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/demo1.py");
    }
}


