#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

typedef enum _SoldierState 
{
    StateStand = 0,
    StateSelected
} SoldierState; 

class Soldier : public CCNode, public CCTargetedTouchDelegate
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

protected:
    virtual bool init();

    Soldier();
    virtual ~Soldier();

    int state;

    CCSprite* sprite;
};

#endif  // __SOLDIER_H__

