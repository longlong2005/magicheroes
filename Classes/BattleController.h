#ifndef __BATTLE_CONTROLLER_H__
#define __BATTLE_CONTROLLER_H__

#include "GameBasic.h"

class BattleController : public CCLayer
{
public:
    static BattleController* create();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

protected:
    virtual bool init();

    CCSprite* sprite1; //background test
    CCSprite* sprite2; //return game main menu
    CCSprite* sprite3; //reinforcements

protected:
    BattleController();
    ~BattleController();
};


#endif  // __BATTLE_CONTROLLER_H__