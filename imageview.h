#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QTime>


class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    virtual ~ImageView();

    void Update(const QImage &image);
protected:
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int raw_min;
    int raw_max;
    int raw_value;
    int posx,posy;
    unsigned long long frame_count;
    double r1;
    double r2;
    double fps;
    QRect rect;
    QImage *img;
    QTime fps_timer;
};

#endif // IMAGEVIEW_H
