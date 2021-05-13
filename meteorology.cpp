#include "meteorology.h"
#include "ui_meteorology.h"

meteorology::meteorology(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::meteorology)
{
    ui->setupUi(this);
}

meteorology::~meteorology()
{
    delete ui;
}
