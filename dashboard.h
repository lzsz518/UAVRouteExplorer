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

    QVector<QImage*> storm_images;
    QVector<QVector<QRect>> storm_areas;
    QVector<QVector<QPoint>> paths;
    void ClearImage();
    void OpenImages();
    QVector<QPoint> FindPath(const int worldWidth,const int worldHeight, const QPoint start_point, const QPoint end_point, const QVector<QRect> &areas);
    QString GetDataFileName(const QString &image_name);

    void LoadTxtFile(const char *pathName, const int width, const int height, QVector<QRect> &areas);
    int  ReadNumber(vector<float> &Numbers,const char* NumberStr,int NumberCount);
};

#endif // DASHBOARD_H
