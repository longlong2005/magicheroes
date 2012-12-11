#ifndef __BACK_GROUND_LAYER_H__
#define __BACK_GROUND_LAYER_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

class BackGroundLayer : public CCLayer
{
public:
    static BackGroundLayer* create();

    virtual void onEnter();
    virtual void onExit();

    //virtual void draw();

    void nextState();

protected:
    virtual bool init();

    CCSprite* sprite1;
    CCSprite* sprite2;
    CCSprite* sprite3;
    CCSprite* sprite4;
    CCSprite* sprite5;

    int state;

protected:
    BackGroundLayer();
    ~BackGroundLayer();
};


#endif  // __BACK_GROUND_LAYER_H__


