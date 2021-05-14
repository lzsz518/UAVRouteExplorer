#ifndef METEOROLOGY_H
#define METEOROLOGY_H

#include <QDialog>

namespace Ui {
class meteorology;
}

class meteorology : public QDialog
{
    Q_OBJECT

public:
    explicit meteorology(QWidget *parent = nullptr);
    ~meteorology();


private slots:
    void slotOK();

private:
    Ui::meteorology *ui;
};

#endif // METEOROLOGY_H
