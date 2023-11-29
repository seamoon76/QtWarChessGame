// playwindow.cpp
// 实现PlayWindow类
// 马骐

#include "playwindow.h"
#include "ui_playwindow.h"
#include "itemforrole.h"
#include"mymessagebox.h"
#include <QDebug>
#include<QPropertyAnimation>
#include<QGraphicsView>
#include<QMessageBox>
#include<QTimer>
#include<QIcon>

// 定义静态变量“当前操作的战士”
ItemForRole* ItemForRole::onlyRoleToOperate=nullptr;

// 实现PlayWindow类的各成员函数
// 参数mapClass是这一关的地图信息和角色设定
PlayWindow::PlayWindow(const mapInfo& mapClass, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayWindow),
    turnEnd(false),
    gameLevelOver(false),
    turnNum(0),
    maxTurnNum(100)
{

    // 设置图形界面和声音播放器
    ui->setupUi(this);
    m_GraphicsScene=new QGraphicsScene(this);
    ui->graphicsView->setScene(m_GraphicsScene);
    playSound=new QSound(":/music/music/bgmusic/mus_title_B.wav");
    playSound->setLoops(-1);
    playSound->play();
    ui->labelForBigEffect->hide();

    // 将静态变量“当前操作的战士”清零
    ItemForRole::onlyRoleToOperate=nullptr;

    // 设置图标
    setWindowIcon(QIcon(":/new/prefix1/icon.ico"));

    // 地图的行数，列数，单元的宽高
    const int itemWidth=myconstant::ITEM_WIDTH;
    const int xNum=myconstant::ITEM_LINE_MAX_NUMBER;
    const int itemHeight=myconstant::ITEM_HEIGHT;
    const int yNum=myconstant::ITEM_ROW_MAX_NUMBER;

    // 设置地图
    // 地图，左上角为原点00
    // 水泥地CementFloor 2,木质地板WoodenFloor 3,障碍物Obstacle 4,水Water 5,树Tree 6
    int x=0;
    for(int i=-itemWidth*(xNum/2);i<itemWidth*(xNum/2)&&x<14;i+=itemWidth,x++)
    {
        int y=0;
        for(int j=-itemHeight*(yNum/2);j<itemHeight*(yNum/2)&&y<8;j+=itemHeight,y++)
        {
            m_MyItem=new MyItem(mapClass.map[y][x]);
            m_MyItem->setPos(QPointF(i,j));
            m_GraphicsScene->addItem(m_MyItem);
            floorList.append(m_MyItem);
        }
    }

    // 设置战士
    foreach(auto sold,mapClass.soldierListInMap)
    {
        m_itemForRole=
                new ItemForRole(0,sold.x,sold.y,sold.soldierType,1);
        soldierList.append(m_itemForRole);
        m_GraphicsScene->addItem(m_itemForRole);
    }

    // 设置怪物
    foreach(auto mons,mapClass.monsterListInMap)
    {
        m_itemForRole=
                new ItemForRole(1,mons.x,mons.y,mons.monsterType,1);
        m_GraphicsScene->addItem(m_itemForRole);
        monsterList.append(m_itemForRole);

    }

    // 连接信号和槽
    connect(this,SIGNAL(roleBeginMoving()),this,SLOT(roleChangeToMovingState()));

    foreach(auto i,m_GraphicsScene->items())
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        connect(p_item,SIGNAL(needToShowInfo(QPointF)),this,SLOT(showItemInfo(QPointF)));
        connect(p_item,SIGNAL(stopShowInfo()),this,SLOT(stopShowItemInfo()));
    }

    foreach(MyItem* p_floor,floorList)
    {
        connect(p_floor,SIGNAL(thisIsWalkDestination(MyItem*)),this,SLOT(choosedSoldierWalkToHere(MyItem*)));
        connect(p_floor,SIGNAL(clearLastOperatorMemory()),
                this,SLOT(clearMainOperatorMemory()));
    }

    foreach(ItemForRole* p_role,soldierList)
    {

        connect(p_role,SIGNAL(thisRoleWIllBeAttacked(ItemForRole*,ItemForRole*)),
                this,SLOT(attackRole(ItemForRole*,ItemForRole*)));
        connect(p_role,SIGNAL(clearLastOperatorMemory()),
                this,SLOT(clearMainOperatorMemory()));
    }

    foreach(ItemForRole* p_role,monsterList)
    {

        connect(p_role,SIGNAL(thisRoleWIllBeAttacked(ItemForRole*,ItemForRole*)),
                this,SLOT(attackRole(ItemForRole*,ItemForRole*)));
        connect(p_role,SIGNAL(clearLastOperatorMemory()),
                this,SLOT(clearMainOperatorMemory()));
    }

    // 设置窗口部件的可见性
    ui->roleName->hide();
    ui->roleImage->hide();
    ui->HPBar->hide();
    ui->HPlabel->hide();
    ui->MPBar->hide();
    ui->MPlabel->hide();
    ui->SkillLabel->hide();
    ui->skillInfo->hide();

    // 最后再添加effect的item用于播放战斗特效和音效
    m_effectPlayer=new ItemForEffect();
    m_GraphicsScene->addItem(m_effectPlayer);
    m_effectPlayer->setZValue(2);
    m_effectPlayer->setVisible(false);
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::clearColor()
{
    foreach(QGraphicsItem* i,ui->graphicsView->scene()->items())
    {
        MyItem* p_Item=static_cast<MyItem*>(i);
        p_Item->greenState=0;
        p_Item->redState=0;

    }
    ui->graphicsView->scene()->update();
}

