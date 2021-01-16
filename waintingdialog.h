#ifndef WAINTINGDIALOG_H
#define WAINTINGDIALOG_H

#include <QDialog>

namespace Ui {
class WaintingDialog;
}

class WaintingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaintingDialog(QWidget *parent = nullptr);
    ~WaintingDialog();

    void SetPara(int imagenum,int sleeptime);
private:
    Ui::WaintingDialog *ui;

    int _imagenum;
    int _sleeptime;
    int count;

    void showEvent(QShowEvent *event);
private slots:
    void slotStart();

};

#endif // WAINTINGDIALOG_H
