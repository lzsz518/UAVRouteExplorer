#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotNewPathExploer();
    void slotViewCascade();
    void slotViewTile();
    void slotExit();
    void slotAbout();
    void slotAugmentation();

private:
    Ui::MainWindow *ui;
    QMdiArea mdiarea;

    void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
