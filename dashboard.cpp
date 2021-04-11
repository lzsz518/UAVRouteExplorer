#include <QSettings>
#include <QProcess>
#include <fstream>
#include <QFileDialog>
#include <QVector>
#include <QMessageBox>
#include "waintingdialog.h"
#include "AStar.hpp"
#include "imageview.h"
#include "dashboard.h"
#include "ui_dashboard.h"
//#undef slots
//#include <Python.h>
//#define slots Q_SLOTS

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    view = new ImageView;
    view->setMinimumSize(300,300);
    ui->layout_image->addWidget(view);

    view->SetStartPoint();
    connect(ui->pb_openimage,SIGNAL(clicked()),this,SLOT(slotOpenImage()));
    connect(ui->rb_startpoint,SIGNAL(clicked()),this,SLOT(slotSetStartPoint()));
    connect(ui->rb_endpoint,SIGNAL(clicked()),this,SLOT(slotSetEndPoint()));
    connect(ui->pb_explore,SIGNAL(clicked()),this,SLOT(slotExplore()));
    connect(ui->pb_stop,SIGNAL(clicked()),this,SLOT(slotStop()));
    connect(ui->pb_close,SIGNAL(clicked()),this,SLOT(slotClose()));
    connect(view,SIGNAL(StartPointSet(QPoint)),this,SLOT(slotGetStartPoint(QPoint)));
    connect(view,SIGNAL(EndPointSet(QPoint)),this,SLOT(slotGetEndPoint(QPoint)));
    connect(ui->pb_prevframe,SIGNAL(clicked()),this,SLOT(slotPrevFrame()));
    connect(ui->pb_nextframe,SIGNAL(clicked()),this,SLOT(slotNextFrame()));
    connect(&animation_timer,SIGNAL(timeout()),this,SLOT(slotAnimationTimer()));

    QDateTime dt;
    setWindowTitle(dt.currentDateTime().toString());

    setWindowIcon(QIcon(":/Pic/UAV.png"));
    frame_index = 0;
}

Dashboard::~Dashboard()
{
    delete ui;
    ClearImage();
}

void Dashboard::slotOpenImage()
{

    OpenImages();
}

void Dashboard::slotExplore()
{
    if(storm_images.empty())
    {
        QMessageBox::information(this,"信息","请选择图像");
        return;
    }
    paths.clear();
    paths = FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),start_point,end_point,storm_areas[frame_index]);
    noncollision_paths.clear();
    noncollision_paths = FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),start_point,end_point,QVector<QRect>());
    int temp = paths.size()*100/noncollision_paths.size()-100;
    view->SetPercentageOfDelay(temp);
    path_index = 0;
    view->SetNonePoint();
    DisableUI();
    animation_timer.start(ui->hs_uavspeed->value());
}

void Dashboard::slotStop()
{
    ui->pb_explore->setEnabled(true);
    ResetUI();
}

void Dashboard::slotClose()
{
    animation_timer.stop();
    if(parentWidget()!=nullptr)
        parentWidget()->close();
    else
        close();
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
    if(frame_index==0)
        return;

    --frame_index;

    if(storm_images.empty())
        return;


    if(animation_timer.isActive())
    {
        animation_timer.stop();
        QVector<QPoint> new_path =  FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),paths[path_index],end_point,storm_areas[frame_index]);
        paths.remove(path_index,paths.size()-path_index);
        noncollision_paths.clear();
        noncollision_paths = FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),paths[path_index],end_point,QVector<QRect>());
        view->SetPercentageOfDelay((paths.size()/noncollision_paths.size())*100-100);
        for(int i=0;i<new_path.size();++i)
        {
            paths.push_back(new_path[i]);
        }
       animation_timer.start();
    }
    else
    {
        QVector<QPoint> p;
        view->Update(*storm_images[frame_index],start_point,storm_areas[frame_index],p,0,storm_reliability[frame_index]);
    }

}

void Dashboard::slotNextFrame()
{
    if(frame_index>=storm_images.size()-1)
        return;

    ++frame_index;

    if(storm_images.empty())
        return;

    if(animation_timer.isActive())
    {
        animation_timer.stop();
        QVector<QPoint> new_path =  FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),paths[path_index],end_point,storm_areas[frame_index]);
        paths.remove(path_index,paths.size()-path_index);
        noncollision_paths.clear();
        noncollision_paths = FindPath(storm_images[frame_index]->width(),storm_images[frame_index]->height(),paths[path_index],end_point,QVector<QRect>());
        view->SetPercentageOfDelay((paths.size()/noncollision_paths.size())*100-100);

        for(int i=0;i<new_path.size();++i)
        {
            paths.push_back(new_path[i]);
        }
       animation_timer.start();
    }
    else
    {
        QVector<QPoint> p;
        view->Update(*storm_images[frame_index],start_point,storm_areas[frame_index],p,0,storm_reliability[frame_index]);
    }
}

