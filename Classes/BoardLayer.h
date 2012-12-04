#ifndef __BOARD_LAYER_H__
#define __BOARD_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class Soldier;

class BoardLayer : public CCLayer
{
public:
    static BoardLayer* create();

    void draw();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    bool containsTouchLocation(CCTouch* touch);

protected:
    static const int BOARD_WIDTH = 8;
    static const int BOARD_HEIGHT = 6;
    static const int BASE_SIZE = 56;

    Soldier* atkBoard[BOARD_WIDTH][BOARD_HEIGHT];
    Soldier* dfnBoard[BOARD_WIDTH][BOARD_HEIGHT];

    Soldier* touchedSoldier;

    CCPoint atkBoardOrigin;
    CCPoint dfnBoardOrigin;

    CCSize size;

    int lastTouchedX;
    int lastTouchedY;

    void rearrange();
    //void moveToTop(Soldier* board[][BOARD_HEIGHT], int x, int y);
    int moveToTop(Soldier*** board, int x, int y);

    CCRect atkBoardRect();

protected:
	BoardLayer();
	~BoardLayer();
};

#endif  // __BOARD_LAYER_H__

