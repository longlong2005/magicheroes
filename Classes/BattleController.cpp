#include "BattleController.h"
#include "Global.h"
#include "BackGroundLayer.h"
#include "GameMainMenu.h"

BattleController::BattleController()
{
    sprite1 = NULL;
    sprite2 = NULL;
    sprite3 = NULL;
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
    sprite1 = CCSprite::createWithTexture(texture);
    sprite1->setAnchorPoint(CCPointMake(0, 0));
    sprite1->setScale(2.0f);
    sprite1->setPosition(CCPointMake(184, 20));

    addChild(sprite1);

    texture = CCTextureCache::sharedTextureCache()->addImage("CloseSelected.png");
    sprite2 = CCSprite::createWithTexture(texture);
    sprite2->setAnchorPoint(CCPointMake(0, 0));
    sprite2->setScale(2.0f);
    sprite2->setPosition(CCPointMake(584, 20));

    addChild(sprite2);

    texture = CCTextureCache::sharedTextureCache()->addImage("CloseSelected.png");
    sprite3 = CCSprite::createWithTexture(texture);
    sprite3->setAnchorPoint(CCPointMake(0, 0));
    sprite3->setScale(2.0f);
    sprite3->setColor(ccc3(0, 128, 0));
    sprite3->setPosition(CCPointMake(384, 20));

    addChild(sprite3);

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
        
    if (sprite1->boundingBox().containsPoint(touchPoint))
    {
        GLOBAL->BattleBG->nextState();
        return true;
    }
    
    if (sprite2->boundingBox().containsPoint(touchPoint))
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1.0f, GameMainMenu::create()));
        return true;
    }

    return false;
}

void BattleController::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    //CCPoint touchPoint = touch->getLocation();
}

void BattleController::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}
