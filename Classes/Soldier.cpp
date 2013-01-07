#include "Soldier.h"
#include "BattleField.h"

Soldier::Soldier()
    :sprite(NULL)
    ,state(StateStand)
    ,rColor(ccc4f(0,0,0,16))
    ,size(CCSize(1,1))
{
    SoldierColor allColors[] = {ColorRed, ColorYellow, ColorGreen};
    color = allColors[rand() % 3];
    if (color == ColorRed || color == ColorYellow)
    {
        rColor.r = 255;
    }
    
    if (color == ColorYellow || color == ColorGreen)
    {
        rColor.g = 255;
    }

    setContentSize(CCSize(FIELD_GRID_WIDTH, FIELD_GRID_HEIGHT));

    //CCLog("Soldier color, r:%f", rColor.r);
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
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("s2.png");
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

    CCSize size = getContentSize();
    ccDrawSolidRect(CCPointMake(size.width / 6, size.height / 6), CCPointMake(size.width * 5 / 6, size.height * 5 / 6), rColor);

    //need to move these code to change state function
    //at here, just for testing 
    switch (state)
    {
    case StateStand:

        break;

    case StateSelected:
        {
            ccDrawRect(CCPointMake(0, -960), CCPointMake(size.width, 960));
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

void Soldier::moveTo(CCPoint position, float duration)
{
    CCArray* actions = CCArray::create();
    actions->addObject(CCMoveTo::create(duration, position));
    actions->addObject(CCCallFunc::create(this, callfunc_selector(Soldier::completeMove)));
    runAction(CCSequence::create(actions));

    actions->release();
}

bool Soldier::isAlone()
{
    return (state != StateCharge) && (state != StateWall);
}

bool Soldier::isSelected()
{
    return state == StateSelected;
}

void Soldier::setColor(SoldierColor color)
{
    this->color = color;
}

SoldierColor Soldier::getColor()
{
    return color;
}

void Soldier::setGridIndex(GridIndex index)
{
    this->index = index;
}

GridIndex Soldier::getGridIndex()
{
    return index;
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

void Soldier::wall()
{
    sprite->setVisible(false);

    state = StateWall;
}

void Soldier::charge()
{
    sprite->setVisible(false);

    state = StateCharge;
}

void Soldier::completeMove()
{
    NOTIFY->postNotification(MSG_ATK_SOLDIER_COMPLETE_MOVE, this);
}
