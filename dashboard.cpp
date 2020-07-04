#include <fstream>
#include <QFileDialog>
#include <QVector>
#include "AStar.hpp"
#include "imageview.h"
#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    view = new ImageView;
    ui->layout_image->addWidget(view);

    view->SetStartPoint();
    connect(ui->pb_openimage,SIGNAL(clicked()),this,SLOT(slotOpenImage()));
    connect(ui->rb_startpoint,SIGNAL(clicked()),this,SLOT(slotSetStartPoint ()));
    connect(ui->rb_endpoint,SIGNAL(clicked()),this,SLOT(slotSetEndPoint ()));
    connect(ui->pb_explore,SIGNAL(clicked()),this,SLOT(slotExplore()));
    connect(view,SIGNAL(StartPointSet(QPoint)),this,SLOT(slotGetStartPoint(QPoint)));
    connect(view,SIGNAL(EndPointSet(QPoint)),this,SLOT(slotGetEndPoint(QPoint)));
    connect(ui->pb_prevframe,SIGNAL(clicked()),this,SLOT(slotPrevFrame()));
    connect(ui->pb_nextframe,SIGNAL(clicked()),this,SLOT(slotNextFrame()));
    connect(&animation_timer,SIGNAL(timeout()),this,SLOT(slotAnimationTimer()));

    frame_index = 0;
}

Dashboard::~Dashboard()
{
    delete ui;
    ClearImage();
}

void Dashboard::slotOpenImage()
{
//    QString fileName= QFileDialog::getOpenFileName(this,
//         tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
//    if(fileName.isEmpty())
//        return;

//    QImage *img = new QImage;
//    if(img==nullptr)
//        return;

//    img->load(fileName);

//    storm_images.push_back(img);
//    if(view!=nullptr)
//        view->Update(*img);

    OpenImages();
}

void Dashboard::slotExplore()
{
//    view->FindPath();
    paths.clear();
    paths = FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),start_point,end_point,storm_areas[frame_index]);
    path_index = paths.size()-1;
    animation_timer.start(50);
}

void Dashboard::slotStop()
{

}

void Dashboard::slotClose()
{

}

void Dashboard::slotSetStartPoint()
{
    if(view!=nullptr)
    {
        view->SetStartPoint();
    }
}

void Dashboard::slotSetEndPoint()
{
    if(view!=nullptr)
    {
        view->SetEndPoint();
    }
}

void Dashboard::slotGetStartPoint(QPoint p)
{
    ui->le_startX->setText(QString("%1").arg(p.x()));
    ui->le_startY->setText(QString("%1").arg(p.y()));

    start_point = p;
}

void Dashboard::slotGetEndPoint(QPoint p)
{
    ui->le_endX->setText(QString("%1").arg(p.x()));
    ui->le_endY->setText(QString("%1").arg(p.y()));

    end_point = p;
}

void Dashboard::slotPrevFrame()
{
    --frame_index;
    if(frame_index<0)
        frame_index = 0;

    if(storm_images.empty())
        return;

    QVector<QPoint> p;
    view->Update(*storm_images[frame_index],start_point,storm_areas[frame_index],p);
}

void Dashboard::slotNextFrame()
{
    ++frame_index;
    if(frame_index>=storm_images.size())
        frame_index = storm_images.size()-1;

    if(storm_images.empty())
        return;

    QVector<QPoint> p;
    view->Update(*storm_images[frame_index],start_point,storm_areas[frame_index],p);
}

void Dashboard::slotAnimationTimer()
{
    if(view!=nullptr)
    {
        view->Update(*storm_images[frame_index],paths[path_index],storm_areas[frame_index],paths);
        --path_index;
        if(path_index < 0)
        {
            path_index = paths.size()-1;
            animation_timer.stop();
        }
    }
}

