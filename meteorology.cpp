#include <QSettings>
#include "meteorology.h"
#include "ui_meteorology.h"

meteorology::meteorology(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::meteorology)
{
    ui->setupUi(this);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&meteorology::slotOK);

    QSettings ini("./Setting.ini",QSettings::IniFormat);
    ui->le_risk->setText(ini.value("meteorology/risk").toString());
    ui->le_ice->setText(ini.value("meteorology/ice").toString());
    ui->le_richardson->setText(ini.value("meteorology/richardson").toString());
}

meteorology::~meteorology()
{
    delete ui;
}

void meteorology::slotOK()
{
    QSettings ini("./Setting.ini",QSettings::IniFormat);
    ini.setValue("meteorology/risk",ui->le_risk->text());
    ini.setValue("meteorology/ice",ui->le_ice->text());
    ini.setValue("meteorology/richardson",ui->le_richardson->text());
}
