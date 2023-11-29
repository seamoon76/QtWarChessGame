#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

// 定义PlayWindow类
// 游戏运行的主要窗口，可以实现与玩家的交互
// 每一关是一个PlayWindow对象
// 马骐

#include "myconstant.h"
#include "myitem.h"
#include "itemforeffect.h"
#include "itemforrole.h"
#include "mapinfo.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSound>
#include <QMovie>
#include <vector>

namespace Ui {
class PlayWindow;
}

// 每一关是一个PlayWindow对象
class PlayWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 参数mapClass是这一关的地图信息和角色设定
    explicit PlayWindow(const mapInfo& mapClass,QWidget *parent = nullptr);
    ~PlayWindow();
    void clearColor();

    ItemForRole* soldierWaitingToAttack=nullptr;
    ItemForEffect* m_effectPlayer=nullptr;// 动画的播放器
    QSound* playSound=nullptr;            // 音效的播放器
    QTimer* newTimer=nullptr;             // 游戏的计时器

signals:
    void needToBackSelectLevelWindow();   // 返回选关界面的信号
    void roleBeginMoving();               // 角色开始移动的信号

public slots:
    // 处理角色移动状态的事件
    void roleChangeToMovingState();

    // 处理“结束操作”按钮的点击事件
    void on_endTurnButton_clicked();

    // 处理“重新选关”按钮的点击事件
    void on_return_2_clicked();

    // 在右侧信息栏显示角色或者地形的详细信息
    void showItemInfo(QPointF position);

    // 停止显示信息
    void stopShowItemInfo();

    // 返回值表示游戏是否结束，0表示游戏没有结束，1表示游戏结束
    // 如果是游戏结束，这个函数还会判断哪一方获胜并存储在成员变量gameWin中
    bool settlementForTurn();

    // 攻击某个角色
    void attackRole(ItemForRole* attacker,ItemForRole* target);

    // 玩家已经选择了移动的目的地，这个函数复杂把角色移动到参数destination的位置
    void choosedSoldierWalkToHere(MyItem* destination);

    // 清楚“当前操作者”的选择，即玩家可以切换操纵另一个角色
    void clearMainOperatorMemory();

private slots:
    // 攻击按钮
    void on_attackButton_clicked();

    // 待命按钮
    void on_standby_clicked();

    // 当轮到怪物行动的时候，会依次对每一个怪物调用该函数，让它执行移动和攻击
    void NextMonsterDo();

    // 技能按钮
    void on_skillButton_clicked();

private:
    Ui::PlayWindow *ui=nullptr;

    QGraphicsScene* m_GraphicsScene=nullptr;// 使用QGraphicsScene管理图形单元
    MyItem* m_MyItem=nullptr;               // 自定义的图形单元类，是对QGraphicsObject的封装
    ItemForRole* m_itemForRole=nullptr;     // 角色单元的指针

    QList<ItemForRole*> soldierList;        // 这一关中的战士列表
    QList<ItemForRole*> monsterList;        // 这一关中的怪物列表
    QList<MyItem*> floorList;               // 这一关中的地形列表

    bool turnEnd;                           // 回合是否结束
    bool gameLevelOver=false;               // 游戏（这一关）是否结束
    bool gameWin=false;                     // 这一关是否获胜
    int turnNum;                            // 回合数
    int maxTurnNum;                         // 这一关最大回合数
    int needToTellHowToAttack=true;         // 是否需要提醒玩家右键执行攻击
    int nowWhichMonsterIsDoing=0;           // 用于让怪物依次操作而不是同时操作的计数器
    QMovie* bigEffect=nullptr;              // boss的大技能播放器

    // 开启新回合
    void beginNewTurn();

};

#endif // PLAYWINDOW_H