void PlayWindow::roleChangeToMovingState()
{
    // 将整个页面的绿色和红色清除
    clearColor();
}

// 开启新回合
void PlayWindow::beginNewTurn()
{
    ui->graphicsView->scene()->update();
    turnEnd=false;
    turnNum++;

    // 更新右上角的回合数
    ui->turnNum->setText(QString("回合数:"+QString::number(turnNum)
                                 ));
    qDebug()<<"now turn "<<turnNum<<" began";
    foreach(auto i,ui->graphicsView->scene()->items())
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        p_item->ResetEveryRound();
    }
}

// 检查是否达到通关结束标准
bool PlayWindow::settlementForTurn()
{
    int liveSoldierNum=0;
    int liveMonsterNum=0;
    foreach(auto i, soldierList)
    {
        if(i->m_Soldier.state!=iSoldier::dead)
        {
            liveSoldierNum++;
        }

    }
    foreach(auto i,monsterList)
    {
        if(i->m_Monster.state!=Monster::dead)
        {
            liveMonsterNum++;
        }
    }
    if(liveMonsterNum!=0)// 还有怪物
    {
        if(liveSoldierNum!=0)// 还有战士
        {

            if(turnNum>maxTurnNum)// 关数超出限制
            {
                gameLevelOver=true;
                gameWin=false;// 这里与游戏设定有关，这里设定为指定关内没有消灭敌人算输
                return true;
            }
            else
            {
                bool gameEndDueToBossDie=false;
                foreach(auto i,monsterList)
                {
                    if(i->m_Monster.getID()==9)// 如果这一局有boss
                    {
                        if(i->m_Monster.state==Monster::dead)//如果boss死了
                        {
                            gameEndDueToBossDie=true;
                            gameWin=true;
                        }
                    }

                }
                if(gameEndDueToBossDie)
                {
                    return true;
                }
                else
                {
                    return false;// 游戏未结束
                }
            }
        }
        else// 战士都死亡了
        {
            gameLevelOver=true;// 游戏失败
            gameWin=false;
            return true;
        }
    }
    else// 怪物都死了
    {
        gameLevelOver=true;
        gameWin=true;
        return true;
    }
}

