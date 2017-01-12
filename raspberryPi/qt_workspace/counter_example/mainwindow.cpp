#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->mybutton,SIGNAL(clicked()),this,SLOT(on_button_click()));
    count=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_click()
{
    qDebug()<<"button pressed dude";
count++;
ui->display_lable->setText(QString::number(count));
}
