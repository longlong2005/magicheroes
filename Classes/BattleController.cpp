#include "BattleController.h"
#include "Global.h"
#include "BackGroundLayer.h"

BattleController::BattleController()
{
}


BattleController::~BattleController()
{
}

BattleController* BattleController::create()
{
	BattleController* controller = NULL;

	do 
	{
		controller = new BattleController();
		CC_BREAK_IF(!controller);

        CC_BREAK_IF(!controller->init());

        controller->autorelease();

        return controller;

	} while(0);

	return NULL;
}

bool BattleController::init()
{
    CCTexture2D* texture = NULL;

    texture = CCTextureCache::sharedTextureCache()->addImage("CloseNormal.png");
    sprite = CCSprite::createWithTexture(texture);
    sprite->setAnchorPoint(CCPointMake(0, 0));
    sprite->setScale(2.0f);
    sprite->setPosition(CCPointMake(384, 20));

    addChild(sprite);

    return true;
}


void BattleController::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 99, true);
    CCNode::onEnter();
}

void BattleController::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool BattleController::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();
        
    if (!sprite->boundingBox().containsPoint(touchPoint))
        return false;

    GLOBAL->BattleBG->nextState();

    return true;
}

void BattleController::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    //CCPoint touchPoint = touch->getLocation();
}

void BattleController::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}