void PlayWindow::on_endTurnButton_clicked()
{
    // 怪物的逻辑是先找到距离最近的一个敌人
    // 如果在攻击范围内，就攻击它
    // 如果不在攻击范围内，但是
    // TODO:怪物开始执行攻击

    nowWhichMonsterIsDoing=0;
    newTimer=new QTimer(this);
    connect(newTimer,SIGNAL(timeout()),this,SLOT(NextMonsterDo()));
    newTimer->start(1500);

    if(nowWhichMonsterIsDoing<0||
          nowWhichMonsterIsDoing>=monsterList.size())
    {
       newTimer->stop();
    }

    turnEnd=true;

    // 结算这一回合
    foreach(ItemForRole* p_sold,soldierList)
    {
        if(p_sold->m_Soldier.state!=iSoldier::dead)
        {
            p_sold->m_Soldier.modifyMP(1);
            if(p_sold->m_Soldier.countForSkill==2)
            {
                p_sold->m_Soldier.countForSkill-=1;
            }
            else if(p_sold->m_Soldier.countForSkill==1)
            {
                p_sold->m_Soldier.countForSkill-=1;
                if(p_sold->m_Soldier.m_skill==iSoldier::hitHard)
                {
                    p_sold->m_Soldier.m_AttackPower/=2;
                }
                else if(p_sold->m_Soldier.m_skill==iSoldier::quickMove)
                {
                    p_sold->m_Soldier.m_MaximumMovingDistance/=2;
                }
                else if(p_sold->m_Soldier.m_skill==iSoldier::improveDefense)
                {
                    p_sold->m_Soldier.m_Defense/=2;
                }
            }
        }
    }



    // 结算在水中的角色，HP-10
    // 清算有没有HP为0的角色
    foreach(ItemForRole* p_sold,soldierList)
    {
        if(p_sold->m_Soldier.state==iSoldier::dead)
        {
            continue;
        }
        else
        {
            // 处理特殊地形造成的事件
            foreach(MyItem* i,floorList)
            {
                if(i->itemType==MyItem::Water&&i->pos()==p_sold->pos())
                {
                    p_sold->m_Soldier.modifyHP(-10);
                    if(p_sold->m_Soldier.state==Monster::dead)
                    {
                        // 处理角色死亡事件，播放死亡动画
                        p_sold->roleDie();
                    }
                }

            }

        }

    }

    if(settlementForTurn())// 如果游戏结束
    {
        if(gameWin)// 通关成功
        {

            // MyMessageBox* msg=new MyMessageBox(QString("tip"),QString("恭喜你通关成功"));
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),QString("恭喜你通关成功！"),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            m_effectPlayer->soundForWin->play();
            QTimer::singleShot(7000,this,[=](){
                this->hide();
                m_effectPlayer->soundForWin->stop();
            });
        }
        else// 通关失败
        {
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),tr("you lose the game."),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            m_effectPlayer->soundForFail->play();
            QTimer::singleShot(7000,this,[=](){
                this->hide();
                m_effectPlayer->soundForFail->stop();
            });
        }
        emit needToBackSelectLevelWindow();
    }

}

// 重新选关的按钮
void PlayWindow::on_return_2_clicked()
{
    emit needToBackSelectLevelWindow();
}

