#include "mymessagebox.h"

MyMessageBox::MyMessageBox(const QString &title, const QString &text,QWidget* parent)
{
    setWindowTitle(title);
    setText(text);
    setParent(parent);
    setStandardButtons(QMessageBox::Ok);
    // setStyleSheet(QString
    //              ("QLabel{background-color: rgb(0, 0, 0);}background-image: url(:/new/prefix1/image/background/message.png);color: rgb(255, 255, 255);font: 75 12pt \"Microsoft YaHei UI\";"));

}

void MyMessageBox::setSize(int width, int height)
{
    m_wight=width;
    m_height=height;

}

void MyMessageBox::resizeEvent(QResizeEvent* event)
{
    setFixedSize(m_wight,m_height);
}
