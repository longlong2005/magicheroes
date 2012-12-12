#ifndef __BOARD_LAYER_H__
#define __BOARD_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class Soldier;

class BoardLayer : public CCLayer
{
public:
    void draw();
    void update();

    bool containsTouchLocation(CCTouch* touch);

protected:
    static const int BOARD_WIDTH = 8;
    static const int BOARD_HEIGHT = 6;
    static const int BASE_SIZE = 56;

    Soldier* board[BOARD_WIDTH][BOARD_HEIGHT];
    Soldier* touchedSoldier;

    CCPoint origin;
    CCSize size;

    int lastTouchedX;
    int lastTouchedY;

    void rearrange();
    
    int moveToTop(int x, int y);

    CCRect boundingBox();

protected:
    virtual CCPoint getPositionByIndex(int x, int y);

    virtual float getPositionXByIndex(int x);
    virtual float getPositionYByIndex(int y) = 0;

protected:
	BoardLayer();
	~BoardLayer();
};

class AttackerBoard : public BoardLayer
{
public:
    static AttackerBoard* create();

    void draw();
    void update();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    bool containsTouchLocation(CCTouch* touch);

protected:
    virtual float getPositionYByIndex(int y);

protected:
	AttackerBoard();
	~AttackerBoard();
};

class DefenderBoard : public BoardLayer
{
public:
    static DefenderBoard* create();

    void draw();
    void update();

protected:
    virtual float getPositionYByIndex(int y);

protected:
	DefenderBoard();
	~DefenderBoard();
};

#endif  // __BOARD_LAYER_H__

