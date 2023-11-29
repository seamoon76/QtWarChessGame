#include "itemforeffect.h"

ItemForEffect::ItemForEffect():MyItem()
{
    itemType=MyItem::effect;

    //动画
    hurtMovieFromMonster=new QMovie(":/new/prefix1/image/effect/paw.gif");
    hurtMovieFromSoldier=new QMovie(":/new/prefix1/image/effect/hurt1.gif");
    // hurtMovieFromFire=new QMovie(":/new/prefix1/image/effect/hurt2.gif");
    // hurtMovieFromSkillLight=new QMovie(":/new/prefix1/image/effect/hurt3.gif");
    defenceUpMovie=new QMovie(":/new/prefix1/image/effect/addDefence.gif");
    addHPMovie=new QMovie(":/new/prefix1/image/effect/restoreBlood.gif");
    // skillMovieBlueFire=new QMovie(":/new/prefix1/image/effect/defence.gif");
    skillMovieOfBOSS=new QMovie(":/new/prefix1/image/effect/big.gif");
    // fireMovie=new QMovie(":/new/prefix1/image/effect/fire.gif");

    // 音效
    soundForSoldierAttack=new QSound(":/music/music/effectSound/robot_attack_metal.wav");
    soundForWalk=new QSound(":/music/music/effectSound/robot_move_01.wav");
    soundForMonsterAttack=new QSound(":/music/music/effectSound/enemy_attack_03.wav");
    soundForWin=new QSound(":/music/music/effectSound/mus_victory.wav");
    soundForFail=new QSound(":/music/music/effectSound/fail.wav");
    soundForAddBlood=new QSound(":/music/music/effectSound/addBlood.wav");
}

void ItemForEffect::setMovie(QMovie* movie)
{
    prepareGeometryChange();
    QObject::disconnect(mConnection); // disconnect old object
    m_Movie = movie;
    if (m_Movie)
        mConnection = QObject::connect(m_Movie, &QMovie::frameChanged, [=]{ update(); });
}

void ItemForEffect::paint(QPainter* painter,
           const QStyleOptionGraphicsItem * option,
           QWidget* widget)
{
    if(m_Movie && m_Movie->state() == QMovie::Running)
    {
        if (m_Movie)
        {
            painter->drawPixmap(boundingRect(), m_Movie->currentPixmap(),
                                m_Movie->frameRect());
        }
    }
    else
    {
        MyItem::paint(painter,option,widget);
    }
}
