#include <QSettings>
#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Setting::slotOK);

    QSettings ini("./Setting.ini",QSettings::IniFormat);
    ui->le_type->setText( ini.value("Setting/type").toString());
    ui->le_speed->setText(ini.value("Setting/speed").toString());
    ui->le_ceiliing->setText(ini.value("Setting/ceiling").toString());
    ui->le_duration->setText(ini.value("Setting/duration").toString());
    ui->le_flyweight->setText(ini.value("Setting/flyweight").toString());
    ui->le_load->setText(ini.value("Setting/load").toString());
}

Setting::~Setting()
{
    delete ui;
}

void Setting::slotOK()
{
    QSettings ini("./Setting.ini",QSettings::IniFormat);
    ini.setValue("Setting/type",ui->le_type->text());
    ini.setValue("Setting/speed",ui->le_speed->text());
    ini.setValue("Setting/ceiling",ui->le_ceiliing->text());
    ini.setValue("Setting/duration",ui->le_duration->text());
    ini.setValue("Setting/flyweight",ui->le_flyweight->text());
    ini.setValue("Setting/load",ui->le_load->text());
}
