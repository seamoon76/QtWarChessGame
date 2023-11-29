// 怪物类
// 实现怪物的所有共同属性
// 马骐

#include "monster.h"

Monster::Monster(QWidget* parent)
    :QWidget(parent)
{

}

void Monster::setName(QString name)
{
    this->m_name=name;
}

QString Monster::getName()
{
    return this->m_name;
}

void Monster::setID(int ID)
{
    this->m_ID=ID;
}

int Monster::getID()
{
    return m_ID;
}

// 最大可设置到HP上限，返回设置后的值
int Monster::setHP(int HP)
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

int Monster::getHP()
{
    return m_HP;
}

int Monster::modifyHP(int change)
{
    if(m_HP+change<0)
    {
        m_HP=0;
        state=Monster::dead;
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
int Monster::setMP(int MP)
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

int Monster::getMP()
{
    return m_MP;
}

int Monster::modifyMP(int change)
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

int Monster::getXCoordinate()
{
    return m_XCoordinate;
}

int Monster::getYCoordinate()
{
    return m_YCoordinate;
}

bool Monster::setXCoordinate(int x)
{
    // 检查坐标是否合法
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

bool Monster::setYCoordinate(int y)
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

void Monster::showInfoOfSoldier()
{
    // 已在palywindow.cpp中实现
}

void Monster::upgrade()
{
    //不再使用升级函数
}

void Monster::die()
{
    state=dead;
}

bool Monster::attack()
{
    // 已在itemForRole类中实现
    return true;
}

void Monster::showAttack()
{
    // 已在itemForRole类中实现
}

void Monster::showUpGrade()
{
    // 不再使用
}

void Monster::showDie()
{
    // 已在itemForRole类中实现
}

void Monster::showWalk()
{
    // 已在itemForRole类中实现
}

void Monster::showRun()
{
    // 不使用奔跑动画
}

void Monster::showMiss()
{
    // 不使用躲避动画
}

void Monster::setMonster(QString name, int id, int maxHP, int maxMP, int XCoordinate, int YCoordinate, int attackPower, int defense, int movingDistance, int level, int attackRange)
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

void Monster::resetForEveryRound()
{
    if(state!=Monster::dead)
    {
        state=init;
    }
}

