#include <QResizeEvent>
#include "aboutdialog.h"
#include "dashboard.h"
#include "mainwindow.h"
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




