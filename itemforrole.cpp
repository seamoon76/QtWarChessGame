#include "itemforrole.h"
#include"itemforeffect.h"
#include<QMenu>
#include<QGraphicsItemAnimation>
#include <QGraphicsSceneMouseEvent>
#include<QDebug>
#include<QGraphicsScene>
#include<QTimer>
#include<queue>
using std::queue;

ItemForRole::ItemForRole(const int Typepara,int XCoordination,
                         int YCoordination,const QString RoleType,
                         const int level)
    :MyItem(Typepara)
{

    setPos(QPointF(XCoordination*myconstant::ITEM_WIDTH,
                   YCoordination*myconstant::ITEM_HEIGHT));

    // 为不同角色种类设置具体信息
    if(Typepara==MyItem::Soldier)// 角色为战士
    {
        stateBeforeMenuChoosed=iSoldier::dead;
        imageGreyPath=":/new/prefix1/image/robot/"+RoleType+"-grey.png";
        setFlag(ItemIsSelectable);
        soldierOrMonster=0;//0 for soldier
        QString Prefix=":/new/prefix1/image/robot/";
        walkMovie=new QMovie(QString(Prefix+RoleType+"-walk.gif"));
        attackMovie=new QMovie(QString(Prefix+RoleType+"-attack.gif"));
        dieMovie=new QMovie(QString(Prefix+RoleType+"-die.gif"));
        // hurtMovie=new QMovie(QString(Prefix+RoleType+"-hurt.gif"));

        if(RoleType=="robot4")// 狙击手 戴维斯
        {
            imagePath=":/new/prefix1/image/robot/robot4-hit0.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,4,35,5,XCoordination,YCoordination,15,3,2,level,3);
            m_Soldier.setName(QString("狙击手 戴维斯"));
            m_Soldier.m_skill=iSoldier::improveDefense;
            m_info=QString("攻击力:15,防御力:3\n移动距离:2,攻击距离:3\n技能【提升】:接下来两回合内防御力加倍，消耗2MP");
        }
        else if(RoleType=="robot6")// 突击手 泰勒
        {
            imagePath=":/new/prefix1/image/robot/robot_6-hit0.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,6,50,5,XCoordination,YCoordination,10,3,2,level,1);
            m_Soldier.setName(QString("突击手 泰勒"));
            m_Soldier.m_skill=iSoldier::improveDefense;
            m_info=QString("攻击力:10,防御力:3\n移动距离:2,攻击距离:1\n技能【提升】:接下来两回合内防御力加倍,消耗2MP");
        }
        else if(RoleType=="robot7")// 机甲战士 史密斯
        {
            imagePath=":/new/prefix1/image/robot/robot_7-walk0.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,7,50,5,XCoordination,YCoordination,10,3,3,level,2);
            m_Soldier.setName(QString("机甲战士 史密斯"));
            m_Soldier.m_skill=iSoldier::hitHard;
            m_info=QString("攻击力:10，防御力:3\n移动距离:3,攻击距离:2\n技能【重击】:接下来两回合内攻击力加倍,消耗3MP");

        }
        else if(RoleType=="robot8")// 喷火兵 埃文斯
        {
            imagePath=":/new/prefix1/image/robot/robot_8-walk0.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,8,50,5,XCoordination,YCoordination,10,3,2,level,1);
            m_Soldier.setName(QString("喷火兵 埃文斯"));
            m_Soldier.m_skill=iSoldier::hitHard;
            m_info=QString("攻击力:10,防御力:3\n移动距离:2,攻击距离:1\n技能【重击】:接下来两回合内攻击力加倍,消耗3MP");

        }
        else if(RoleType=="robot9")// 重炮兵 约翰
        {
            imagePath=":/new/prefix1/image/robot/robot_9-walk0v.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,9,40,5,XCoordination,YCoordination,20,5,1,level,3);
            m_Soldier.setName(QString("重炮兵 约翰"));
            m_Soldier.m_skill=iSoldier::quickMove;
            m_info=QString("攻击可越过阻碍,攻击力:20,防御力:5\n移动距离:1,攻击距离:3\n技能【速移】:接下来两回合内移动距离+1,消耗2MP");

        }
        else if(RoleType=="robot10")// 侦察兵 威尔逊
        {
            imagePath=":/new/prefix1/image/robot/robot10-attack0v.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,10,40,5,XCoordination,YCoordination,10,3,4,level,1);
            m_Soldier.setName(QString("侦察兵 威尔逊"));
            m_Soldier.m_skill=iSoldier::improveDefense;
            m_info=QString("机动性强,攻击力:10,防御力:3\n移动距离:4,攻击距离:1\n技能【提升】:接下来两回合内防御力加倍,消耗2MP");

        }
        else if(RoleType=="robot12")// 支援型无人机 布朗
        {
            imagePath=":/new/prefix1/image/robot/robot12-fly0v.png";
            image.load(imagePath);
            m_Soldier.setSoldier(RoleType,12,35,5,XCoordination,YCoordination,15,3,3,level,2);
            m_Soldier.setName(QString("支援型无人机 布朗"));
            m_Soldier.m_skill=iSoldier::support;
            m_info=QString("攻击力:15,防御力:3\n移动距离:3,攻击距离:2\n技能【支援】:为攻击范围内所有我方战士恢复HP30，消耗3MP");
        }

    }
    else if(Typepara==MyItem::monsterType)// 角色是怪物
    {
        stateBeforeMenuChoosed=Monster::dead;
        soldierOrMonster=1;//1 for monster
        QString Prefix=":/new/prefix1/image/alien/";
        walkMovie=new QMovie(QString(Prefix+RoleType+"-walk.gif"));
        attackMovie=new QMovie(QString(Prefix+RoleType+"-attack.gif"));
        dieMovie=new QMovie(QString(Prefix+RoleType+"-die.gif"));
        // hurtMovie=new QMovie(QString(Prefix+RoleType+"-hurt.gif"));
        if(RoleType=="alien1")//   巨兽
        {
            imagePath=":/new/prefix1/image/alien/alien1-hit0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,1,35,5,XCoordination,YCoordination,10,2,2,1,1);
            m_Monster.setName(QString("巨兽"));
            m_info=QString("攻击力：10,防御力：2\n移动距离:2,攻击距离:1\n技能：\n无");

        }
        else if(RoleType=="alien3") // 巨蝎
        {
            imagePath=":/new/prefix1/image/alien/alien_3-walk0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,3,20,5,XCoordination,YCoordination,15,3,3,1,1);
            m_Monster.setName(QString("巨蝎"));
            m_info=QString("攻击力：15,防御力：3\n移动距离:3,攻击距离:1\n技能：\n无");

        }
        else if(RoleType=="alien5")// 蝙蝠
        {
            imagePath=":/new/prefix1/image/alien/alien_5-fly3.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,5,25,5,XCoordination,YCoordination,10,3,4,1,1);
            m_Monster.setName(QString("蝙蝠"));
            m_info=QString("移动范围大，攻击力：10，防御力：3\n移动距离:4,攻击距离:1\n技能：\n无");

        }
        else if(RoleType=="alien6")// 怪龙
        {
            imagePath=":/new/prefix1/image/alien/alien_6-walk0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,6,40,5,XCoordination,YCoordination,10,3,2,1,1);
            m_Monster.setName(QString("怪龙"));
            m_info=QString("生命值高，攻击力：10，防御力：3\n移动距离:2,攻击距离:1\n技能：\n无");

        }
        else if(RoleType=="alien7")// 四足虫
        {
            imagePath=":/new/prefix1/image/alien/alien_7-walk0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,7,30,5,XCoordination,YCoordination,10,3,3,1,1);
            m_Monster.setName(QString("四足虫"));
            m_info=QString("攻击力：10，防御力：3\n移动距离:3,攻击距离:1\n技能：\n无");
        }
        else if(RoleType=="alien8")// 怪蜴，近程精英怪
        {
            imagePath=":/new/prefix1/image/alien/alien_8-walk0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,8,40,5,XCoordination,YCoordination,20,3,3,1,1);
            m_Monster.setName(QString("怪蜴"));
            m_info=QString("攻击力：20，防御力：3\n移动距离:3,攻击距离:1\n技能：\n无");

        }
        else if(RoleType=="alien9")// boss 猩猩
        {
            imagePath=":/new/prefix1/image/alien/alien_9-walk0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,9,100,10,XCoordination,YCoordination,20,5,2,1,1);
            m_Monster.setName(QString("boss 巨猩"));
            m_info=QString("攻击力:20,防御力:5\n移动距离:2,攻击距离:1\n技能:\nHP<50会触发一次对所有战士的10HP伤害，HP<40时移动力+1,技能仅使用一次");

        }
        else if(RoleType=="alien10")// 飞虫
        {
            imagePath=":/new/prefix1/image/alien/alien_10-attack0.png";
            image.load(imagePath);
            m_Monster.setMonster(RoleType,25,40,5,XCoordination,YCoordination,20,3,3,1,1);
            m_Monster.setName(QString("飞虫"));
            m_info=QString("移动范围大，攻击力：20，防御力：3\n移动距离:3,攻击距离:1\n技能：\n无");
        }

    }
    else
    {
        qDebug()<<"this type of "<<RoleType<<"has not been defined.";
    }
    animWalk=new QPropertyAnimation(this, "pos");
}

