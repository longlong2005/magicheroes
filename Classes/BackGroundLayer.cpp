#include "BackGroundLayer.h"


BackGroundLayer::BackGroundLayer()
{
    sprite1 = NULL;
    sprite2 = NULL;
    sprite3 = NULL;
    sprite4 = NULL;
    sprite5 = NULL;

    state = 0;
}


BackGroundLayer::~BackGroundLayer()
{
}

BackGroundLayer* BackGroundLayer::create()
{
	BackGroundLayer* bg = NULL;

	do 
	{
		bg = new BackGroundLayer();
		CC_BREAK_IF(!bg);

        CC_BREAK_IF(!bg->init());

        bg->autorelease();

        return bg;

	} while(0);

	return NULL;
}

bool BackGroundLayer::init()
{
    CCTexture2D* texture = NULL;

    texture = CCTextureCache::sharedTextureCache()->addImage("bg_1.jpg");
    sprite1 = CCSprite::createWithTexture(texture);
    sprite1->setAnchorPoint(CCPointMake(0, 0));
 
    texture = CCTextureCache::sharedTextureCache()->addImage("bg_2.jpg");
    sprite2 = CCSprite::createWithTexture(texture);
    sprite3 = CCSprite::createWithTexture(texture);
    sprite2->setAnchorPoint(CCPointMake(0, 0));
    sprite3->setAnchorPoint(CCPointMake(0, 0));
    sprite3->setPosition(CCPointMake(0, 512));

    texture = CCTextureCache::sharedTextureCache()->addImage("bg_3.jpg");
    sprite4 = CCSprite::createWithTexture(texture);
    sprite5 = CCSprite::createWithTexture(texture);
    sprite4->setAnchorPoint(CCPointMake(0, 0));
    sprite5->setAnchorPoint(CCPointMake(0, 0));
    sprite5->setPosition(CCPointMake(384, 0));
    sprite5->setFlipX(true);

    addChild(sprite1);
    addChild(sprite2);
    addChild(sprite3);
    addChild(sprite4);
    addChild(sprite5);

    nextState();

    return true;
}

void BackGroundLayer::onEnter()
{
    CCNode::onEnter();
}

void BackGroundLayer::onExit()
{
    CCNode::onExit();
}

void BackGroundLayer::nextState()
{
    if (++state >= 3) state = 0;

    sprite1->setVisible(false);
    sprite2->setVisible(false);
    sprite3->setVisible(false);
    sprite4->setVisible(false);
    sprite5->setVisible(false);
    
    if (state == 0)
    {
        sprite1->setVisible(true);
    }
    else if (state == 1)
    {
        sprite2->setVisible(true);
        sprite3->setVisible(true);
    }
    else
    {
        sprite4->setVisible(true);
        sprite5->setVisible(true);
    }
}