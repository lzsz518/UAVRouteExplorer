#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QTime>
#include <QVector>

#define UAV_IMAGE_NUMBER 8

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    virtual ~ImageView();

    void Update(const QImage &image);
    void Update(const QImage &image, const QPoint current_point,const QVector<QRect> &areas,  const QVector<QPoint> &_path, int const angle=0,const QVector<float> &reliability = QVector<float>());
    void SetStartPoint();
    void SetEndPoint();
    void SetNonePoint();
    void SetPercentageOfDelay(int i);
    void SetNormalPath(const QVector<QPoint> &npath);
    void SetStartFlag(bool enable);

    void FindPath();
protected:
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void CalculateDelayRate();
signals:
    void StartPointSet(QPoint);
    void EndPointSet(QPoint);
    void RiskRatePointSet(QPoint);

private:
    int uav_angle;
    int posx,posy;
    unsigned long long frame_count;
    double r1;
    double r2;
    double fps;
    double launch_delay;
    double landing_delay;
    double temp1;
    double temp2;
    bool isSetStartPoint;
    bool isSetEndPoint;
    bool isSetNonePoint;
    bool isStart;
    int percentageofdelay;
    QRect rect;
    QImage *img;
    QTime fps_timer;
    QImage uav_img[UAV_IMAGE_NUMBER];

    QFont font;
    QPoint start_point;
    QPoint end_point;
    QPoint uav_point;
    QVector<QRect> storm_areas;
    QVector<QPoint> path;
    QVector<QPoint> path_normal;
    QVector<float> storm_reliability;
};

#endif // IMAGEVIEW_H