void PlayWindow::showItemInfo(QPointF position)
{

    MyItem* p_item=static_cast<MyItem*>(m_GraphicsScene->itemAt(position,QTransform()));
    if(!p_item)
    {
        return;
    }
    // 如果要显示的是战士的信息
    if(p_item->itemType==MyItem::Soldier)
    {
        ItemForRole* p_soldier=static_cast<ItemForRole*>(p_item);
        ui->roleName->setText(p_soldier->m_Soldier.getName());
//        int width=ui->roleImage->width();
//        int height=ui->roleImage->height();
        ui->roleImage->setStyleSheet(QString("QLabel{border-image:url("+p_item->imagePath+");}"));
        int HP=p_soldier->m_Soldier.getHP();
        int maxHP=p_soldier->m_Soldier.m_HPUpperLimit;
        ui->HPBar->setMaximum(maxHP);
        ui->HPBar->setValue(HP);

        ui->HPlabel->setText(QString("HP:")+QString::number(HP)
                             +QString("/")+QString::number(maxHP));
        ui->skillInfo->setText(p_soldier->m_info);
        int MP=p_soldier->m_Soldier.getMP();
        int maxMP=p_soldier->m_Soldier.m_MPUpperLimit;
        ui->MPBar->setMaximum(maxMP);
        ui->MPBar->setValue(MP);

        ui->MPlabel->setText(QString("MP:")+QString::number(MP)
                             +QString("/")+QString::number(maxMP));

        ui->SkillLabel->setText("详细信息");

        ui->roleName->show();
        ui->roleImage->show();
        ui->HPBar->show();
        ui->HPlabel->show();
        ui->MPBar->show();
        ui->MPlabel->show();
        ui->SkillLabel->show();
        ui->skillInfo->show();
    }
    // 如果要显示的是怪物的信息
    else if(p_item->itemType==MyItem::monsterType)
    {
        ItemForRole* p_monster=static_cast<ItemForRole*>(p_item);
        ui->roleName->setText(p_monster->m_Monster.getName());
        ui->roleImage->setStyleSheet(QString("QLabel{border-image:url("+p_item->imagePath+");}"));
        int HP=p_monster->m_Monster.getHP();
        int maxHP=p_monster->m_Monster.m_HPUpperLimit;
        ui->HPBar->setMaximum(maxHP);
        ui->HPBar->setValue(HP);

        ui->HPlabel->setText(QString("HP:")+QString::number(HP)
                             +QString("/")+QString::number(maxHP));
        ui->skillInfo->setText(p_monster->m_info);
        int MP=p_monster->m_Monster.getMP();
        int maxMP=p_monster->m_Monster.m_MPUpperLimit;
        ui->MPBar->setMaximum(maxMP);
        ui->MPBar->setValue(MP);

        ui->MPlabel->setText(QString("MP:")+QString::number(MP)
                             +QString("/")+QString::number(maxMP));
        ui->SkillLabel->setText("详细信息");

        ui->roleName->show();
        ui->roleImage->show();
        ui->HPBar->show();
        ui->HPlabel->show();
        ui->MPBar->show();
        ui->MPlabel->show();
        ui->SkillLabel->show();
        ui->skillInfo->show();
    }
    else// 要显示的是特殊地形
    {
        ui->roleName->setText(p_item->terrainName);// 显示地形名
        ui->roleImage->setStyleSheet(QString("QLabel{border-image:url("+p_item->imagePath+");}"));
        ui->skillInfo->setText(p_item->m_info);
        ui->SkillLabel->setText(QString("地形信息"));
        ui->roleName->show();
        ui->roleImage->show();
        ui->skillInfo->show();
        ui->SkillLabel->show();
        ui->HPBar->hide();
        ui->HPlabel->hide();
        ui->MPBar->hide();
        ui->MPlabel->hide();
    }
}

void PlayWindow::stopShowItemInfo()
{
    ui->roleName->hide();
    ui->roleImage->hide();
    ui->HPBar->hide();
    ui->HPlabel->hide();
    ui->MPBar->hide();
    ui->MPlabel->hide();
    ui->SkillLabel->hide();
    ui->skillInfo->hide();
}

void PlayWindow::attackRole(ItemForRole* attacker,ItemForRole* target)
{
    attacker->attack(target,m_effectPlayer);
    QTimer::singleShot(2000,this,[=](){
        if(target->roleIsDead||target->m_Monster.state==Monster::dead
                ||target->m_Monster.getHP()<=0)
        {
            ui->graphicsView->scene()->removeItem(target);
        }
    });
}

void PlayWindow::choosedSoldierWalkToHere(MyItem* destination)
{
    if(ItemForRole::onlyRoleToOperate!=nullptr)
    {
        ItemForRole* p_walker=ItemForRole::onlyRoleToOperate;
        if(p_walker->soldierOrMonster==0)// 是战士
        {
            // 判断目的地是否在可到达范围内
            bool destinationIsInTheList=false;
            if(destination->greenState==true)
            {
                destinationIsInTheList=true;
            }
            if(destinationIsInTheList)
            {
                // 如果可以到达，就移动到那里
                p_walker->walkToTarget(destination->pos(),m_effectPlayer);
            }
        }
    }

}

