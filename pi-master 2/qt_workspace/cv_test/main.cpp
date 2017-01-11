#include <QApplication>
#include "mainwindow.h"
#include"cv.h"
#include"cxcore.h"
#include"highgui.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     /*thewebcam=cvCaptureFromCAM(0);
    if(thewebcam==0)
        qDebug()<<"web cam not found";
       else
        qDebug()<<"*********************web cam found";*/
    /*if(thewebcam!=0)
    myimage=cvQueryFrame(thewebcam);
*/
   /* myimage=cvLoadImage("myimage.jpg");
    cvShowImage("theimage",myimage);*/
    MainWindow w;
    w.show();


    return a.exec();
}