// 计算两点间距离。不是欧氏距离，而是横坐标差的绝对值+纵坐标差的绝对值
double ItemForRole::distance(QPointF a,QPointF b)
{
    return abs(a.x()-b.x())+abs(a.y()-b.y());
}

QPointF ItemForRole::pos()
{
    return QGraphicsItem::pos();
}

void ItemForRole::setPos(const QPointF &pos)
{
    if(this->pos()!=pos){
    QGraphicsItem::setPos(pos);
    update();
    }
}

void ItemForRole::paint(QPainter* painter,
                const QStyleOptionGraphicsItem * option,
                QWidget* widget)
{
    // 根据状态执行需要补充绘画的地方
    // MyItem::paint(painter,option,widget);

    // 判断是否需要绘制动画
    if(m_Movie && m_Movie->state() == QMovie::Running)
    {
        if (m_Movie)
        {
            painter->drawPixmap(boundingRect(), m_Movie->currentPixmap(),
                                m_Movie->frameRect());
        }
    }
    // 如果这个角色的回合结束就把它变灰
    else if(itemType==MyItem::Soldier&&m_Soldier.state==iSoldier::cannotSelected)
    {
        if(!image.load(imageGreyPath))
        {
            qDebug()<<"can not find image of "<<imageGreyPath;
        }
        MyItem::paint(painter,option,widget);
    }
    else
    {
        MyItem::paint(painter,option,widget);
    }

    // 如果这个角色是“当前操作者”，为它画一个黄颜色边框
    if(this==ItemForRole::onlyRoleToOperate)
    {
        QRectF rec=boundingRect();
        painter->save();
        painter->setPen(QPen(Qt::yellow,1));
        if(itemType==MyItem::Soldier)
        {
            painter->drawRect(rec);
        }
        painter->restore();
    }

    // 绘制血量条
    QBrush greenBrush(QColor("#5DED5A"));
    painter->setBrush(greenBrush);
    int nowBlood=0;
    int maxBlood=0;
    if(soldierOrMonster)// 怪物
    {
        nowBlood=m_Monster.getHP();
        maxBlood=m_Monster.m_HPUpperLimit;
    }
    else
    {
        nowBlood=m_Soldier.getHP();
        maxBlood=m_Soldier.m_HPUpperLimit;
    }
    double bloodRate=1.0*nowBlood/maxBlood;
    QRectF rec=boundingRect();
    painter->drawRect(rec.x(),rec.y()-rec.height()*0.4,rec.width()*bloodRate,rec.height()*0.2);

}

