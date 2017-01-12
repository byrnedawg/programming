#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<wiringPi.h>
#include"lcd_pi.h"
#include<QProcess>
#include<QDir>
#define LED_PIN_NUMBER 25
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

        QString path;

        path=QDir::currentPath()+"/export_gpio.sh";
        qDebug()<<path;
        QProcess *the_process= new QProcess();
        the_process->start(path);
        the_process->waitForFinished(-1);
        if(the_process->exitCode()!=0)
        {
            qDebug()<<"error"<<the_process->readAllStandardError();
        }
        else
        {
        qDebug()<<"ok"<<the_process->readAllStandardOutput();
        }


        if(wiringPiSetupSys()==-1)
            qDebug()<<"WIRING PI SETUP FAILED";
        else
            qDebug()<<"WIRING PI SETUP DONE";
            lcd_init();
            //INITALLY SWITCH OFF THE LED
            digitalWrite(LED_PIN_NUMBER,1);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(send_to_lcd_button_cicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::send_to_lcd_button_cicked()
{
   QString themsg;
const char* lcd_msg;
           themsg=ui->plainTextEdit->toPlainText();

qDebug()<<themsg;
lcd_msg=themsg.toStdString().c_str();

lcd_string(lcd_msg);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
    lcd_cmd(0x01);
    delay(150);
}

void MainWindow::on_led_button_clicked()
{
    unsigned char status;
   status=ui->led_button->isChecked();
   if(status==true)
   {
       qDebug()<<"on the led";
       digitalWrite(LED_PIN_NUMBER,0);

   }
   else
   {
       qDebug()<<"off the led";
        digitalWrite(LED_PIN_NUMBER,1);
   }
}
