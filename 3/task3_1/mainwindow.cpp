#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   isPicOnLabel=false;
//   myplayer = new vplayer;
//   connect(myplayer,SIGNAL(sigGetOneFrame(QImage)),this,
//           SLOT(slotGetOneFrame(QImage)));
//   connect(myplayer,SIGNAL(sigEndOfFrame()),this,SLOT(slotEndOfFrame()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myplayer;
}


void MainWindow::on_pushButton_clicked()
{

    if(!QString::compare(ui->pushButton->text(),QString::fromUtf8("play"))){
        ShowPic();
    }
    else if(!QString::compare(ui->pushButton->text(),QString::fromUtf8("pause"))){
        PausePic();
    }
    else if(!QString::compare(ui->pushButton->text(),QString::fromUtf8("resume"))){
        ResumePic();
    }
}

void MainWindow::slotGetOneFrame(QImage img){
    mImage=img.copy();
    if(isPicOnLabel==false)
        return ;
    ui->label->setPixmap(QPixmap::fromImage(mImage).scaled(ui->label->size()));
}

void MainWindow::slotEndOfFrame(){
    isPicOnLabel=true;
    ui->pushButton->setText("play");
    disconnect(myplayer,SIGNAL(sigGetOneFrame(QImage)),this,
            SLOT(slotGetOneFrame(QImage)));
    disconnect(myplayer,SIGNAL(sigEndOfFrame()),this,SLOT(slotEndOfFrame()));
    myplayer->uninit();
}

void MainWindow::ShowPic(){

    myplayer = new vplayer;
    connect(myplayer,SIGNAL(sigGetOneFrame(QImage)),this,
            SLOT(slotGetOneFrame(QImage)));
    connect(myplayer,SIGNAL(sigEndOfFrame()),this,SLOT(slotEndOfFrame()));
    isPicOnLabel=true;
    ui->pushButton->setText("pause");
    myplayer->videoStart();
}

void MainWindow::PausePic(){
    isPicOnLabel=true;
    ui->pushButton->setText("resume");
    myplayer->videoPause();
}

void MainWindow::ResumePic(){
    isPicOnLabel=true;
    ui->pushButton->setText("pause");
    myplayer->videoResume();
}

void MainWindow::StopPic(){
    disconnect(myplayer,SIGNAL(sigGetOneFrame(QImage)),this,
            SLOT(slotGetOneFrame(QImage)));
    disconnect(myplayer,SIGNAL(sigEndOfFrame()),this,SLOT(slotEndOfFrame()));

}

void MainWindow::on_pushButton_2_clicked()
{
   if(isPicOnLabel){
        isPicOnLabel=false;
        ui->pushButton->setText("play");
        StopPic();

    }
}
