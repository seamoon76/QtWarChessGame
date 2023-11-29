#ifndef ITEMFORROLE_H
#define ITEMFORROLE_H

// ItemForRole 角色图元类
// 用于战士或者怪物的图元，继承自MyItem类(自定义的图元类)
// 马骐

#include "myitem.h"
#include "isoldier.h"
#include "monster.h"
#include "itemforeffect.h"
#include <QMovie>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>

// 用于在地图上显示人物角色的类，包括战士和怪物
class ItemForRole : public MyItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    bool soldierOrMonster;                  // 区分是战士还是怪物，0代表战士，1代表怪物
    iSoldier m_Soldier;
    Monster  m_Monster;    
    MyItem* tmpItem=nullptr;
    QList<QGraphicsItem*> itemListCanMoveTo;// 记录这个角色当前可以移动到的位置
    QList<QGraphicsItem*> attackableList;   // 记录这个角色当前可以攻击的目标
    QMovie* m_Movie=nullptr;                // 播放动画的指针
    QMovie* walkMovie=nullptr;              // 走路的动画
    QMovie* attackMovie=nullptr;            // 攻击的动画
    QMovie* dieMovie=nullptr;               // 死亡的动画
    // QMovie* hurtMovie=nullptr;
    QPropertyAnimation* animWalk=nullptr;   // 位移动画
    QString walkGIfPath="";
    QString imageGreyPath="";               // 灰色图案路径，如果角色回合结束，就变灰
    bool roleIsDead=false;
    // 表示被选中要被玩家进行操作的角色，在一场游戏中，只允许最多有一个角色该值为真
    static ItemForRole* onlyRoleToOperate;
    // 点击菜单按钮前的状态，用于如果菜单不可选就恢复，
    // 默认设置为dead状态防止误操作
    int stateBeforeMenuChoosed;

public:
    // 参数Typepara,0代表战士，1代表怪物,对应于MyItem的Type枚举量
    // 参数RoleType: robot4,robot6,alien1,可以用这个量具体设置战士和怪物的类别
    ItemForRole(const int Typepara,int XCoordination,int YCoordination,const QString RoleType="robot4",const int level=1);
    virtual ~ItemForRole(){};
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem * option,
               QWidget* widget)override;

    void ResetEveryRound()override;

    void setMovie(QMovie* movie);

    QPointF pos();

    void setPos(const QPointF &pos);

    // 角色移动的函数，移动到指定位置，并使用参数effectPlayer播放移动动画
    void walkToTarget(QPointF target,ItemForEffect* effectPlayer);

    // 角色死亡，播放死亡动画
    void roleDie();

    // 右键弹出菜单，已废弃该函数
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)override;

    // 寻找可攻击对象
    // 可攻击对象会被存储在成员变量attackableList里，同时会在屏幕上把这些方格标红
    void searchAttackableItem();

    // 寻找可到达目的地
    // 可到达目的地会被存储在成员变量itemListCanMoveTo里，同时会把屏幕上这些方格标绿
    void searchReachabkeItem();

    // 计算两个点之间的距离，不是欧氏距离，而是横坐标差的绝对值+纵坐标差的绝对值
    double distance(QPointF a,QPointF b);

public slots:
    // 攻击指定目标target，并使用参数effectPlayer播放攻击动画
    void attack(ItemForRole* target,ItemForEffect* effectPlayer);

    // 清除周围的红色方格和绿色的颜色
    void removeColorNearby();

signals:
    // 这个角色将被攻击的信号，用于让用户用右键选择攻击对象后触发
    void thisRoleWIllBeAttacked(ItemForRole* attacker,ItemForRole* Target);

    // 让全局清空“要操作者”的可移动列表、可攻击列表、颜色、准备攻击状态
    void clearLastOperatorMemory();
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QMetaObject::Connection mConnection;
};

#endif // ITEMFORROLE_H
