#include "Hero.h"
#include "BattleField.h"

Hero::Hero()
{
    sprite = NULL;
    state = 0;

    reinforcement = CCArray::create();
    reinforcement->retain();
}

Hero::~Hero()
{
    reinforcement->release();
}

Hero* Hero::create()
{
	Hero* hero = NULL;

	do 
	{
		hero = new Hero();
		CC_BREAK_IF(!hero);

        CC_BREAK_IF(!hero->init());

        hero->autorelease();

        return hero;

	} while(0);

	return NULL;
}

bool Hero::init()
{
    return true;
}

void Hero::draw()
{
    CCNode::draw();
}

void Hero::onEnter()
{
    CCNode::onEnter();
}

void Hero::onExit()
{
    CCNode::onExit();
}

bool Hero::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return false;
}

void Hero::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void Hero::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}