void ItemForRole::searchAttackableItem()
{
    if(soldierOrMonster)// 如果是怪物
    {
        // 根据怪物状态，如果不能攻击就返回
        if(m_Monster.state==Monster::cannotSelected
                ||m_Monster.state==Monster::dead)
        {
            return;
        }
        QPointF posNowItem=this->pos();
        int maxAttack=m_Monster.m_MaximumAttackRange;
        attackableList.clear();
        // 把攻击周围内的item加入列表
        attackableList.append(
                scene()->items(posNowItem.x()-maxAttack*myconstant::ITEM_WIDTH,
                               posNowItem.y()-maxAttack*myconstant::ITEM_HEIGHT,
                               2*maxAttack*myconstant::ITEM_WIDTH,
                               2*maxAttack*myconstant::ITEM_HEIGHT,
                               Qt::IntersectsItemShape,Qt::DescendingOrder)
                    );
        foreach(QGraphicsItem* i,attackableList)
        {
            MyItem* p_attackableItem=static_cast<MyItem*>(i);
            if(p_attackableItem->pos()!=this->pos())// 不是自己
            {
            if(p_attackableItem->redState==0)
            {
                p_attackableItem->redState=1;
                p_attackableItem->update();
            }
            }
            else// 如果item是自己，去掉它
            {
                attackableList.removeOne(i);
            }
        }
    }
    else // 是战士
    {
        if(m_Soldier.state==iSoldier::cannotSelected
                ||m_Soldier.state==iSoldier::dead)
        {
            return;
        }
        QPointF posNowItem=this->pos();
        int maxAttack=m_Soldier.m_MaximumAttackRange;
        attackableList.clear();

        if(m_Soldier.getID()==4
                ||m_Soldier.getID()==7
                ||m_Soldier.getID()==12)// 子弹类只能按照直线攻击
        {
            int maxStep=m_Soldier.m_MaximumAttackRange;
            QPointF posNowItem=this->pos();

            // 把当前位置为中心的十字的图元加入列表
            // 左侧
            QList<QGraphicsItem*> tmpItemList;
            QPointF tmpPointF=posNowItem;
            for(int i=1;i<=maxStep;i++)
            {
                bool canPass=true;// 当前方向没有遇到阻挡
                tmpPointF.setX(posNowItem.x()-i*myconstant::ITEM_WIDTH);
                // tmpPointF.setY(posNowItem.y()-i*myconstant::ITEM_HEIGHT);
                tmpItemList=scene()->items(tmpPointF);
                foreach(QGraphicsItem* j,tmpItemList)
                {
                    MyItem* p_myItem=static_cast<MyItem*>(j);
                    {
                        if(p_myItem->itemType==Soldier||p_myItem->itemType==monsterType)// 如果列表有人就在这里终止
                        {
                            canPass=false;
                            attackableList.append(tmpItemList);//但是这一点可以攻击
                            break;
                        }
                        else if(p_myItem->itemType==Obstacle
                                ||p_myItem->itemType==Tree)//不能通过也不能攻击
                        {
                            canPass=false;
                            break;
                        }
                        else
                        {
                            attackableList.append(tmpItemList);
                        }
                    }
                }
                if(!canPass)
                {
                    break;
                }

                tmpItemList.clear();
            }

            // 右侧
            tmpPointF=posNowItem;
            for(int i=1;i<=maxStep;i++)
            {
                bool canPass=true;// 当前方向没有遇到阻挡
                tmpPointF.setX(posNowItem.x()+i*myconstant::ITEM_WIDTH);
                // tmpPointF.setY(posNowItem.y()-i*myconstant::ITEM_HEIGHT);
                tmpItemList=scene()->items(tmpPointF);
                foreach(QGraphicsItem* j,tmpItemList)
                {
                    MyItem* p_myItem=static_cast<MyItem*>(j);
                    {
                        if(p_myItem->itemType==Soldier||p_myItem->itemType==monsterType)// 如果列表有人就把这个人从地形列表里清除
                        {
                            canPass=false;
                            attackableList.append(tmpItemList);
                            break;
                        }
                        else if(p_myItem->itemType==Obstacle

                                ||p_myItem->itemType==Tree)
                        {
                            canPass=false;
                            break;
                        }
                        else
                        {
                            attackableList.append(tmpItemList);
                        }
                    }
                }
                if(!canPass)
                {
                    break;
                }

                tmpItemList.clear();
            }

            // 上侧
            tmpPointF=posNowItem;
            for(int i=1;i<=maxStep;i++)
            {
                bool canPass=true;// 当前方向没有遇到阻挡
                // tmpPointF.setX(posNowItem.x()+i*myconstant::ITEM_WIDTH);
                tmpPointF.setY(posNowItem.y()-i*myconstant::ITEM_HEIGHT);
                tmpItemList=scene()->items(tmpPointF);
                foreach(QGraphicsItem* j,tmpItemList)
                {
                    MyItem* p_myItem=static_cast<MyItem*>(j);
                    {
                        if(p_myItem->itemType==Soldier||p_myItem->itemType==monsterType)// 如果列表有人就把这个人从地形列表里清除
                        {
                            canPass=false;
                            attackableList.append(tmpItemList);

                            break;
                        }
                        else if(p_myItem->itemType==Obstacle

                                ||p_myItem->itemType==Tree)
                        {
                            canPass=false;
                            break;
                        }
                        else
                        {
                            attackableList.append(tmpItemList);
                        }
                    }
                }
                if(!canPass)
                {
                    break;
                }

                tmpItemList.clear();
            }

            // 下侧
            tmpPointF=posNowItem;
            for(int i=1;i<=maxStep;i++)
            {
                bool canPass=true;// 当前方向没有遇到阻挡
                // tmpPointF.setX(posNowItem.x()+i*myconstant::ITEM_WIDTH);
                tmpPointF.setY(posNowItem.y()+i*myconstant::ITEM_HEIGHT);
                tmpItemList=scene()->items(tmpPointF);
                foreach(QGraphicsItem* j,tmpItemList)
                {
                    MyItem* p_myItem=static_cast<MyItem*>(j);
                    {
                        if(p_myItem->itemType==Soldier||p_myItem->itemType==monsterType)// 如果列表有人就把这个人从地形列表里清除
                        {
                            canPass=false;
                            attackableList.append(tmpItemList);
                            break;
                        }
                        else if(p_myItem->itemType==Obstacle

                                ||p_myItem->itemType==Tree)
                        {
                            canPass=false;
                            break;
                        }
                        else
                        {
                           attackableList.append(tmpItemList);
                        }
                    }
                }
                if(!canPass)
                {
                    break;
                }

                tmpItemList.clear();
            }

        }
        else
        {
            // 不是子弹类的攻击，可以直接把周围的item加入列表
            attackableList.append(
                    scene()->items(posNowItem.x()-maxAttack*myconstant::ITEM_WIDTH,
                                   posNowItem.y()-maxAttack*myconstant::ITEM_HEIGHT,
                                   2*maxAttack*myconstant::ITEM_WIDTH,
                                   2*maxAttack*myconstant::ITEM_HEIGHT,
                                   Qt::IntersectsItemShape,Qt::DescendingOrder)
                        );
        }

        foreach(QGraphicsItem* i,attackableList)
        {
            MyItem* p_attackableItem=static_cast<MyItem*>(i);
            if(p_attackableItem->pos()!=this->pos())// 不是自己或者脚下土地
            {
            if(p_attackableItem->redState==0)
            {
                p_attackableItem->redState=1;
                p_attackableItem->update();
            }
            }
            else// 是自己
            {
                attackableList.removeOne(i);
            }
        }
    }
}

