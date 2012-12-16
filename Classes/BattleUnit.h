#ifndef __BATTLE_UNIT_H__
#define __BATTLE_UNIT_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

class BattleUnit : public CCNode, public CCTargetedTouchDelegate
{
public:
    virtual void draw();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

protected:
    BattleUnit();
    virtual ~BattleUnit();

    virtual bool init() = 0;

    int state;

    CCSprite* sprite;
};

#endif  // __BATTLE_UNIT_H__
