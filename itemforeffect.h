#ifndef ITEMFOREFFECT_H
#define ITEMFOREFFECT_H

// 实现战斗动画、技能动画效果的类
// 马骐

#include "myitem.h"
#include<QMovie>
#include<QSound>

// 继承自MyItem(自定义的图元类)，专门用于播放特殊动画
class ItemForEffect : public MyItem
{
    Q_OBJECT
public:
    ItemForEffect();
    // 用于设置动画的接口函数
    void setMovie(QMovie* movie);
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem * option,
               QWidget* widget)override;

public:
    // 下面这些指针用于指向战斗动画、防御力增加等动画效果的gif文件
    QMovie* m_Movie=nullptr;
    QMovie* hurtMovieFromMonster=nullptr;
    QMovie* hurtMovieFromSoldier=nullptr;
    QMovie* hurtMovieFromFire=nullptr;
    QMovie* hurtMovieFromSkillLight=nullptr;
    QMovie* defenceUpMovie=nullptr;
    QMovie* addHPMovie=nullptr;
    QMovie* fireMovie=nullptr;
    QMovie* skillMovieBlueFire=nullptr;
    QMovie* skillMovieOfBOSS=nullptr;
    // 下面这些指针用于指向战斗音效、技能音效等wav文件
    QSound* soundForSoldierAttack=nullptr;
    QSound* soundForWalk=nullptr;
    QSound* soundForMonsterAttack=nullptr;
    QSound* soundForWin=nullptr;
    QSound* soundForFail=nullptr;
    QSound* soundForAddBlood=nullptr;

private:
    QMetaObject::Connection mConnection;

};

#endif // ITEMFOREFFECT_H
