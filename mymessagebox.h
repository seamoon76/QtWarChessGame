#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

// 自定义的消息框，暂时不再使用
// 马骐

#include <QMessageBox>
#include <QResizeEvent>

class MyMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    MyMessageBox(const QString &title, const QString &text,QWidget* parent=nullptr);
    void setSize(int width,int height);
    void resizeEvent(QResizeEvent* event)override;

private:
    int m_wight{500};
    int m_height{300};
};

#endif // MYMESSAGEBOX_H