void PlayWindow::clearMainOperatorMemory()
{
   // 清除颜色
   foreach(MyItem* p_floor,floorList)
   {
       p_floor->greenState=0;
       p_floor->redState=0;
   }
   clearColor();
   m_GraphicsScene->update();
   // 取消对“选中的操作者”的选择
   if(ItemForRole::onlyRoleToOperate!=nullptr)
   {
       ItemForRole::onlyRoleToOperate->itemListCanMoveTo.clear();
       ItemForRole::onlyRoleToOperate->attackableList.clear();
       if(ItemForRole::onlyRoleToOperate->m_Soldier.state==iSoldier::prepareToAttack)
       {
           if(ItemForRole::onlyRoleToOperate->stateBeforeMenuChoosed==iSoldier::init
                   ||ItemForRole::onlyRoleToOperate->stateBeforeMenuChoosed==iSoldier::cannotMove)
           ItemForRole::onlyRoleToOperate->m_Soldier.state=ItemForRole::onlyRoleToOperate->stateBeforeMenuChoosed;
       }
   }
}

void PlayWindow::on_attackButton_clicked()
{
    clearColor();
    if(ItemForRole::onlyRoleToOperate!=nullptr)
    {
        if(ItemForRole::onlyRoleToOperate->m_Soldier.state==iSoldier::init
                ||ItemForRole::onlyRoleToOperate->m_Soldier.state==iSoldier::cannotMove)
        {
            ItemForRole::onlyRoleToOperate->stateBeforeMenuChoosed=
                    ItemForRole::onlyRoleToOperate->m_Soldier.state;
            ItemForRole::onlyRoleToOperate->m_Soldier.state=iSoldier::prepareToAttack;
            ItemForRole::onlyRoleToOperate->searchAttackableItem();
            foreach(QGraphicsItem* i,ItemForRole::onlyRoleToOperate->attackableList)
            {
                MyItem* p_item=static_cast<MyItem*>(i);
                p_item->redState=1;
            }
            m_GraphicsScene->update();
        }
    }
}

//放弃攻击机会
void PlayWindow::on_standby_clicked()
{
    if(ItemForRole::onlyRoleToOperate!=nullptr)
    {
        if(ItemForRole::onlyRoleToOperate->m_Soldier.state!=iSoldier::dead)
        {
            ItemForRole::onlyRoleToOperate->m_Soldier.state=iSoldier::cannotSelected;
        }
    }
    m_GraphicsScene->update();
}

