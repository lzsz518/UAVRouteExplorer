#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/Pic/UAV.png"));
    connect(ui->pb_close,&QPushButton::clicked,this,&AboutDialog::close);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