// 查找可到达的item
// 使用BFS
void ItemForRole::searchReachabkeItem()
{
    itemListCanMoveTo.clear();
    int maxStep=0;

    // 如果当前不能移动就返回
    if(itemType==MyItem::Soldier)
    {
        if(m_Soldier.state!=iSoldier::init&&m_Soldier.state!=iSoldier::chosen)
        {
            return;
        }
        maxStep=m_Soldier.m_MaximumMovingDistance;

    }
    else if(itemType==MyItem::monsterType)
    {
        if(m_Monster.state!=Monster::init)
        {
            return;
        }
        maxStep=m_Monster.m_MaximumMovingDistance;
    }
    // 如果处于沼泽中，移动距离的计算要减1
    foreach(QGraphicsItem* i,scene()->items(this->pos()))
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        if(p_item->itemType==MyItem::Swamp)
        {
            maxStep-=1;
        }
        if(maxStep<0)
        {
            maxStep=0;
        }
    }
    foreach(QGraphicsItem* i,scene()->items())
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        p_item->disToSearchingItem=1000;
        p_item->haveBeenAddedIntoReachableList=false;
    }

    // BFS 搜索路径
    queue<MyItem*> reachableList;
    queue<MyItem*> needCheckList;
    this->disToSearchingItem=0;
    needCheckList.push(this);
    while (!needCheckList.empty()) {
        // qDebug()<<"needCheckList.size is" <<needCheckList.size();
        MyItem* nowCheckNode=needCheckList.front();
        needCheckList.pop();
        reachableList.push(nowCheckNode);
        // qDebug()<<nowCheckNode->pos()<<" is reachable";
        nowCheckNode->haveBeenAddedIntoReachableList=true;
        // 如果还能接着往下走
        if(nowCheckNode->disToSearchingItem<maxStep)
        {
            // 把四个方向的item都加入待检查清单
            // 左边的点
            QTransform transform;
            QPointF tmpPointFLeft=nowCheckNode->pos();
            MyItem* nextNode=nullptr;
            if(tmpPointFLeft.x()>=-600)
            {
                tmpPointFLeft.setX(nowCheckNode->pos().x()-myconstant::ITEM_WIDTH);
                nextNode=static_cast<MyItem*>(scene()->itemAt(tmpPointFLeft,transform));
                // 如果这个地方不是障碍物并且没有敌人也没有我方的人
                if(nextNode->itemType==MyItem::CementFloor||
                    nextNode->itemType==MyItem::WoodenFloor||
                    nextNode->itemType==MyItem::Swamp ||
                        (!soldierOrMonster&&nextNode->itemType==MyItem::Water))
                {
                    // 并且这个地方不是已经在可到达列表里的
                    if(nextNode->haveBeenAddedIntoReachableList==false)
                    {
                        // 取得到达这一点的移动消耗
                        int cost=1;
                        if(nextNode->itemType==MyItem::Swamp)
                        {
                            cost=2;
                        }
                        // 如果有更短路径，就更新
                        if(nextNode->disToSearchingItem>nowCheckNode->disToSearchingItem+cost)
                        {
                            nextNode->disToSearchingItem=nowCheckNode->disToSearchingItem+cost;
                        }
                        if(nextNode->disToSearchingItem<=maxStep)//这个点可以到达
                        {
                            needCheckList.push(nextNode);

                        }
                    }
                }
            }


            // 右边
            QPointF tmpPointFRight=nowCheckNode->pos();
            if(tmpPointFRight.x()<=500)
            {
                tmpPointFRight.setX(nowCheckNode->pos().x()+myconstant::ITEM_WIDTH);

                nextNode=static_cast<MyItem*>(scene()->itemAt(tmpPointFRight,transform));
                            // 如果这个地方不是障碍物并且没有敌人也没有我方的人
                            if(nextNode->itemType==MyItem::CementFloor||
                                nextNode->itemType==MyItem::WoodenFloor||
                                nextNode->itemType==MyItem::Swamp ||
                                    (!soldierOrMonster&&nextNode->itemType==MyItem::Water))
                            {
                                // 并且这个地方不是已经在可到达列表里的
                                if(nextNode->haveBeenAddedIntoReachableList==false)
                                {
                                    // 取得到达这一点的移动消耗
                                    int cost=1;
                                    if(nextNode->itemType==MyItem::Swamp)
                                    {
                                        cost=2;
                                    }
                                    if(nextNode->disToSearchingItem>nowCheckNode->disToSearchingItem+cost)
                                    {
                                        nextNode->disToSearchingItem=nowCheckNode->disToSearchingItem+cost;
                                    }
                                    if(nextNode->disToSearchingItem<=maxStep)//这个点可以到达
                                    {
                                        needCheckList.push(nextNode);

                                    }
                                }
                            }


            }

             // 上边
            QPointF tmpPointFOver=nowCheckNode->pos();
            if(tmpPointFOver.y()>=-300)
            {
                tmpPointFOver.setY(nowCheckNode->pos().y()-myconstant::ITEM_HEIGHT);
                nextNode=static_cast<MyItem*>(scene()->itemAt(tmpPointFOver,transform));
                            // 如果这个地方不是障碍物并且没有敌人也没有我方的人
                            if(nextNode->itemType==MyItem::CementFloor||
                                nextNode->itemType==MyItem::WoodenFloor||
                                nextNode->itemType==MyItem::Swamp ||
                                    (!soldierOrMonster&&nextNode->itemType==MyItem::Water))
                            {
                                // 并且这个地方不是已经在可到达列表里的
                                if(nextNode->haveBeenAddedIntoReachableList==false)
                                {
                                    // 取得到达这一点的移动消耗
                                    int cost=1;
                                    if(nextNode->itemType==MyItem::Swamp)
                                    {
                                        cost=2;
                                    }
                                    if(nextNode->disToSearchingItem>nowCheckNode->disToSearchingItem+cost)
                                    {
                                        nextNode->disToSearchingItem=nowCheckNode->disToSearchingItem+cost;
                                    }
                                    if(nextNode->disToSearchingItem<=maxStep)//这个点可以到达
                                    {
                                        needCheckList.push(nextNode);

                                    }
                                }
                            }

            }
            //下边
            QPointF tmpPointFBelow=nowCheckNode->pos();
            if(tmpPointFBelow.y()<=200)
            {
                tmpPointFBelow.setY(nowCheckNode->pos().y()+myconstant::ITEM_HEIGHT);
                nextNode=static_cast<MyItem*>(scene()->itemAt(tmpPointFBelow,transform));
                            // 如果这个地方不是障碍物并且没有敌人也没有我方的人
                            if(nextNode->itemType==MyItem::CementFloor||
                                nextNode->itemType==MyItem::WoodenFloor||
                                nextNode->itemType==MyItem::Swamp||
                                    (!soldierOrMonster&&nextNode->itemType==MyItem::Water))
                            {
                                // 并且这个地方不是已经在可到达列表里的
                                if(nextNode->haveBeenAddedIntoReachableList==false)
                                {
                                    // 取得到达这一点的移动消耗
                                    int cost=1;
                                    if(nextNode->itemType==MyItem::Swamp)
                                    {
                                        cost=2;
                                    }
                                    // 如果有更短路径，就更新
                                    if(nextNode->disToSearchingItem>nowCheckNode->disToSearchingItem+cost)
                                    {
                                        nextNode->disToSearchingItem=nowCheckNode->disToSearchingItem+cost;
                                    }
                                    if(nextNode->disToSearchingItem<=maxStep)//这个点可以到达
                                    {
                                        needCheckList.push(nextNode);

                                    }
                                }
                            }

            }

        }

    }

    // 现在reachableList里面就是可到达的地方
    MyItem* tmp=nullptr;
    int queue_size = reachableList.size();
    for(int i = 0; i < queue_size; i++) {
        tmp=reachableList.front();
        if(tmp->pos()!=this->pos())
        {
            this->itemListCanMoveTo.append(tmp);
        }
        reachableList.push(reachableList.front());
        reachableList.pop();
    }

    foreach(QGraphicsItem* i,scene()->items())
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        p_item->disToSearchingItem=1000;
        p_item->haveBeenAddedIntoReachableList=false;
    }
}