// 让下一个怪物执行移动和攻击
void PlayWindow::NextMonsterDo()// 默认从0开始
{
    if(nowWhichMonsterIsDoing<0||
          nowWhichMonsterIsDoing>=monsterList.size())
  {
      newTimer->stop();
      if(nowWhichMonsterIsDoing==monsterList.size())
      {


              beginNewTurn();

      }
      return;
  }
    // 如果这个怪物是死的，一直往后找直到找到一个没有死亡的
    // 这个循环只会检查死亡到倒数第二个怪物
    while(monsterList[nowWhichMonsterIsDoing]->m_Monster.state==Monster::dead
          &&nowWhichMonsterIsDoing<monsterList.size()-1)
    {
        // qDebug()<<"nowWhichMonsterIsDoing is "<<nowWhichMonsterIsDoing;
        nowWhichMonsterIsDoing++;
    }
    // 如果最后一个怪物也是死的，就没有必要继续往下寻找了，再加一次
    if(monsterList[nowWhichMonsterIsDoing]->m_Monster.state==Monster::dead
            &&nowWhichMonsterIsDoing==monsterList.size()-1)
    {
        nowWhichMonsterIsDoing++;
        return;
    }



    ItemForRole* p_monster=monsterList[nowWhichMonsterIsDoing];
    nowWhichMonsterIsDoing++;
    p_monster->searchAttackableItem();

//    if(p_monster->m_Monster.getID()==6&&turnNum%3==2)// 可恢复怪物血量的飞龙并且回合数为3的倍数
//    {
//        foreach(auto i,monsterList)
//        {
//            if(i->m_Monster.state!=Monster::dead)
//            {
//                i->m_Monster.modifyHP(10);
//            }
//        }
//        return;
//    }

    if(p_monster->m_Monster.getID()==9)// BOSS
    {
        if(p_monster->m_Monster.getHP()<40)
        {
            p_monster->m_Monster.m_MaximumMovingDistance=4;
        }
        if(p_monster->m_Monster.getHP()<50
                &&p_monster->m_Monster.bossCanPlayBigEffect==true)
        {
            // 控制发动BOSS的大招
            p_monster->setMovie(p_monster->attackMovie);
            p_monster->attackMovie->start();

            QTimer::singleShot(1000,this,[=](){
                p_monster->attackMovie->stop();
                p_monster->m_Monster.bossCanPlayBigEffect=false;
                bigEffect=m_effectPlayer->skillMovieOfBOSS;
                bigEffect->setScaledSize(ui->labelForBigEffect->size());
                ui->labelForBigEffect->setMovie(bigEffect);
                ui->labelForBigEffect->show();
                ui->labelForBigEffect->raise();
                bigEffect->start();
                QTimer::singleShot(1000,this,[=](){
                    bigEffect->stop();
                    ui->labelForBigEffect->hide();
                    ui->labelForBigEffect->lower();
                    foreach(auto i,soldierList)
                    {
                        if(i->m_Soldier.state!=iSoldier::dead)
                        {
                            i->m_Soldier.modifyHP(-10);
                            if(i->m_Soldier.state==iSoldier::dead)
                            {
                                // 播放死亡动画
                                i->roleDie();
                            }
                        }
                    }
                });
            });
            return;
        }

    }

    // 如果附近有敌人就攻击敌人
    foreach(auto i,p_monster->attackableList)
    {
        MyItem* p_item=static_cast<MyItem*>(i);
        if(p_item->itemType==MyItem::Soldier)// 如果有敌人
        {
            ItemForRole* target=static_cast<ItemForRole*>(p_item);
            p_monster->attack(target,m_effectPlayer);
            QTimer::singleShot(2000,this,[=](){
                if(target->roleIsDead)
                {
                    ui->graphicsView->scene()->removeItem(target);
                }
            });
            break;//跳出攻击循环
        }
    }

    if(p_monster->m_Monster.state==Monster::cannotSelected)// 攻击完成
    {
        return;
    }
    else
    {
        // 找到距离最近的一个敌人
        ItemForRole* targetSoldier=nullptr;
        double minDis=100000000000000;
        foreach(ItemForRole* sold,this->soldierList)
        {
            if(sold->m_Soldier.state!=iSoldier::dead)//死亡对象不能作为目标
            {
                double tmp=p_monster->distance(sold->pos(),p_monster->pos());
                if(minDis>tmp)
                {
                    minDis=tmp;
                    targetSoldier=sold;
                }
            }
        }

        // 搜索可移动路径
        if(targetSoldier==nullptr)
        {
            return;
        }
        qDebug()<<"the target Soldier of this monster is at "<<targetSoldier->pos();
        p_monster->searchReachabkeItem();

        // 遍历移动范围的item，看敌人和item距离是否小于攻击距离
        foreach(auto p_greenitem,p_monster->itemListCanMoveTo)
        {
            // 如果是
            //    移动到那个位置，然后执行攻击
            if((p_monster->distance(p_greenitem->pos(),
                                    targetSoldier->pos())/myconstant::ITEM_WIDTH
                <p_monster->m_Monster.m_MaximumAttackRange+1))
            {

             p_monster->walkToTarget(p_greenitem->pos(),m_effectPlayer);
             QTimer::singleShot(1500,this,[=](){
                 p_monster->searchAttackableItem();
                 targetSoldier->redState=1;
                 p_monster->attack(targetSoldier,m_effectPlayer);
             });
             break;
        }
        }

        // 如果不是
        //    遍历移动范围，看移动范围中哪一个与敌人距离最小，移动到那里
        qDebug()<<"now monster"<<p_monster->m_Monster.getName()<<"is in "
<<p_monster->m_Monster.state<<"state";
        if(p_monster->m_Monster.state==Monster::init)
        {
            QGraphicsItem* destination=nullptr;
            double minDis=100000000000000;
            foreach(auto p_greenitem,p_monster->itemListCanMoveTo)
            {
                double tmp=p_monster->distance(targetSoldier->pos(),p_greenitem->pos());
                if(minDis>tmp)
                {
                    minDis=tmp;
                    destination=p_greenitem;
                }
            }
            if(destination!=nullptr)
            {
                p_monster->walkToTarget(destination->pos(),m_effectPlayer);
                p_monster->searchAttackableItem();
                foreach(auto i,p_monster->attackableList)
                {
                    MyItem* p_item=static_cast<MyItem*>(i);
                    if(p_item->itemType==MyItem::Soldier)// 如果有敌人
                    {
                        ItemForRole* target=static_cast<ItemForRole*>(p_item);
                        p_monster->attack(target,m_effectPlayer);
                        QTimer::singleShot(2000,this,[=](){
                            if(target->roleIsDead)
                            {
                                ui->graphicsView->scene()->removeItem(target);
                            }
                        });
                        break;//跳出攻击循环
                    }
                }
                p_monster->m_Monster.state=Monster::cannotSelected;
            }

        }
}
}

