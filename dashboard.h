#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <vector>
#include <QWidget>
#include <QTimer>

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
    void slotPrevFrame();
    void slotNextFrame();
    void slotAnimationTimer();
private:
    int frame_index;
    int path_index;
    Ui::Dashboard *ui;
    ImageView *view;
    QPoint start_point;
    QPoint end_point;
    QTimer animation_timer;

    QVector<QImage*> storm_images;
    QVector<QVector<QRect>> storm_areas;
    QVector<QPoint> paths;

    void ClearImage();
    void OpenImages();
    QVector<QPoint> FindPath(const int worldWidth,const int worldHeight, const QPoint start_point, const QPoint end_point, const QVector<QRect> &areas);
    QString GetDataFileName(const QString &image_name);

    void LoadTxtFile(const char *pathName, const int width, const int height, QVector<QRect> &areas);
    int  ReadNumber(vector<float> &Numbers,const char* NumberStr,int NumberCount);
    int GetUAVAngle(QPoint p1,QPoint p2);
};

#endif // DASHBOARD_H
