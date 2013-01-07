#ifndef __BATTLE_FIELD_H__
#define __BATTLE_FIELD_H__

#include "GameBasic.h"

class Soldier;

class BattleField : public CCLayer
{
public:
    void draw();
    void update();

    Soldier* lastSoldierInColumn(int column);
    Soldier* firstSoldierInColumn(int column);
    Soldier* getSoldierByIndex(const GridIndex& index);
    Soldier* getSoldierByPosition(CCPoint position);

    Soldier** soldiersInColumn(int column);
    Soldier** soldiersInRow(int row);

    bool appendSoldier(int column, Soldier* soldier);
    void insertSoldier(int column, int row, Soldier* soldier);
    void removeSoldier(Soldier* soldier);
    void removeSoldiers(CCArray* soldiers);
    void removeSoldierByIndex(const GridIndex& index);
    void reformSoldierInColumn(int column);

    void createReinforcements();
    void sendSoldiersToField();

    bool changeIntoAttackFormation(int column, int row);
    bool changeIntoDefendWall(int column, int row);

    bool containsTouchLocation(CCTouch* touch);

protected:
    GridIndex getIndexByPosition(CCPoint point);

    virtual CCPoint getPositionByIndex(const GridIndex& index);

    virtual float getPositionXByColumn(int column);
    virtual float getPositionYByRow(int row) = 0;
    
    virtual int getColumnByPosition(float x);
    virtual int getRowByPosition(float y) = 0;

    bool getEnableGridIndex(int &x, int &y); //for test function

    CCRect boundingBox();
    
    Soldier* field[FIELD_COLUMN_MAX][FIELD_ROW_MAX];
    Soldier* touchedSoldier;
    Soldier* rowSoldiers[FIELD_COLUMN_MAX];

    CCArray* allSoldiers;
    CCArray* reinforcements;
    CCArray* movedSoldiers;
    CCArray* needToRearrange;

    CCPoint origin;
    CCSize size;

    GridIndex lastIndex;

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
    virtual float getPositionYByRow(int row);
    virtual int getRowByPosition(float y);

    bool init();

    void onAtkSoldierCompleteMove(CCObject* obj);

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
    virtual float getPositionYByRow(int row);
    virtual int getRowByPosition(float y);

protected:
	DefenderField();
	~DefenderField();
};

#endif  // __BATTLE_FIELD_H__

