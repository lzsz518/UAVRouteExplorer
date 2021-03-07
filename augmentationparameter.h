#ifndef AUGMENTATIONPARAMETER_H
#define AUGMENTATIONPARAMETER_H

#include <QDialog>

namespace Ui {
class AugmentationParameter;
}

class AugmentationParameter : public QDialog
{
    Q_OBJECT

public:
    explicit AugmentationParameter(QWidget *parent = nullptr);
    ~AugmentationParameter();

    int GetEpoch();
    int GetFrames();
private:
    Ui::AugmentationParameter *ui;
};

#endif // AUGMENTATIONPARAMETER_H
