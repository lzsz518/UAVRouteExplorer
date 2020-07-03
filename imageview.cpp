#include "AStar.hpp"
#include <QApplication>
#include <Windows.h>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QPen>
#include "imageview.h"

#define MILLISECOND 1000

//using namespace AStar;
#define UAV_ICON_SIZE 45

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    img = nullptr;
    frame_count = 0;
    raw_value = -1;
    setMouseTracking(true);
    start_point.setX(-1);
    start_point.setY(-1);
    end_point.setX(-1);
    end_point.setY(-1);
    uav_img.load(":/Pic/UAV.png");
    uav_img = uav_img.scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
}

ImageView::~ImageView()
{
    if(img!=nullptr)
        delete img;
}

void ImageView::Update(const QImage &image)
{
    if(img!=nullptr)
    {
        delete img;
        img = new QImage;
    }
    else
        img = new QImage;

    *img = image;
    update();
}

void ImageView::Update(const QImage &image, const QVector<QPoint> &_path)
{
    if(img!=nullptr)
    {
        delete img;
        img = new QImage;
    }
    else
        img = new QImage;

    *img = image;

    path.clear();
    path = _path;
}

void ImageView::SetStartPoint()
{
    isSetStartPoint = true;
    isSetEndPoint = false;
    isSetNonePoint = false;
}
void ImageView::SetEndPoint()
{
    isSetStartPoint = false;
    isSetEndPoint = true;
    isSetNonePoint = false;
}

void ImageView::SetNonePoint()
{
    isSetStartPoint = false;
    isSetEndPoint = false;
    isSetNonePoint = true;
}

void ImageView::FindPath()
{
    AStar::Generator gen;
    gen.setWorldSize({600,600});
    for(int i= 250;i<390;++i)
    {
        for(int j=160; j< 300;++j)
        {
            gen.addCollision({j,i});
        }
    }
    gen.setHeuristic(AStar::Heuristic::manhattan);
    gen.setDiagonalMovement(true);
    AStar::CoordinateList list = gen.findPath({start_point.x(),start_point.y()},{end_point.x(),end_point.y()});
    path.clear();
    for(int i=0;i<list.size();++i)
    {
        path.push_back(QPoint(list[i].x,list[i].y));
    }

    update();

    for(int i=path.size()-1;i>=0;--i)
    {
        QApplication::processEvents(QEventLoop::AllEvents);
        Sleep(100);
        uav_point.setX(path[i].x()-UAV_ICON_SIZE*0.5);
        uav_point.setY(path[i].y()-UAV_ICON_SIZE*0.5);
        update();
    }
}

void ImageView::paintEvent(QPaintEvent *event)
{
    if(img==nullptr)
        return;
    QPainter painter(this);
    int w = this->width();
    if(w == 0)
        w = 1;
    int h = this->height();
    if(h == 0)
        h = 1;
    r1 = (double)img->width()/w;
    r2 = (double)img->height()/h;
    if (r1 >= r2)
    {
        if(r1 == 0)
            r1 = 1;
        int yvalue = int(this->height()*0.5- img->height()/(2*r1));
        int heightvalue = int(img->height()/r1);
        rect.setRect(0, yvalue, this->width(), heightvalue);
    }
    else
    {
        if(r2 == 0)
            r2 = 1;
        int xvalue = int(this->width()*0.5 - img->width()/(2*r2));
        int widthvalue = int(img->width()/r2);
        rect.setRect(xvalue, 0, widthvalue, this->height());
    }

    QImage update_img = img->copy();
    QPainter painter_img(&update_img);

    QPen oldpen = painter_img.pen();
    QPen newpen;
    newpen.setColor(Qt::green);
    newpen.setWidth(2);
    painter_img.setPen(newpen);
    painter_img.drawEllipse(start_point,20,20);
    newpen.setColor(Qt::red);
    newpen.setWidth(2);
    painter_img.setPen(newpen);
    painter_img.drawEllipse(end_point,20,20);

    for(int i=0;i<path.size()-1;++i)
    {
        painter_img.drawPoint(path[i]);
    }

    painter_img.drawImage(uav_point,uav_img);

    painter_img.setPen(oldpen);
    QImage paintimg = update_img.scaled(rect.width(),rect.height());
    painter.drawImage(rect,paintimg);
}



void ImageView::resizeEvent(QResizeEvent *event)
{
    this->setAutoFillBackground(true);
      QPalette palette = this->palette();
//      palette.setBrush(QPalette::Window,
//              QBrush(QPixmap(":/Picture/Marie_Curie_1903.jpg").scaled(
//                  this->size(),
//                  Qt::IgnoreAspectRatio,
//                  Qt::SmoothTransformation)));
      palette.setBrush(QPalette::Window,QBrush(Qt::black));
      this->setPalette(palette);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
//    if(current_ximage != nullptr)
//    {
//        int x = event->x();
//        int y = event->y();

//        if(r1>=r2)
//        {
//            posx = x * r1;
//            posy = (y-rect.y()) * r1;
//        }
//        else
//        {
//            posx = (x-rect.x()) * r2;
//            posy = y * r2;
//        }

//        raw_value = current_ximage->GetPixelVal(posy,posx);
//    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if(img==nullptr)
        return;
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();
        if(r1>=r2)
        {
            x *= r1;
            y = (y-rect.y())*r1;
            if(x<0)
                x = 0;
            if(x>=img->width())
                x = img->width()-1;
            if(y<0)
                y = 0;
            if(y>=img->height())
                y = img->height()-1;
        }
        else
        {
            x  = (x-rect.x()) *r2;
            y = y *r2;
            if(x<0)
                x = 0;
            if(x>=img->width())
                x = img->width()-1;
            if(y<0)
                y = 0;
            if(y>=img->height())
                y = img->height()-1;
        }

        if(isSetStartPoint)
        {
            start_point.setX(x);
            start_point.setY(y);
            StartPointSet(QPoint(x,y));
            uav_point.setX(x-UAV_ICON_SIZE*0.5);
            uav_point.setY(y-UAV_ICON_SIZE*0.5);
        }

        if(isSetEndPoint)
        {
            end_point.setX(x);
            end_point.setY(y);
            EndPointSet(QPoint(x,y));
        }
    }

    update();
}