void ItemForRole::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 只有左键可以改变“选中操作者”指针的值

    if(event->button()==Qt::LeftButton)// 如果点击的是左键
    {
        // 如果点击的是地形，清空对“要操作者”的存储
        // 在MyItem层面已经实现

        // 如果点击的是敌人，画出敌人的可移动范围和可攻击范围
        if(soldierOrMonster)//是敌人
        {

            // 清空“要操作者”的可移动列表、可攻击列表、颜色、准备攻击状态
            emit clearLastOperatorMemory();

            onlyRoleToOperate=nullptr;

            searchReachabkeItem();
            scene()->update();
            // 画绿色
            foreach(QGraphicsItem* i,itemListCanMoveTo)
            {
                MyItem* p_myItem=static_cast<MyItem*>(i);
                // qDebug()<<i->pos();
                if(p_myItem->greenState==0)// 如果不是绿色
                {
                   p_myItem->greenState=1;
                   p_myItem->update();
                }
            }
            searchAttackableItem();
            // 画红色
            foreach(QGraphicsItem* i,attackableList)
            {
                MyItem* p_attackableItem=static_cast<MyItem*>(i);
                if(p_attackableItem!=this)// 不是自己
                {
                    if(p_attackableItem->redState==0)
                    {
                        p_attackableItem->redState=1;
                        p_attackableItem->update();
                    }
                }
            }


        }


        // 如果点击的是战士

        // 移动完之后按钮亮起
        // 如果选择的是战斗按钮，战士进入prepare状态，等待玩家右键选择攻击对象，画出红色可攻击范围
        // 如果战士处于prepare状态，左键点击仍将战士作为“要操作者”
        // 如果战士不可选，点击在它身上不做反映即可,清空对“要操作者”的存储
        else if(!soldierOrMonster)// 是战士
        {
            // 清空“要操作者”的可移动列表、可攻击列表、颜色、准备攻击状态
            emit clearLastOperatorMemory();

            // 将当前要操作的战士设置为这个战士（先不管能不能操作）
            // 将战士作为“要操作者”

            onlyRoleToOperate=this;
            // 如果战士处于Init状态或者chosen状态，画出可移动范围，不画攻击范围，等待玩家右键点击选择目的地
            if(this->m_Soldier.state==iSoldier::init)
            {
                this->searchReachabkeItem();
                scene()->update();
                // 画绿色
                foreach(QGraphicsItem* i,itemListCanMoveTo)
                {
                    MyItem* p_myItem=static_cast<MyItem*>(i);
                    // qDebug()<<i->pos();
                    if(p_myItem->greenState==0)// 如果不是绿色
                    {
                       p_myItem->greenState=1;
                       p_myItem->update();
                    }
                }
                return;

            }
            else if(this->m_Soldier.state==iSoldier::cannotSelected)
            {
                onlyRoleToOperate=nullptr;
            }
        }

    }
    else if(event->button()==Qt::RightButton)
    {
        //如果当前操作者不为空，而且是战士
        if(onlyRoleToOperate!=nullptr&&onlyRoleToOperate->soldierOrMonster==false)
        {
            // 如果这个“要操作者”是init状态的战士，说明他周围还画着绿色
            if(onlyRoleToOperate->m_Soldier.state==iSoldier::init)
            {
                // 不可移动到有itemForRole类型的区域
                // 放弃移动,清楚绿色
                foreach(auto i,onlyRoleToOperate->itemListCanMoveTo)
                {
                    MyItem* p_item=static_cast<MyItem*>(i);
                    p_item->greenState=0;
                }
                scene()->update();
                onlyRoleToOperate=nullptr;
            }
            // 如果要操作者的状态是准备战斗
            else if(onlyRoleToOperate->m_Soldier.state==iSoldier::prepareToAttack)
            {
                if(!soldierOrMonster)// 如果右键选中的是战士，不能攻击战士
                {
                    foreach(auto i,onlyRoleToOperate->itemListCanMoveTo)
                    {
                        MyItem* p_item=static_cast<MyItem*>(i);
                        p_item->redState=0;
                    }
                    scene()->update();
                    emit clearLastOperatorMemory();

                    onlyRoleToOperate=nullptr;
                    return;
                }
                // 现在右键选中的一定是敌人了
                if(soldierOrMonster)
                {
                    // 判断敌人是否在攻击范围内
                    bool targetIsInTheList=false;
                    if(this->redState==1)
                    {
                        targetIsInTheList=true;
                    }
                    if(targetIsInTheList)
                    {
                        // 向上层发送攻击目标已选中的信号
                        emit thisRoleWIllBeAttacked(onlyRoleToOperate,this);
                    }
                }
            }
        }
    }
}

