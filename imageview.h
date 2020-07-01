#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QTime>
#include <QVector>


class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    virtual ~ImageView();

    void Update(const QImage &image);
    void SetStartPoint();
    void SetEndPoint();
    void SetNonePoint();

    void FindPath();
protected:
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    int raw_min;
    int raw_max;
    int raw_value;
    int posx,posy;
    unsigned long long frame_count;
    double r1;
    double r2;
    double fps;
    bool isSetStartPoint;
    bool isSetEndPoint;
    bool isSetNonePoint;
    QRect rect;
    QImage *img;
    QTime fps_timer;

    QPoint start_point;
    QPoint end_point;
    QVector<QPoint> path;
};

#endif // IMAGEVIEW_H
