#include <QFileDialog>
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

    connect(ui->pb_openimage,SIGNAL(clicked()),this,SLOT(slotOpenImage()));
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::slotOpenImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.isEmpty())
        return;

    QImage *img = new QImage;
    if(img==nullptr)
        return;

    img->load(fileName);

    storm_images.push_back(img);
    if(view!=nullptr)
        view->Update(*img);
}

void Dashboard::slotExplore()
{

}

void Dashboard::slotStop()
{

}

void Dashboard::slotClose()
{

}