// 攻击
void ItemForRole::attack(ItemForRole* target,ItemForEffect* effectPlayer)
{

    bool doneAttack=0;
    waitToAttack=false;
    if(target->redState==1)//在攻击范围内
    {
        // 如果目标是怪物
        if(this->itemType==MyItem::Soldier&&target->itemType==MyItem::monsterType)
        {
            doneAttack=true;
            setMovie(attackMovie);
            attackMovie->start();
            effectPlayer->setPos(target->pos());
            effectPlayer->setMovie(effectPlayer->hurtMovieFromSoldier);
            effectPlayer->setVisible(true);
            effectPlayer->soundForSoldierAttack->play();
            effectPlayer->hurtMovieFromSoldier->start();

            qDebug()<<"soldier ID"<<m_Soldier.getID()<<" has attacked monster ID"<<target->m_Monster.getID();
            int hurt=this->m_Soldier.m_AttackPower-target->m_Monster.m_Defense;
            hurt=hurt>0?hurt:0;
            target->m_Monster.modifyHP(-hurt);
            qDebug()<<"monster ID "<<target->m_Monster.getID()<<" HP - "<<hurt;
            qDebug()<<"now HP of monster ID "<<target->m_Monster.getID()<<" is "<<target->m_Monster.getHP();
            // 更新被攻击者的信息到右侧
            emit needToShowInfo(this->pos());
            scene()->update();
            QTimer::singleShot(1000,this,[=](){
                attackMovie->stop();
                effectPlayer->hurtMovieFromSoldier->stop();
                effectPlayer->setVisible(false);
                effectPlayer->soundForSoldierAttack->stop();
                if(target->m_Monster.state==Monster::dead)
                {
                    // 处理角色死亡事件，播放死亡动画
                    target->roleDie();
                }
                emit clearLastOperatorMemory();
            });
        }
        else if(this->itemType==MyItem::monsterType&&target->itemType==MyItem::Soldier)
        {
            doneAttack=true;
            setMovie(attackMovie);
            attackMovie->start();
            effectPlayer->setPos(target->pos());
            effectPlayer->setVisible(true);
            effectPlayer->setMovie(effectPlayer->hurtMovieFromMonster);
            effectPlayer->hurtMovieFromMonster->start();
            effectPlayer->soundForMonsterAttack->play();
            qDebug()<<"monster ID"<<this->m_Monster.getID()<<" attack soldier ID "<<target->m_Soldier.getID();
            int hurt=this->m_Monster.m_AttackPower-target->m_Soldier.m_Defense;
            hurt=hurt>0?hurt:0;
            target->m_Soldier.modifyHP(-hurt);
            qDebug()<<target->m_Soldier.getID()<<" HP - "<<hurt;
            qDebug()<<"now HP of soldier "<<target->m_Soldier.getID()<<" is "<<target->m_Soldier.getHP();
            this->m_Monster.state=Monster::cannotSelected;
            scene()->update();
            QTimer::singleShot(1100,this,[=](){
                attackMovie->stop();
                effectPlayer->hurtMovieFromMonster->stop();
                effectPlayer->soundForMonsterAttack->stop();
                effectPlayer->setVisible(false);
                if(target->m_Soldier.state==iSoldier::dead)
                {
                    // 播放死亡动画
                    target->roleDie();
                }
            });
        }

    }
    // 如果成功
    if(doneAttack&&!soldierOrMonster)//并且是战士
    {
        m_Soldier.state=iSoldier::cannotSelected;
        stateBeforeMenuChoosed=iSoldier::cannotSelected;

    }
    else if(doneAttack&&soldierOrMonster)// 怪物并且攻击成功
    {
        this->m_Monster.state=Monster::cannotSelected;
    }
    else if(!soldierOrMonster)// 如果是战士,没有攻击成功
    {
        m_Soldier.state=iSoldier::cannotMove;// 不再等待攻击
    }
    scene()->update();
}

