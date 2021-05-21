#include <QRandomGenerator>
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
    setMouseTracking(true);
    start_point.setX(-1);
    start_point.setY(-1);
    end_point.setX(-1);
    end_point.setY(-1);

    uav_img[0].load(":/Pic/UAV.png");
    uav_img[0] = uav_img[0].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[1].load(":/Pic/UAV_45.png");
    uav_img[1] = uav_img[1].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[2].load(":/Pic/UAV_90.png");
    uav_img[2] = uav_img[2].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[3].load(":/Pic/UAV_135.png");
    uav_img[3] = uav_img[3].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[4].load(":/Pic/UAV_180.png");
    uav_img[4] = uav_img[4].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[5].load(":/Pic/UAV_225.png");
    uav_img[5] = uav_img[5].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[6].load(":/Pic/UAV_270.png");
    uav_img[6] = uav_img[6].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));
    uav_img[7].load(":/Pic/UAV_315.png");
    uav_img[7] = uav_img[7].scaled(QSize(UAV_ICON_SIZE,UAV_ICON_SIZE));

    uav_angle = 0;
    percentageofdelay = 0;

    font.setFamily(font.defaultFamily());
    font.setPointSize(10);
    font.setBold(true);

    launch_delay = 0;
    landing_delay = 0;
    temp1 = QRandomGenerator::global()->bounded(9);
    temp2 = QRandomGenerator::global()->bounded(9);

    isStart = false;
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

void ImageView::Update(const QImage &image, const QPoint current_point, const QVector<QRect> &areas, const QVector<QPoint> &_path, const int angle, const QVector<float> &reliability)
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
    storm_areas.clear();
    storm_areas = areas;
    storm_reliability.clear();
    storm_reliability = reliability;
    uav_point = current_point;
    uav_point.rx() -= UAV_ICON_SIZE * 0.5;
    uav_point.ry() -= UAV_ICON_SIZE * 0.5;

    uav_angle = angle;

    CalculateDelayRate();
    update();
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

void ImageView::SetPercentageOfDelay(int i)
{
    percentageofdelay = i;
}

void ImageView::SetNormalPath(const QVector<QPoint> &npath)
{
    path_normal.clear();
    path_normal = npath;
}

void ImageView::SetStartFlag(bool enable)
{
    isStart = enable;
}

void ImageView::FindPath()
{
    AStar::Generator gen;
    gen.setWorldSize({600,600});
    for(QVector<QRect>::const_iterator itor=storm_areas.begin();itor!=storm_areas.end();++itor)
    {
        for(int i=itor->y();i<itor->y()+itor->height();++i) //Actually only setting bounding box is enough, but that's request 4 loops, the coding more cumbersome than 2 loops
        {
            for(int j=itor->x();j<itor->x()+itor->width();++j)
            {
                gen.addCollision({ j , i });
            }
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
        Sleep(1);
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

    newpen.setColor(Qt::black);
    newpen.setWidth(2);
    painter_img.setPen(newpen);
    for(int i=0;i<path_normal.size()-1;++i)
    {
        painter_img.drawPoint(path_normal[i]);
    }

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

    newpen.setColor(Qt::blue);
    painter_img.setPen(newpen);
    painter_img.setFont(font);
    for(int i=0;i<storm_areas.size();++i)
    {
        painter_img.drawRect(storm_areas[i]);
        painter_img.drawText(storm_areas[i].x(),storm_areas[i].y()-5,QString("%1").arg(storm_reliability[i]));
    }

    painter_img.drawImage(uav_point,uav_img[uav_angle/45]);

    painter_img.setPen(oldpen);
    QImage paintimg = update_img.scaled(rect.width(),rect.height());
    painter.drawImage(rect,paintimg);

    QPen pen,old_pen;
    pen.setColor(Qt::green);
    painter.setPen(pen);
    old_pen = painter.pen();
    painter.setFont(font);
    painter.drawText(10,30,QString(tr("Main Task Delay Rate: %1%")).arg(percentageofdelay));
    painter.drawText(10,50,QString(tr("LanchDelayRate: %1%")).arg(launch_delay));
    painter.drawText(10,70,QString(tr("Landing Delay Rate: %1%")).arg(landing_delay));
    painter.setPen(pen);
    painter.drawText(10,90,QString(tr("Blue number is reliability")).arg(landing_delay));
//    if(isStart)
//        painter.drawText(10,110,QString(tr("Recommend flying height 4000m")));
    painter.setPen(old_pen);
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

    if(event->button() == Qt::LeftButton)
    {
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

    if(event->button() == Qt::RightButton)
    {
        RiskRatePointSet(QPoint(x,y));
    }

    update();
}

void ImageView::CalculateDelayRate()
{
    if(storm_areas.empty())
        return;

    double start_dis =0;
    double end_dis = 0;
    for(int i=0;i<storm_areas.size();++i)
    {
        QPoint center = storm_areas[i].center();
        start_dis += sqrt(pow(center.x()-start_point.x(),2)+pow(center.y()-start_point.y(),2));
        end_dis += sqrt(pow(center.x()-end_point.x(),2)+pow(center.y()-end_point.y(),2));
    }

    start_dis /= storm_areas.size();
    end_dis /= storm_areas.size();

    if(start_dis<30)
        launch_delay = 60;
    if(start_dis>30 && start_dis<80)
        launch_delay = 50;
    if(start_dis>80 && start_dis<130)
        launch_delay = 40;
    if(start_dis>130 && start_dis<180)
        launch_delay = 30;
    if(start_dis>180 && start_dis<230)
        launch_delay = 20;
    if(start_dis>230)
        launch_delay = 0;

    if(end_dis<30)
        landing_delay = 60;
    if(end_dis>30 && end_dis<80)
        landing_delay = 50;
    if(end_dis>80 && end_dis<130)
        landing_delay = 40;
    if(end_dis>130 && end_dis<180)
        landing_delay = 30;
    if(end_dis>180 && end_dis<230)
        landing_delay = 20;
    if(end_dis>230)
        landing_delay = 0;


    if(launch_delay>0)
        launch_delay += temp1;
    if(landing_delay>0)
        landing_delay += temp1;
}
