#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mytip.h"
#include<QDialog>
#include<QTimer>
#include<QIcon>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // setFixedSize(myconstant::WINDOW_WIDTH,myconstant::WINDOW_HEIGHT);
    ui->setupUi(this);
    titleSound=new QSound(":/music/music/bgmusic/mus_title_A.wav");
    titleSound->play();
    setWindowIcon(QIcon(":/new/prefix1/icon.ico"));
//    setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
//    setWindowTitle(TITLE_OF_GAME);
// scene firstScene(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 开始游戏的按钮被点击
void MainWindow::on_pushButton_clicked()
{
    m_pSelectLevelWindow=new SelectLevelWindow();
    QTimer::singleShot(1000, this, [=](){
        this->hide();
        this->titleSound->stop();
    });

    m_pSelectLevelWindow->setGeometry(this->geometry());
    m_pSelectLevelWindow->show();

    connect(m_pSelectLevelWindow,SIGNAL(backButtonClicked()),
            this,SLOT(backFormSelectWindow()));
}

// 退出游戏的按钮被点击
void MainWindow::on_pushButton_2_clicked()
{
    QTimer::singleShot(300, this, SLOT(close()));
}

// 从选关界面返回
void MainWindow::backFormSelectWindow()
{
    m_pSelectLevelWindow->hide();
    m_pSelectLevelWindow->introSound->stop();

    setGeometry(m_pSelectLevelWindow->geometry());
    delete m_pSelectLevelWindow;
    m_pSelectLevelWindow = NULL;
    this->show();
    this->titleSound->play();
}

// 游戏简介的窗口被点击
void MainWindow::on_intropushButton_clicked()
{
    dialogForIntroGame=new QDialog;
    dialogForIntroGame->setWindowTitle("game introduction");
    dialogForIntroGame->setStyleSheet(QString("border-image: url(:/new/prefix1/image/background/intro2.png);"));
    dialogForIntroGame->resize(1600,800);
    dialogForIntroGame->show();
    dialogForIntroGame->exec();
}

void MainWindow::on_operapushButton_3_clicked()
{
    dialogForTeachOperate=new QDialog;
    dialogForTeachOperate->setWindowTitle("Instructions");
    dialogForTeachOperate->setStyleSheet(QString("border-image: url(:/new/prefix1/image/background/intro1.png);"));
    dialogForTeachOperate->resize(1600,800);
    dialogForTeachOperate->show();
    dialogForTeachOperate->exec();
}
