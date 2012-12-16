#ifndef __BATTLE_FIELD_H__
#define __BATTLE_FIELD_H__

#include "cocos2d.h"

USING_NS_CC;

class Soldier;

class BattleField : public CCLayer
{
public:
    void draw();
    void update();

    Soldier* getLastSoldier(int x);
    Soldier* getHeadSoldier(int x);
    Soldier* getSoldierByIndex(int x, int y);
    Soldier* getSoldierByPosition(CCPoint position);

    bool appendSoldier(int x, Soldier* soldier);
    bool removeSoldier(Soldier* soldier);
    bool removeSoldierByIndex(int x, int y);

    bool containsTouchLocation(CCTouch* touch);

    static const int FIELD_WIDTH = 8;
    static const int FIELD_HEIGHT = 6;
    static const int BASE_TILE_WIDTH = 64;
    static const int BASE_TILE_HEIGHT = 56;

protected:
    virtual CCPoint getPositionByIndex(int x, int y);

    virtual float getPositionXByIndex(int x);
    virtual float getPositionYByIndex(int y) = 0;
    
    void rearrange();
    
    int moveToTop(int x, int y);

    CCRect boundingBox();
    
    Soldier* field[FIELD_WIDTH][FIELD_HEIGHT];
    Soldier* touchedSoldier;
    CCArray* reinforcement;
    CCPoint origin;
    CCSize size;

    int lastTouchedX;
    int lastTouchedY;

protected:
	BattleField();
	~BattleField();
};

class AttackerField : public BattleField
{
public:
    static AttackerField* create();

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
	AttackerField();
	~AttackerField();
};

class DefenderField : public BattleField
{
public:
    static DefenderField* create();

    void draw();
    void update();

protected:
    virtual float getPositionYByIndex(int y);

protected:
	DefenderField();
	~DefenderField();
};

#endif  // __BATTLE_FIELD_H__

