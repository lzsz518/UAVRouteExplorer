#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <vector>
#include <QWidget>

namespace Ui {
class Dashboard;
}

using namespace std;

class ImageView;

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private slots:
    void slotOpenImage();
    void slotExplore();
    void slotStop();
    void slotClose();
private:
    Ui::Dashboard *ui;
    ImageView *view;

    vector<QImage*> storm_images;
    vector<vector<QRect>> storm_areas;
};

#endif // DASHBOARD_H
