#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "GameBasic.h"
#include "BattleUnit.h"

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
    
    virtual void moveTo(CCPoint position, float duration=0.5f);
    
    void completeMove();

    bool isAlone();
    bool isSelected();

    //for testing state changed
    virtual void selected();
    virtual void stand();
    virtual void wall();
    virtual void charge();

    void setColor(SoldierColor color);
    SoldierColor getColor();

    void setGridIndex(GridIndex index);
    GridIndex getGridIndex();

protected:
    Soldier();
    virtual ~Soldier();

    virtual bool init();

    SoldierState state;
    SoldierColor color;
    ccColor4F rColor;
    GridIndex index;

    CCSize size;

    int population;

    CCSprite* sprite;
};

#endif  // __SOLDIER_H__

