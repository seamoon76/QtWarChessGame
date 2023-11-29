#include "mytip.h"
#include "ui_mytip.h"
#include <QDebug>

MyTip::MyTip(QString content,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyTip)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint
                         |Qt::Tool
                         |Qt::WindowStaysOnBottomHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    // qDebug()<<"content is "<<content;
    ui->label->setText(content);
    ui->label->raise();
    ui->label->resize(500,300);
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->label->setAlignment(Qt::AlignVCenter);
    ui->label->setStyleSheet(QString
           ("border-image: url(:/new/prefix1/image/background/message.png);color: rgb(255, 255, 255);font: 75 12pt \"Microsoft YaHei UI\";"));
    QTimer::singleShot(4000,this,[=](){
        ui->label->lower();
        this->close();
    });


}

MyTip::~MyTip()
{
    delete ui;
}
