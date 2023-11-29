//  AI战士基类
// 定义我方AI战士的所有共同属性
// 马骐
#include "isoldier.h"
#include"myconstant.h"


//  AI战士基类构造函数
// int iSoldier::MAX_HP_LEVEL_ONE=20;
iSoldier::iSoldier(QWidget* parent)
    :QWidget(parent),state(init)
{

    m_name="default_soldier";
    m_ID=0;
    m_Level=1;
    m_HP=50;
    m_MP=5;
    m_XCoordinate=0;
    m_YCoordinate=0;
    m_HPUpperLimit=myconstant::MAX_HP_OF_LEVEL_1;
    m_MPUpperLimit=myconstant::MAX_MP_OF_LEVEL_1;
    m_Exp=0;
    m_MaximumMovingDistance=2;
    m_MaximumAttackRange=1;
}

void iSoldier::setName(QString name)
{
    this->m_name=name;
}

QString iSoldier::getName()
{
    return this->m_name;
}

void iSoldier::setID(int ID)
{
    this->m_ID=ID;
}

int iSoldier::getID()
{
    return m_ID;
}

// 最大可设置到HP上限，返回设置后的值
int iSoldier::setHP(int HP)
{
    if(HP<0)
    {
        return m_HP;
    }
    else if(HP<=m_HPUpperLimit)
    {
        m_HP=HP;
    }
    else
    {
        m_HP=m_HPUpperLimit;
    }
    return m_HP;
}

int iSoldier::getHP()
{
    return m_HP;
}

int iSoldier::modifyHP(int change)
{
    if(m_HP+change<=0)
    {
        m_HP=0;// 死亡
        state=iSoldier::dead;
        die();
    }
    else if(m_HP+change>m_HPUpperLimit)
    {
        m_HP=m_HPUpperLimit;
    }
    else
    {
        m_HP+=change;
    }
    return m_HP;
}

// 最大可设置到MP上限，返回设置后的值
int iSoldier::setMP(int MP)
{
    if(MP<0)
    {
        return m_MP;
    }
    else if(MP<=m_MPUpperLimit)
    {
        m_MP=MP;
    }
    else
    {
        m_MP=m_MPUpperLimit;
    }
    return m_MP;
}

int iSoldier::getMP()
{
    return m_MP;
}

int iSoldier::modifyMP(int change)
{
    if(m_MP+change<0)
    {
        m_MP=0;
    }
    else if(m_MP+change>m_MPUpperLimit)
    {
        m_MP=m_MPUpperLimit;
    }
    else
    {
        m_MP+=change;
    }
    return m_MP;
}

int iSoldier::getXCoordinate()
{
    return m_XCoordinate;
}

int iSoldier::getYCoordinate()
{
    return m_YCoordinate;
}

bool iSoldier::setXCoordinate(int x)
{
    if(x<myconstant::WINDOW_WIDTH&&x>0)
    {
        m_XCoordinate=x;
        return true;
    }
    else
    {
        return false;
    }
}

bool iSoldier::setYCoordinate(int y)
{
    if(y>0&&y<myconstant::WINDOW_HEIGHT)
    {
        m_YCoordinate=y;
        return y;
    }
    else
    {
        return false;
    }
}

void iSoldier::showInfoOfSoldier()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
}

void iSoldier::upgrade()
{
    //升级功能暂时不使用
}

void iSoldier::die()
{
    state=iSoldier::dead;
}

bool iSoldier::attack()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
    return true;
}

void iSoldier::showAttack()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
}

void iSoldier::showUpGrade()
{
    // 不再使用经验和升级系统
}

void iSoldier::showDie()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
}

void iSoldier::showWalk()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
}

void iSoldier::showRun()
{
    // 该函数功能已经在ItemForRole类中实现，不再使用这个函数
}

void iSoldier::showMiss()
{
    // 不使用躲避攻击的函数
}

void iSoldier::setSoldier(QString name, int id, int maxHP, int maxMP, int XCoordinate, int YCoordinate, int attackPower, int defense, int movingDistance, int level, int attackRange)
{
    m_name=name;
    m_ID=id;
    m_HPUpperLimit=maxHP;
    m_HP=maxHP;
    m_MPUpperLimit=maxMP;
    m_MP=maxMP;
    m_XCoordinate=XCoordinate;
    m_YCoordinate=YCoordinate;
    m_AttackPower=attackPower;
    m_Defense=defense;
    m_MaximumAttackRange=attackRange;
    m_MaximumMovingDistance=movingDistance;
    m_Level=level;
}

void iSoldier::resetForEveryRound()
{
    if(state!=iSoldier::dead)
    {
        state=init;
    }
}
