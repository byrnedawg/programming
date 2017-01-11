#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"cv.h"
#include"cxcore.h"
#include"highgui.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::run_the_code()
{
    IplImage *myimage;
    CvCapture *thewebcam=0;
     thewebcam=cvCaptureFromCAM(0);
cvSetCaptureProperty(thewebcam,CV_CAP_PROP_FRAME_WIDTH,360);
cvSetCaptureProperty(thewebcam,CV_CAP_PROP_FRAME_HEIGHT,240);
//cvSetCaptureProperty(thewebcam,CV_CAP_PROP_FRAME_COUNT,30);
    if(thewebcam==0)
        qDebug()<<"web cam not found";
       else
        qDebug()<<"*********************web cam found";
    int i=400;
    cvNamedWindow("video",CV_WINDOW_AUTOSIZE);
    while(i>0)
    {
    myimage=cvQueryFrame(thewebcam);
    cvShowImage("video",myimage);
    cvWaitKey(10);
   i--;
    //if(myimage!=0)
    //qDebug()<<i<<myimage->width<<myimage->height;
}
    cvReleaseCapture(&thewebcam);
    }

void MainWindow::on_start_button_clicked()
{
    qDebug()<<"came here you clicked the button dude";
   run_the_code();
}