void ItemForRole::setMovie(QMovie* movie)
{
        prepareGeometryChange();
        QObject::disconnect(mConnection); // disconnect old object
        m_Movie = movie;
        if (m_Movie)
            mConnection = QObject::connect(m_Movie, &QMovie::frameChanged, [=]{ update(); });
}

// 清除周围颜色（绿色和红色）
void ItemForRole::removeColorNearby()
{
    foreach(QGraphicsItem* i,itemListCanMoveTo)
    {
        MyItem* p_myItem=static_cast<MyItem*>(i);
        // qDebug()<<i->pos();
        if(p_myItem->greenState==1)// 如果不是绿色
        {
           p_myItem->greenState=0;
           // 换一个flag状态，也就是设置为不可选
           if(p_myItem->itemType!=MyItem::Soldier
                   &&p_myItem->itemType!=MyItem::monsterType)
           {
               p_myItem->setFlag(ItemIgnoresTransformations);
           }
               p_myItem->update();
        }
    }
    foreach(QGraphicsItem* i,attackableList)
    {
        MyItem* p_attackableItem=static_cast<MyItem*>(i);
        if(p_attackableItem->redState==1)
        {
            p_attackableItem->redState=0;
            p_attackableItem->update();
        }
    }
    focusLeave();

}

// 每回合重置角色状态
void ItemForRole::ResetEveryRound()
{
    if(itemType==Soldier)
    {
        m_Soldier.resetForEveryRound();
        image.load(imagePath);
    }
    else if(itemType==monsterType)
    {
        m_Monster.resetForEveryRound();
        image.load(imagePath);
    }

    this->itemListCanMoveTo.clear();
    this->attackableList.clear();
    MyItem::ResetEveryRound();
}