void PlayWindow::on_skillButton_clicked()
{
    if(ItemForRole::onlyRoleToOperate==nullptr)
    {
        return;
    }
    ItemForRole* m_pSoldier=ItemForRole::onlyRoleToOperate;
    if(m_pSoldier->m_Soldier.state==iSoldier::cannotSelected)
    {
        qDebug()<<QString("turn of soldier ID")<<m_pSoldier->m_Soldier.getID()
               <<" is end so you can not use skill.";
        return;
    }
    if(m_pSoldier->m_Soldier.m_skill==iSoldier::hitHard)
    {
        if(m_pSoldier->m_Soldier.getMP()<3)// 如果MP值不够
        {
            qDebug()<<"MP of this soldier is not enough.";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),QString("这个角色的MP不足以发动技能"),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else if(m_pSoldier->m_Soldier.countForSkill!=0)
        {
            qDebug()<<"can not use skill to improve for two consecutive rounds";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),QString("不能连续两回合使用提升类技能"),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else// MP值足够
        {
            m_pSoldier->m_Soldier.modifyMP(-3);
            m_pSoldier->m_Soldier.m_AttackPower*=2;
            m_pSoldier->m_Soldier.countForSkill=2;
            m_pSoldier->m_Soldier.state=iSoldier::cannotSelected;

            // 播放战斗力提升的动画
            m_effectPlayer->setPos(m_pSoldier->pos());
            m_effectPlayer->setMovie(m_effectPlayer->defenceUpMovie);
            m_effectPlayer->setVisible(true);
            m_effectPlayer->soundForAddBlood->play();
            m_effectPlayer->defenceUpMovie->start();

            m_GraphicsScene->update();
            QTimer::singleShot(1000,this,[=](){
                m_effectPlayer->defenceUpMovie->stop();
                m_effectPlayer->setVisible(false);
                m_effectPlayer->soundForAddBlood->stop();
                clearMainOperatorMemory();
            });

        }
    }
    else if(m_pSoldier->m_Soldier.m_skill==iSoldier::quickMove)
    {
        if(m_pSoldier->m_Soldier.getMP()<2)// 如果MP值不够
        {
            qDebug()<<"MP is not enough";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),"MP is not enough",
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else if(m_pSoldier->m_Soldier.countForSkill!=0)
        {
            qDebug()<<"can not use skill to improve for two consecutive rounds";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),QString("不能连续两回合使用提升类技能"),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else// MP值足够
        {
            m_pSoldier->m_Soldier.modifyMP(-2);
            m_pSoldier->m_Soldier.m_MaximumMovingDistance*=2;
            m_pSoldier->m_Soldier.countForSkill=2;
            m_pSoldier->m_Soldier.state=iSoldier::cannotSelected;

            // 播放战斗力提升的动画
            m_effectPlayer->setPos(m_pSoldier->pos());
            m_effectPlayer->setMovie(m_effectPlayer->addHPMovie);
            m_effectPlayer->setVisible(true);
            m_effectPlayer->soundForAddBlood->play();
            m_effectPlayer->addHPMovie->start();

            m_GraphicsScene->update();
            QTimer::singleShot(1000,this,[=](){
                m_effectPlayer->addHPMovie->stop();
                m_effectPlayer->setVisible(false);
                m_effectPlayer->soundForAddBlood->stop();
                clearMainOperatorMemory();
            });

        }
    }
    else if(m_pSoldier->m_Soldier.m_skill==iSoldier::improveDefense)
    {
        if(m_pSoldier->m_Soldier.getMP()<2)// 如果MP值不够
        {
            qDebug()<<"MP is not enough";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),"MP is not enough",
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else if(m_pSoldier->m_Soldier.countForSkill!=0)
        {
            qDebug()<<"can not use skill to improve for two consecutive rounds";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),QString("不能连续两回合使用提升类技能"),
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else// MP值足够
        {
            m_pSoldier->m_Soldier.modifyMP(-2);
            m_pSoldier->m_Soldier.m_Defense*=2;
            m_pSoldier->m_Soldier.countForSkill=2;
            m_pSoldier->m_Soldier.state=iSoldier::cannotSelected;

            // 播放战斗力提升的动画
            m_effectPlayer->setPos(m_pSoldier->pos());
            m_effectPlayer->setMovie(m_effectPlayer->addHPMovie);
            m_effectPlayer->setVisible(true);
            m_effectPlayer->soundForAddBlood->play();
            m_effectPlayer->addHPMovie->start();

            m_GraphicsScene->update();
            QTimer::singleShot(1000,this,[=](){
                m_effectPlayer->addHPMovie->stop();
                m_effectPlayer->setVisible(false);
                m_effectPlayer->soundForAddBlood->stop();
                clearMainOperatorMemory();
            });

        }
    }
    else if(m_pSoldier->m_Soldier.m_skill==iSoldier::support)
    {
        if(m_pSoldier->m_Soldier.getMP()<3)// 如果MP值不够
        {
            qDebug()<<"MP is not enough";
            QMessageBox* msg=new QMessageBox(
                        tr("tip"),"MP is not enough",
                        QMessageBox::Information,QMessageBox::Ok,0,0);
            msg->show();
            return;
        }
        else// MP值足够
        {
            m_pSoldier->m_Soldier.modifyMP(-3);

            m_pSoldier->searchAttackableItem();
            m_pSoldier->m_Soldier.modifyHP(30);
            // 搜索攻击范围内的战士
            vector<ItemForRole*> supportTargetList;
            foreach(QGraphicsItem* p_graphitem,m_pSoldier->attackableList)
            {
                MyItem* p_myItem=static_cast<MyItem*>(p_graphitem);
                if(p_myItem->itemType==MyItem::Soldier)
                {
                    ItemForRole* p_sold=static_cast<ItemForRole*>(p_myItem);
                    if(p_sold->m_Soldier.state!=iSoldier::dead)
                    {
                        p_sold->m_Soldier.modifyHP(30);
                        supportTargetList.push_back(p_sold);
                    }
                }
            }

           m_pSoldier->m_Soldier.state=iSoldier::cannotSelected;

           m_effectPlayer->soundForAddBlood->play();
           m_GraphicsScene->update();
            QTimer::singleShot(1000,this,[=](){

                m_effectPlayer->soundForAddBlood->stop();
                clearMainOperatorMemory();
            });

        }
    }
}
