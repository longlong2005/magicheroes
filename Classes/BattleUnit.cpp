#include "BattleUnit.h"
#include "BattleField.h"

BattleUnit::BattleUnit()
{
    sprite = NULL;
    state = 0;
}

BattleUnit::~BattleUnit()
{
}

void BattleUnit::draw()
{
    CCNode::draw();
}

void BattleUnit::onEnter()
{
    CCNode::onEnter();
}

void BattleUnit::onExit()
{
    CCNode::onExit();
}

bool BattleUnit::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return false;
}

void BattleUnit::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void BattleUnit::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}
