#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "cocos2d.h"
#include "AnimationManager.h"
#include "BattleUnit.h"

USING_NS_CC;

typedef enum _SoldierState 
{
    StateStand = 0,
    StateWall,
    StateCharge,
    StateAttack,
    StateUnderAttack,
    StateSelected,
    StateLeaveField,
    StateReturnField,
} SoldierState; 

typedef enum _SoldierColor 
{
    ColorRed = 0,
    ColorYellow,
    ColorGreen
} SoldierColor; 

class Soldier : public BattleUnit
{
public:
    static Soldier* create();

    virtual void draw();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    //for testing state changed
    virtual void selected();
    virtual void stand();

    void setColor(SoldierColor color);
    SoldierColor getColor();

protected:
    Soldier();
    virtual ~Soldier();

    virtual bool init();

    SoldierState state;
    SoldierColor color;

    int population;

    CCSprite* sprite;
};

#endif  // __SOLDIER_H__

