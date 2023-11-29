#ifndef MYITEM_H
#define MYITEM_H

// 自定义的图元类MyItem
// 继承自QGraphicsObject
// 是战棋地图的基本单元，可以放地形或者人物或者效果动画
// 马骐

#include "myconstant.h"
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>

class MyItem:public QGraphicsObject
{
    Q_OBJECT
public:
    MyItem(const int type=MyItem::CementFloor,int itemID=0);

    QRectF boundingRect() const override;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem * option,
               QWidget* widget)override;
    // 将地形画为绿色
    void showGreen();
    // 将绿色清除
    void hideGreen();

    virtual void ResetEveryRound();// 用于每回合重新修改某些参数，比如可移动状态

    // 鼠标悬浮在上方，则在窗口右侧显示地形或者战士的详细信息
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;

    // 处理鼠标点击在这个单元的事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;

 public:
    // 单元的类型
    enum type
    {   Soldier,       // 战士
        monsterType,   // 怪物
        CementFloor,   // 普通地面
        WoodenFloor,   // 木质地板，可被引燃
        Obstacle,      // 障碍物
        Water,         // 水
        Swamp,         // 沼泽
        Tree,          // 树
        effect};       // 动画效果播放器

    int m_itemID;
    int itemType{CementFloor};  // 是哪一种类型，默认是Floor
    QString terrainName="floor";// 地形名称，比如地板、水
    QString m_info="";          // 单元的详细信息
    bool haveRole{0};           // 地形上是否有人
    int greenState{0};          // 这个单元是否被标记为绿色，代表是否可以成为移动的目的地
    int redState{0};            // 这个单元是否被标记为红色，代表是否可以成为攻击的目标
    QString imagePath;          // 这个单元的图片的路径
    QImage image;
    bool waitToAttack=false;
    bool waitToMove=false;
    int disToSearchingItem=1000;// 需要每次查询距离的时候都清空，这个变量会用在寻路函数中
    bool haveBeenAddedIntoReachableList=false;// 同上

signals:
    void needToShowInfo(QPointF);  // 需要显示信息到右侧信息栏
    void stopShowInfo();           // 停止在右侧显示信息
    void thisIsWalkDestination(MyItem* destination);// 这个单元是移动目的地
    void clearLastOperatorMemory();                 // 清空“操作战士”的选中

public slots:
    void focusLeave();// 这个函数已不再使用
};

#endif // MYITEM_H