void Dashboard::ClearImage()
{
    for(QVector<QImage*>::iterator itor= storm_images.begin();itor!=storm_images.end();++itor)
    {
        delete *itor;
    }
    storm_images.clear();
}

void Dashboard::OpenImages()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
         tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileNames.empty())
        return;

    ClearImage();
    storm_areas.clear();
    paths.clear();

    for(int i=0;i<fileNames.size();++i)
    {
        QImage *img = new QImage;
        img->load(fileNames[i]);
        storm_images.push_back(img);
        QString dataFineName = GetDataFileName(fileNames[i]);
        QVector<QRect> areas;
        LoadTxtFile(dataFineName.toStdString().c_str(),img->width(),img->height(),areas);
        storm_areas.push_back(areas);
    }

    frame_index = 0;
    QVector<QPoint> p;
    view->Update(*storm_images[0],QPoint(0,0),storm_areas[0],p);
}

QVector<QPoint> Dashboard::FindPath(const int world_width,const int world_height, const QPoint start_point, const QPoint end_point, const QVector<QRect> &areas)
{
    QVector<QPoint> path;
    AStar::Generator gen;
    gen.setWorldSize({world_width,world_height});
    gen.setHeuristic(AStar::Heuristic::manhattan);
    gen.setDiagonalMovement(true);

    for(QVector<QRect>::const_iterator itor=areas.begin();itor!=areas.end();++itor)
    {
        for(int i=itor->y();i<itor->y()+itor->height();++i) //Actually only setting bounding box is enough, but that's request 4 loops, the coding more cumbersome than 2 loops
        {
            for(int j=itor->x();j<itor->x()+itor->width();++j)
            {
                gen.addCollision({ j , i });
            }
        }
    }

    AStar::CoordinateList list = gen.findPath({start_point.x(),start_point.y()},{end_point.x(),end_point.y()});
    path.clear();
    for(int i=0;i<list.size();++i)
    {
        path.push_back(QPoint(list[i].x,list[i].y));
    }

    return path;
}
QString Dashboard::GetDataFileName(const QString &image_name)
{
    QString result = image_name;
    result.remove(result.size()-3,3);
    result.append("txt");

    return result;
}


void Dashboard::LoadTxtFile(const char *pathName, const int width, const int height, QVector<QRect> &areas)
{
    areas.clear();
    fstream fin;
    fin.open(pathName,fstream::in);
    if(!fin.is_open())
        return ;

    const int line_len = 4096;

    char Line[line_len]={0};
    vector<float> data;
    data.clear();

    while(fin.getline(Line,line_len))
    {
        ReadNumber(data,Line, 5);
        if(data.size()==5)
        {
            QRect r;
            int left,top;
            int halfw,halfh;
            halfw = width*data[3]*0.5;
            halfh = height*data[4]*0.5;
            left = width*data[1] - halfw;
            top = height*data[2] - halfh;

            r.setX(left);
            r.setY(top);
            r.setWidth(halfw<<1);
            r.setHeight(halfh<<1);
            areas.push_back(r);
        }
        memset(Line,0,line_len);
    }
}

int  Dashboard::ReadNumber(vector<float> &Numbers,const char* NumberStr,int NumberCount)
{
    Numbers.clear();

    char tempstr[32] = {0};
    int i = 0;
    int j = 0;

    while(NumberStr[i]!='\0')
    {
        if(NumberStr[i]!=0x20)
        {
            tempstr[j] = NumberStr[i];
            ++j;
        }
        else
        {
            if(NumberStr[i-1]!=0x20)
//                Numbers.push_back(atoi(tempstr));
                Numbers.push_back(stod(tempstr));
            j = 0;
            memset(tempstr,0,sizeof(char)*32);
        }
        ++i;
    }

    if(strlen(tempstr)>0)
        Numbers.push_back(stod(tempstr));

    if(Numbers.size()!=NumberCount)
    {
        Numbers.clear();
        return 1;
    }

    return 0;
}













