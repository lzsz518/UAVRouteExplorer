#include <QThread>
#include "waintingdialog.h"
#include "ui_waintingdialog.h"

WaintingDialog::WaintingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaintingDialog)
{
    ui->setupUi(this);
    _imagenum = 0;
    _sleeptime = 0;
    count = 0;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotStart()));
}

WaintingDialog::~WaintingDialog()
{
    delete ui;
}


void WaintingDialog::SetPara(int imagenum, int sleeptime)
{
    _imagenum = imagenum;
    _sleeptime = sleeptime;
}


void WaintingDialog::showEvent(QShowEvent *event)
{

}

void WaintingDialog::slotStart()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(_imagenum*_sleeptime-1);
    int minsleeptime = _sleeptime/1000;
    if(minsleeptime<=0)
        minsleeptime = 1;
    for(int i=0;i<ui->progressBar->maximum();++i)
    {
        ui->progressBar->setValue(i);
        QThread::msleep(minsleeptime);
        QApplication::processEvents(QEventLoop::AllEvents);
    }

    accept();
}