void Dashboard::slotAnimationTimer()
{
    if(view!=nullptr)
    {
        int angle;
        if(path_index+1<paths.size())
        {
            angle = GetUAVAngle(paths[path_index],paths[path_index+1]);
        }
        else
        {
            angle = GetUAVAngle(paths[path_index-1],paths[path_index]);
        }

        view->Update(*storm_images[frame_index],paths[path_index],storm_areas[frame_index],paths,angle,storm_reliability[frame_index]);
        ++path_index;
        if(path_index >= paths.size())
        {
            path_index = paths.size()-1;
            animation_timer.stop();
            ResetUI();
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

    FILE *file = fopen("./ImageNames.txt","w");

    for(int i=0;i<fileNames.size();++i)
    {
        fprintf(file,fileNames[i].toStdString().c_str());
        fprintf(file,"\n");
    }

    fclose(file);

    RunPython(fileNames);
    WaintingDialog wd;
    QSettings * configIniRead = new QSettings("./Setting.ini",QSettings::IniFormat);
    int delay = configIniRead->value("Setting/Delay").toInt();
    wd.SetPara(fileNames.count(),delay);
    if(wd.exec()==QDialog::Rejected)
        return;

    ClearImage();
    storm_areas.clear();
    paths.clear();
    storm_reliability.clear();

    for(int i=0;i<fileNames.size();++i)
    {
        QImage *img = new QImage;
        img->load(fileNames[i]);
        storm_images.push_back(img);
        QString dataFineName = GetDataFileName(fileNames[i]);
        QVector<QRect> areas;
        QVector<float> rel;
        LoadTxtFile(dataFineName.toStdString().c_str(),img->width(),img->height(),areas,rel);
        storm_areas.push_back(areas);
        storm_reliability.push_back(rel);
    }

    frame_index = 0;
    QVector<QPoint> p;
    view->Update(*storm_images[0],QPoint(0,0),storm_areas[0],p,0,storm_reliability[0]);
}

void Dashboard::RunPython(const QStringList &ImageNames)
{
//    Py_Initialize();
//    PyObject * sys = PyImport_ImportModule("sys");
//    PyObject * path = PyObject_GetAttrString(sys, "path");
//    PyList_Append(path, PyUnicode_FromString("."));

//    PyObject * ModuleString = PyUnicode_FromString("demo2");
//    PyObject * Module = PyImport_Import(ModuleString);
//    PyObject * Dict = PyModule_GetDict(Module);
//    PyObject * Func = PyDict_GetItemString(Dict, "printList");


//    PyObject * Result = PyObject_CallObject(Func, NULL);

//    PyObject* pModule = nullptr;
//    PyObject* pFunc = nullptr;

//    pModule = PyImport_ImportModule("./demo2");
//    if(pModule==nullptr)
//        return;

//    PyRun_SimpleString("execfile('./demo1.py')");
    QProcess process;
    process.execute("python ./yolov4_pytorch/predict.py");

//	if(Module && PyCallable_Check(Func))
//    {
//        PyObject* pArg = PyList_New(0);
//        PyList_Append(pArg,Py_BuildValue("(s)",ImageNames[0].toStdString().c_str()));
//        PyObject *args = PyTuple_New(1);
//        PyTuple_SetItem(args,0,pArg);
//        PyRun_SimpleString("execfile('demo1.py')");
////        PyEval_CallObject(Func,args);
//    }

//    Py_Finalize();
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
        QRect r = *itor;
        if(ui->hs_uavmargin->value()>0)
        {
            int newWidth = r.width()*(1.0+(double)ui->hs_uavmargin->value()/100);
            int newHeight = r.height()*(1.0+(double)ui->hs_uavmargin->value()/100);
            int x = r.x() - ((newWidth - r.width())*0.5);
            int y = r.y() - ((newHeight - r.height())*0.5);
            if(x<0)
                x = 0;
            if(y<0)
                y = 0;
            if(x + newWidth>=world_width)
                newWidth = world_width -x;
            if(y + newHeight>=world_height)
                newHeight = world_height - y;

            r.setX(x);
            r.setY(y);
            r.setWidth(newWidth);
            r.setHeight(newHeight);
        }
        for(int i=r.y();i<r.y()+r.height();++i) //Actually only setting bounding box is enough, but that's request 4 loops, the coding more cumbersome than 2 loops
        {
            for(int j=r.x();j<r.x()+r.width();++j)
            {
                gen.addCollision({ j , i });
            }
        }
    }

    AStar::CoordinateList list = gen.findPath({start_point.x(),start_point.y()},{end_point.x(),end_point.y()});
    path.clear();
    for(int i=0;i<list.size();++i)
    {
        path.push_front(QPoint(list[i].x,list[i].y));
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


void Dashboard::LoadTxtFile(const char *pathName, const int width, const int height, QVector<QRect> &areas, QVector<float> &rel, const double relth)
{
    areas.clear();
    rel.clear();
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
        ReadNumber(data,Line, 6);
        if(data.size()==6)
        {
            if(data[5]<relth)
                continue;
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
            rel.push_back(data[5]);
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

int Dashboard::GetUAVAngle(QPoint p1, QPoint p2)
{
    int deltaX = p2.x()-p1.x();
    int deltaY = p2.y()-p1.y();

    if(deltaX==0 && deltaY==-1)
        return 0;
    if(deltaX==1 && deltaY==-1)
        return 45;
    if(deltaX==1 && deltaY==0)
        return 90;
    if(deltaX==1 && deltaY==1)
        return 135;
    if(deltaX==0 && deltaY==1)
        return 180;
    if(deltaX==-1 && deltaY==1)
        return 225;
    if(deltaX==-1 && deltaY==0)
        return 270;
    if(deltaX==-1 && deltaY==-1)
        return 315;

    return 0;
}

void Dashboard::DisableUI()
{
    ui->pb_explore->setEnabled(false);
    ui->rb_startpoint->setEnabled(false);
    ui->rb_endpoint->setEnabled(false);
    ui->hs_uavspeed->setEnabled(false);
    ui->hs_uavmargin->setEnabled(false);
}

void Dashboard::ResetUI()
{
    animation_timer.stop();
    if(ui->rb_startpoint->isChecked())
        view->SetStartPoint();
    else
        view->SetEndPoint();

    ui->pb_explore->setEnabled(true);
    ui->rb_startpoint->setEnabled(true);
    ui->rb_endpoint->setEnabled(true);
    ui->hs_uavspeed->setEnabled(true);
    ui->hs_uavmargin->setEnabled(true);
}











