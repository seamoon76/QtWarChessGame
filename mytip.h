#ifndef MYTIP_H
#define MYTIP_H

// 自定义的消息提示框
// 目前不再使用
// 马骐

#include <QWidget>
#include<QTimer>
#include<QString>

namespace Ui {
class MyTip;
}

class MyTip : public QWidget
{
    Q_OBJECT

public:
    explicit MyTip(QString content="",QWidget *parent = nullptr);
    ~MyTip();

private:
    Ui::MyTip *ui;
};

#endif // MYTIP_H
