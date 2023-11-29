#ifndef MONSTER_H
#define MONSTER_H

// 怪物类
// 定义怪物的所有共同属性
// 马骐

#include "isoldier.h"
#include "myconstant.h"
#include <QWidget>

class Monster:public QWidget
{
    Q_OBJECT
public:
    Monster(QWidget* parent=0);

protected:
    QString m_name;// 怪物的名字
    int m_ID;// 怪物的编号
    int m_HP;//Hit Point,生命值
    int m_MP;//Magic Point,技能值
    int m_XCoordinate,m_YCoordinate;// XY坐标
    int m_Exp;//经验值，不再使用
    int m_Level;// 怪物的等级
    QString  m_Skill;// 技能，不同怪物的技能不同

public:
    int m_MaximumMovingDistance;// 机动性，单回合最大移动距离
    int m_MaximumAttackRange;   // 攻击范围，一次能攻击到的最远距离
    int m_HPUpperLimit;         //生命值上限，升级后就可以提高
    int m_MPUpperLimit;         //技能值上限，升级后就可以提高
    int m_AttackPower;          // 攻击能力
    int m_Defense;              // 防御力
    bool bossCanPlayBigEffect=true;

public:
    // 用有限状态机记录怪物状态
    enum monsterState{init,             // 初始状态，未执行移动和攻击，没有被选中
                      hoverd,           // 鼠标悬浮在角色上方，显示相关信息（HP，MP，技能）
                      chosen,           // 未执行移动和攻击，被选中,显示可移动范围（绿色矩形）,显示可攻击范围（红色方框）
                      moving,           // 在玩家选择了绿色的某个方块后， 清除颜色，并开始向目的地移动，在移动过程中播放动画
                      cannotMove,       // 移动完毕，显示菜单，由菜单选项触发攻击、治疗、技能、待命、取消等行动
                      prepareToAttack,  // 将要进行攻击，将攻击范围显示为红色
                      attacking,        // 玩家选择了攻击对象后，清除红色，向目标发动攻击
                      cannotSelected,   // 执行完毕，变灰，不能再用鼠标选它
                     dead               // 死亡状态，当切换到这一状态时播放死亡事件
                     };
    int state=Monster::init;// 对应monsterState的值

    // 设置怪物名字
    void setName(QString name);
    // 读取怪物名字
    QString getName();
    void setID(int ID);
    int getID();


    // 设置生命值
    // 参数HP为要设置的值
    // 返回值代表实际设置成的值，考虑等级等限制因素
    int setHP(int HP);

    int getHP();

    // 修改HP
    // 参数change，int型，要增加或要减少的值
    // 返回值，修改后的HP值
    int modifyHP(int change);

    // 设置技能值
    // 参数MP为要设置的值
    // 返回值代表实际设置成的值，考虑等级等限制因素
    int setMP(int MP);

    int getMP();

    // 修改MP
    // 参数change，int型，要增加或要减少的值
    // 返回值，修改后的MP值
    int modifyMP(int change);

    int getXCoordinate();
    int getYCoordinate();

    bool setXCoordinate(int x);
    bool setYCoordinate(int y);

    // 显示怪物的信息，用于当玩家将鼠标放置在怪物上方时，可以查看怪物信息
    void showInfoOfSoldier();
    // 进行升级，不在使用
    virtual void upgrade();

    // 虚函数
    // 处理怪物的死亡事件
    virtual void die();

    bool attack();

    // 显示攻击
    virtual void showAttack();

    // 显示升级过程
    virtual void showUpGrade();

    // 显示死亡过程
    virtual void showDie();

    // 显示走路
    virtual void showWalk();

    // 显示跑步(非必须）
    virtual void showRun();

    // 显示躲避（蹲伏）
    virtual void showMiss();

    // 待命,即放弃这一回合的执行动作的机会
    void standBy();

    // 设置战士的值，用于初始化的过程
    void setMonster(QString name,int id,int maxHP,int maxMP,
                    int XCoordinate,int YCoordinate,
                    int attackPower,int defense,int movingDistance,
                    int level,int attackRange);

    // 每回合重置怪物状态
    void resetForEveryRound();
};

#endif // MONSTER_H
