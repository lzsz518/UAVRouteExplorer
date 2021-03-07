#include "augmentationparameter.h"
#include "ui_augmentationparameter.h"

AugmentationParameter::AugmentationParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AugmentationParameter)
{
    ui->setupUi(this);
}

AugmentationParameter::~AugmentationParameter()
{
    delete ui;
}

int AugmentationParameter::GetEpoch()
{
    return ui->sb_epoch->value();
}

int AugmentationParameter::GetFrames()
{
    return ui->sb_frames->value();
}
