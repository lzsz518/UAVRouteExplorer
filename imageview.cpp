#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include "imageview.h"

#define MILLISECOND 1000

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    img = nullptr;
    frame_count = 0;
    raw_value = -1;
    setMouseTracking(true);
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

    QImage paintimg = img->scaled(rect.width(),rect.height());
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
