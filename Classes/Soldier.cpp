#include "Soldier.h"

Soldier::Soldier()
{
    sprite = NULL;
    state = StateStand;
}

Soldier::~Soldier()
{
}

Soldier* Soldier::create()
{
	Soldier* soldier = NULL;

	do 
	{
		soldier = new Soldier();
		CC_BREAK_IF(!soldier);

        CC_BREAK_IF(!soldier->init());

        soldier->autorelease();

        return soldier;

	} while(0);

	return NULL;
}

bool Soldier::init()
{
    CCAnimation* animation = ANIMATION_MGR->getAnimation();
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("s1.png");
    CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(120 * 0, 0, 120, 120));

    sprite = CCSprite::createWithSpriteFrame(frame);
    sprite->setAnchorPoint(CCPointMake(0.2f, 0.15f));
    sprite->setScale(0.8f);
 
    addChild(sprite);

    CCAnimate* animate = CCAnimate::create(animation);
    sprite->runAction(CCRepeatForever::create(animate));

    return true;
}

void Soldier::draw()
{
    CCNode::draw();

    CHECK_GL_ERROR_DEBUG();
    //glEnable(GL_LINE_SMOOTH);
/*
    CCRect rect = sprite->boundingBox();
    ccDrawColor4B(255,255,255,255);
    ccDrawRect(rect.origin, CCPointMake(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height));

    rect = this->boundingBox();
    ccDrawColor4B(255,0,0,255);
    ccDrawRect(rect.origin, CCPointMake(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height));
*/
    //need to move these code to change state function
    //at here, just for testing 
    switch (state)
    {
    case StateStand:

        break;

    case StateSelected:
        {


        CCPoint origin = getPosition();
        //ccDrawSolidRect(CCPointMake(0, -960), CCPointMake(0 + 56, 960), ccc4f(255,0,0,8));
        ccDrawRect(CCPointMake(0, -960), CCPointMake(0 + 56, 960));
        }break;

    default:
        break;
    }
}

void Soldier::onEnter()
{
    CCNode::onEnter();
}

void Soldier::onExit()
{
    CCNode::onExit();
}

bool Soldier::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return false;
}

void Soldier::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void Soldier::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}

void Soldier::selected()
{
    sprite->setScale(1.6f);
    sprite->setAnchorPoint(CCPointMake(0.32f, 0.3f));

    state = StateSelected;
}

void Soldier::stand()
{
    sprite->setScale(0.8f);
    sprite->setAnchorPoint(CCPointMake(0.2f, 0.15f));
    
    state = StateStand;
}