// 移动
void ItemForRole::walkToTarget(QPointF target,ItemForEffect* effectPlayer)
{
    if(!soldierOrMonster)//如果是战士
    {
        m_Soldier.state=iSoldier::moving;
        // 播放walk动画
        setMovie(walkMovie);
        walkMovie->start();
        effectPlayer->soundForWalk->play();
        qDebug()<<"soldier"<<m_Soldier.getID()<<"is going to move to "<<target;

        // 将角色的移动设置为一定时间的移动
        double dis=abs(this->x()-target.x())+abs(this->y()-target.y());
        int durationTime=dis/myconstant::ITEM_WIDTH*500;
        animWalk->setDuration(durationTime);
        animWalk->setStartValue(this->pos());
        animWalk->setEndValue(target);
        animWalk->start();

        // 当平移动画结束后，将人物状态设置为不可移动
        connect(this->animWalk,&QPropertyAnimation::finished,[=]{
            // qDebug()<<"soldider have moved";
            m_Soldier.state=iSoldier::cannotMove;
            walkMovie->stop();

            qDebug()<<"after moving now soldier is at"<<this->pos();
            emit clearLastOperatorMemory();
            effectPlayer->soundForWalk->stop();
//            searchAttackableItem();
//            foreach(auto i,attackableList)
//            {
//                MyItem* p_item=static_cast<MyItem*>(i);
//                if(p_item!=this)
//                {
//                    p_item->redState=1;
//                }
//                if(p_item->itemType==MyItem::monsterType)// 只允许攻击怪物
//                {
//                    p_item->setFlag(ItemIsSelectable);
//                }
//            }
            scene()->update();
        });


    }
    else// 是怪物要移动
    {
        m_Monster.state=Monster::moving;
        // 播放walk动画
        setMovie(walkMovie);
        walkMovie->start();
        effectPlayer->soundForWalk->play();
        qDebug()<<"monster"<<m_Monster.getID()<<"is moving to "<<target;

        // 将角色的移动设置为一定时间的移动
        double dis=abs(this->x()-target.x())+abs(this->y()-target.y());
        int durationTime=dis/myconstant::ITEM_WIDTH*500;
        animWalk->setDuration(durationTime);
        animWalk->setStartValue(this->pos());
        animWalk->setEndValue(target);
        animWalk->start();

        // 当平移动画结束后，将人物状态设置为不可移动
        connect(this->animWalk,&QPropertyAnimation::finished,[=]{
            // qDebug()<<"have moved";
            m_Monster.state=Monster::cannotMove;
            walkMovie->stop();
            this->setPos(target);
            searchAttackableItem();
            qDebug()<<"after moving now monster is at"<<this->pos();
            emit clearLastOperatorMemory();
            effectPlayer->soundForWalk->stop();
//            searchAttackableItem();
//            foreach(auto i,attackableList)
//            {
//                MyItem* p_item=static_cast<MyItem*>(i);
//                if(p_item!=this)
//                {
//                    p_item->redState=1;
//                }
//                if(p_item->itemType==MyItem::monsterType)// 只允许攻击怪物
//                {
//                    p_item->setFlag(ItemIsSelectable);
//                }

//            }
            scene()->update();
        });

    }
}

// 处理角色死亡事件，播放死亡动画，从画面上清除角色
void ItemForRole::roleDie()
{

    setMovie(dieMovie);
    dieMovie->start();

    qDebug()<<"role type "<<itemType<<" has dead";

    QTimer::singleShot(1000,this,[=](){
        dieMovie->stop();
        roleIsDead=true;
        this->hide();
    });

}
