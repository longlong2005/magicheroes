#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "AnimationManager.h"
#include "BattleUnit.h"

USING_NS_CC;

class Hero : public BattleUnit
{
public:
    static Hero* create();

    virtual void draw();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

protected:
    Hero();
    virtual ~Hero();

    virtual bool init();

    CCArray* reinforcement;

    int state;

    CCSprite* sprite;
};

#endif  // __HERO_H__
