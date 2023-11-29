#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// MainWindow类
// 主窗口类，实现了游戏标题和开始界面，
// 从主窗口可以进入选关界面，或者进入游戏简介、操作说明等界面
// 马骐

#include <QMainWindow>
#include <QSound>
#include <QDialog>
#include "myconstant.h"
#include "selectlevelwindow.h"
#include "mytip.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSound* titleSound=nullptr;


private slots:
    // “开始游戏”键
    void on_pushButton_clicked();

    // 结束游戏键
    void on_pushButton_2_clicked();

    // 从选关窗口返回后执行的内容
    void backFormSelectWindow();

    // 弹出游戏简介窗口
    void on_intropushButton_clicked();

    // 弹出操作说明窗口
    void on_operapushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    SelectLevelWindow * m_pSelectLevelWindow=nullptr;
    QDialog* dialogForIntroGame=nullptr;
    QDialog* dialogForTeachOperate=nullptr;
    // 自定义的提示框
    MyTip* p_tip=nullptr;
};
#endif // MAINWINDOW_H
