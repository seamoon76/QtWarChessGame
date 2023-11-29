#ifndef ISOLDIER_H
#define ISOLDIER_H
// AI战士类
// 定义我方AI战士的所有共同属性
// 马骐

#include "monster.h"
#include "myconstant.h"
#include <QWidget>
#include <QString>
#include <QResource>

// 战士类
class iSoldier:public QWidget
{
    Q_OBJECT
private:
    QString m_name;// 战士的名字
    int m_ID;// 战士的编号
    int m_HP;// Hit Point,生命值
    int m_MP;// Magic Point,技能值
    int m_XCoordinate,m_YCoordinate;// XY坐标
    int m_Exp;// 经验值，未启用
    int m_Level;// AI战士的等级，默认为1

public:
    int m_MaximumMovingDistance;// 机动性，单回合最大移动距离
    int m_MaximumAttackRange;// 攻击范围，一次能攻击到的最远距离
    int m_HPUpperLimit;//生命值上限，升级后就可以提高
    int m_MPUpperLimit;//技能值上限，升级后就可以提高
    int m_AttackPower;// 攻击能力
    int m_Defense;// 防御力

public:
    // 用有限状态机记录战士状态
    enum soldierState{init,// 初始状态，未执行移动和攻击，没有被选中
                      hoverd,// 鼠标悬浮在角色上方，显示相关信息（HP，MP，技能）
                      chosen,// 未执行移动和攻击，被选中,显示可移动范围（绿色矩形）,显示可攻击范围（红色方框）
                      moving,// 在玩家选择了绿色的某个方块后， 清除颜色，并开始向目的地移动，在移动过程中播放动画
                      cannotMove,// 移动完毕，显示菜单，由菜单选项触发攻击、治疗、技能、待命、取消等行动
                      prepareToAttack,// 将要进行攻击，将攻击范围显示为红色
                      attacking,// 玩家选择了攻击对象后，清除红色，向目标发动攻击
                      cannotSelected,// 执行完毕，变灰，不能再用鼠标选它
                     dead// 死亡状态，当切换到这一状态时播放死亡事件
                     };
    int state=iSoldier::init;// 对应soldierState的值
    // 战士的技能
    enum skillKind{
        hitHard,// 【重击】：接下来两回合内攻击力加倍，消耗3mp
        quickMove,//【速移】：接下来两回合内移动距离+1，消耗2MP
        improveDefense,// 【提升】：接下来两回合内防御力加倍，消耗2mp
        support//【支援】为攻击范围内所有我方战士恢复HP30，消耗3mp
    };
    int m_skill=iSoldier::improveDefense;// 技能，不同战士的技能不同
    int countForSkill=0;// 用于配合技能使用，比如使用【提升】技能后下两回合防御力增加，
                        // 在回合数变化时修改这个变量的值

    // 构造函数
    iSoldier(QWidget* parent=0);
    // 设置战士名字
    void setName(QString name);
    // 读取战士名字
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

    // 显示战士的信息，用于当玩家将鼠标防止在战士上方时，可以查看战士信息
    void showInfoOfSoldier();
    // 进行升级
    virtual void upgrade();

    // 虚函数
    // 处理战士的死亡事件
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
    void setSoldier(QString name,int id,int maxHP,int maxMP,
                    int XCoordinate,int YCoordinate,
                    int attackPower,int defense,int movingDistance,
                    int level,int attackRange);
    // 在新的回合开始时将战士状态恢复
    void resetForEveryRound();
};

#endif // ISOLDIER_H
