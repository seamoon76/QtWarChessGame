// myitem.cpp
// 实现MyItem类

#include "myitem.h"
#include<QDebug>
#include<QToolTip>
#include<QResource>
#include<QGraphicsSceneHoverEvent>
MyItem::MyItem(int type,int itemID)
{
    setAcceptHoverEvents(true);
    itemType=type;
    m_itemID=itemID;
    if(type==MyItem::CementFloor)
    {
        imagePath=":/new/prefix1/image/map/floor.png";
        terrainName=QString("地板");
        m_info=QString("普通地面，无特殊效果");
    }
    else if(type==MyItem::WoodenFloor)// 木质地形,可着火形成阻碍
    {
        imagePath=":/new/prefix1/image/map/woodenFloor.png";
        terrainName=QString("木质地板");
        m_info=QString("木质地板,可被点燃从而形成阻碍");
    }
    else if(type==MyItem::Obstacle)// 阻碍型地形
    {
        imagePath=":/new/prefix1/image/map/obstacle2.png";
        terrainName=QString("障碍物");
        m_info=QString("阻碍型地形，可阻碍角色移动和攻击，但不能阻挡飞行物");
    }
    else if(type==MyItem::Water)// 损伤型地形
    {
        imagePath=":/new/prefix1/image/map/water.png";
        terrainName=QString("水池");
        m_info=QString("角色在其中每移动一格或停留一回合都会损失10HP");
    }
    else if(type==MyItem::Swamp)// 妨碍型地形
    {
        imagePath=":/new/prefix1/image/map/swamp.png";
        terrainName=QString("沼泽");
        m_info=QString("角色在其中最大移动距离减1");
    }
    else if(type==MyItem::Tree)// 可摧毁的障碍物
    {
        imagePath=":/new/prefix1/image/map/tree.png";
        terrainName=QString("树");
        m_info=QString("阻碍移动，可被摧毁");
    }
    else if(type==MyItem::Soldier)
    {
        // 默认战士robot4
        imagePath=":/new/prefix1/image/robot/robot4-hit0.png";
    }
    else if(type==MyItem::monsterType)
    {
        imagePath=":/new/prefix1/image/alien/alien1-hit0.png";
    }
    if(!image.load(imagePath))
    {
        qDebug() << "image load failed";
        return;
    }
}

QRectF MyItem::boundingRect()const
{
     qreal penwidth=1;
    return QRectF(-myconstant::ITEM_WIDTH/2+penwidth/2,-myconstant::ITEM_HEIGHT/2+penwidth/2,
                  myconstant::ITEM_WIDTH-penwidth/2,myconstant::ITEM_HEIGHT-penwidth/2);
}


void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec=boundingRect();
    painter->save();
    painter->setPen(QPen(Qt::black,1,Qt::DotLine));
    if(itemType!=MyItem::Soldier&&itemType!=MyItem::monsterType)
    {
        painter->drawRect(rec);
    }
    painter->drawImage(rec,image);
    QRectF newRec=QRectF(rec.x()+2,rec.y()+2,rec.width()-4,rec.height()-4);
    if(greenState)
    {
        painter->setBrush(QColor(147,252,100,200));
        painter->drawRect(newRec);
    }
    if(redState)
    {
    painter->setPen(QPen(Qt::red,2));
    painter->drawRect(newRec);
    }
    painter->restore();
}

void MyItem::showGreen()
{
    // 这个图元是人或者障碍物或者有人，不能变绿
    if(this->itemType==Soldier||this->itemType==monsterType||this->itemType==Obstacle||haveRole)
    {
        return;
    }
    else// 是可达地形，可以变绿
    {
        greenState=true;
        update();
    }
}

void MyItem::hideGreen()
{
    greenState=false;
    update();
}

void MyItem::focusLeave()
{
    qDebug()<<"focusOut!";
    greenState=0;
    redState=0;
    update();
}

// 每回合重置
void MyItem::ResetEveryRound()
{
    greenState=0;
    redState=0;
    waitToAttack=false;
    waitToMove=false;
}

// 鼠标悬浮事件，显示单元的详细信息到右侧信息框
void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    if(itemType==MyItem::Soldier||itemType==MyItem::monsterType)
    {
        emit MyItem::needToShowInfo(pos());
    }
    // 增加特殊地形的信息
    else if(itemType==MyItem::CementFloor
            ||itemType==MyItem::Obstacle||itemType==MyItem::Water
            ||itemType==MyItem::Tree||itemType==MyItem::WoodenFloor||itemType==MyItem::Swamp)
    {
        emit MyItem::needToShowInfo(pos());
    }
    QGraphicsObject::hoverEnterEvent(event);
}

void MyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // emit MyItem::stopShowInfo();
    QGraphicsObject::hoverLeaveEvent(event);
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        // 如果地形是可移动范围
        if(itemType==MyItem::CementFloor
                ||itemType==MyItem::WoodenFloor
                ||itemType==MyItem::Water
                ||itemType==MyItem::Swamp)
        {
            if(greenState==true)//这个地形在某人的移动范围内
            {
                emit thisIsWalkDestination(this);
            }
        }
    }
    else if(event->button()==Qt::LeftButton)
    {
        if(itemType!=MyItem::Soldier
                &&itemType!=monsterType
                &&itemType!=effect)
        {
            emit clearLastOperatorMemory();
        }
    }

}
