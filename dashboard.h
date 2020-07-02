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
    void slotSetStartPoint();
    void slotSetEndPoint();
    void slotGetStartPoint(QPoint p);
    void slotGetEndPoint(QPoint p);
private:
    Ui::Dashboard *ui;
    ImageView *view;

    vector<QImage*> storm_images;
    vector<vector<QRect>> storm_areas;
    void ClearImage();
    void OpenImages();
    QString GetDataFileName(const QString &image_name);

    void LoadTxtFile(const char *pathName, const int width, const int height, vector<QRect> &areas);
    int  ReadNumber(vector<float> &Numbers,const char* NumberStr,int NumberCount);
};

#endif // DASHBOARD_H
