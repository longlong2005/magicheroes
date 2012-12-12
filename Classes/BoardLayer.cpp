#include "BoardLayer.h"
#include "Global.h"
#include "Soldier.h"
#include "Spriter\SpriterNode.h"
#include "Spriter\CCSpriterX.h"

//BoardLayer///////////////////////////////////////////////////////////////////////////////////////////////
BoardLayer::BoardLayer()
{
    size = CCSize(BASE_SIZE * BOARD_WIDTH, BASE_SIZE * BOARD_HEIGHT);
}

BoardLayer::~BoardLayer()
{
}

void BoardLayer::draw()
{
    CCLayer::draw();

    CHECK_GL_ERROR_DEBUG();

    //glEnable(GL_LINE_SMOOTH);

    ccDrawColor4B(255,0,255,64);
    for (int j = 0; j <= BOARD_HEIGHT; j++) 
    {
        ccDrawLine(ccp(origin.x, origin.y + j * BASE_SIZE), 
            ccp(origin.x + BOARD_WIDTH * BASE_SIZE, origin.y + j * BASE_SIZE));
    }

    for (int i = 0; i <= BOARD_WIDTH; i ++) 
    {
        ccDrawLine(ccp(origin.x + i * BASE_SIZE, origin.y), 
            ccp(origin.x + i * BASE_SIZE, origin.y + BOARD_HEIGHT * BASE_SIZE));
    }

    //attacker board origin, index(0,0)
    ccDrawColor4B(255,0,0,128);
    ccDrawCircle(origin, 10, 360, 360, false, 1, 1);
}

void BoardLayer::update()
{
}

CCRect BoardLayer::boundingBox()
{
    return CCRectMake(origin.x, origin.y, size.width, size.height);
}

void BoardLayer::rearrange()
{
    //need to reorder child for render.
    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        int y = 0;
        for (int j = BOARD_HEIGHT - 1; j >= 0; j--) 
        {
            if (board[i][j])
            {
                y = moveToTop(i, j);
                board[i][y]->setPosition(getPositionByIndex(i, y));
            }
        }
    }
}

int BoardLayer::moveToTop(int x, int y)
{
    int finalY = y;

    Soldier** soldier = (Soldier**)board + BOARD_HEIGHT * x + y;

    if (y + 1 < BOARD_HEIGHT && *(soldier + 1) == NULL)
    {
        *(soldier + 1) = *soldier;
        
        (*soldier) = NULL;
        finalY = moveToTop(x, y + 1);
    }

    return finalY;
}

float BoardLayer::getPositionXByIndex(int x)
{
    return origin.x + x * BASE_SIZE;
}

CCPoint BoardLayer::getPositionByIndex(int x, int y)
{
    return CCPointMake(getPositionXByIndex(x), getPositionYByIndex(y));
}

//AttackerBoard///////////////////////////////////////////////////////////////////////////////////////////////
AttackerBoard::AttackerBoard()
{
    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height - BASE_SIZE) / 2 - size.height);
    
    //for test
    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        for (int j = 0; j < BOARD_HEIGHT; j++) 
        {
            board[i][j] = NULL;

            if (rand() % 100 < 30)
            {
                board[i][j] = Soldier::create();
                board[i][j]->setPosition(getPositionByIndex(i, j));

                GLOBAL->Attackers->addObject(board[i][j]);
                this->addChild(board[i][j], BOARD_HEIGHT * 2 - j);
            }
        }
    }

    rearrange();
}

AttackerBoard::~AttackerBoard()
{
    GLOBAL->Attackers->removeAllObjects();
}


AttackerBoard* AttackerBoard::create()
{
	AttackerBoard* layer = NULL;

	do 
	{
		layer = new AttackerBoard();
		CC_BREAK_IF(!layer);
        /*
        CCObject* attacker = NULL;
        CCARRAY_FOREACH(GLOBAL->Attackers, attacker)
        {
            layer->addChild((Soldier*)attacker);
        }

        CCObject* defender = NULL;
        CCARRAY_FOREACH(GLOBAL->Defenders, defender)
        {
            layer->addChild((Soldier*)defender);
        }
        */
      /*
        SpriterNode *n = SpriterNode::create( "Example.SCML" );
     
    n->setPosition( ccp(200, 100) );
    n->runAnimation( "Idle" );
     
    layer->addChild( n );

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "monster.plist" );
    SpriterNode *n = SpriterNode::create( "Example.SCML", "monster.png" );
     
    n->setPosition( ccp(600, 100) );
    n->runAnimation( "Posture" );
     
    layer->addChild( n );
        */
        ///////////////////////////////////////////////////////////////
        /*
        CCSpriterX *animator = CCSpriterX::create("Example.SCML");

		CC_BREAK_IF(! animator);

		animator->setPosition(ccp(240, 30));
		animator->setScale(0.8f);
		layer->addChild(animator, 0, 100);
        */
        ///////////////////////////////////////////////////////////////
        /*
        	SPX_Sprite* sprite = SPX_Manager::CreateSprite(L"boy.sprite");
	
	sprite->SetAction("walk");//Set action ID
	sprite->SetPosition(50, 30);
    sprite->setScale(0.5f);
    sprite->autorelease();

		layer->addChild(sprite, 10);
            */
        layer->autorelease();

        return layer;

	} while(0);

	return NULL;
}

void AttackerBoard::draw()
{
    BoardLayer::draw();
}

void AttackerBoard::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCNode::onEnter();
}

void AttackerBoard::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool AttackerBoard::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!boundingBox().containsPoint(touchPoint))
        return false;

    lastTouchedX = (touchPoint.x - origin.x) / BASE_SIZE;
    lastTouchedY = (touchPoint.y - origin.y) / BASE_SIZE;

    if (!board[lastTouchedX][lastTouchedY])
        return false;

    CCLog("move index x:%d, y:%d\n", lastTouchedX, lastTouchedY);

    board[lastTouchedX][lastTouchedY]->selected();
    this->reorderChild(board[lastTouchedX][lastTouchedY], BOARD_HEIGHT * 2);

    return true;
}

void AttackerBoard::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!boundingBox().containsPoint(touchPoint))
        return;

    touchedSoldier = board[lastTouchedX][lastTouchedY];
    if (!touchedSoldier)
        return;
    
    int x = (touchPoint.x - origin.x) / BASE_SIZE;
    int y = (touchPoint.y - origin.y) / BASE_SIZE;
    x = x == BOARD_WIDTH ? x - 1 : x;
    y = y == BOARD_HEIGHT ? y - 1 : y;

    //already have a soldier in this place
    if ((x == lastTouchedX && y == lastTouchedY) || board[x][y])
    {
        return;
    }

    board[x][y] = touchedSoldier;
    board[lastTouchedX][lastTouchedY] = NULL;
    lastTouchedX = x;
    lastTouchedY = y;

    touchedSoldier->setPosition(getPositionByIndex(x, y));
}

void AttackerBoard::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    this->reorderChild(board[lastTouchedX][lastTouchedY], BOARD_HEIGHT * 2 - lastTouchedY);
    board[lastTouchedX][lastTouchedY]->stand();

    rearrange();
}

float AttackerBoard::getPositionYByIndex(int y)
{
    return origin.y + y * BASE_SIZE;
}


//DefenderBoard///////////////////////////////////////////////////////////////////////////////////////////////
DefenderBoard::DefenderBoard()
{
    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height + BASE_SIZE) / 2);

    //for test
    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        for (int j = 0; j < BOARD_HEIGHT; j++) 
        {
            board[i][j] = NULL;

            if (rand() % 100 < 30)
            {
                board[i][j] = Soldier::create();
                board[i][j]->setPosition(getPositionByIndex(i, j));

                GLOBAL->Defenders->addObject(board[i][j]);
                this->addChild(board[i][j], j);
            }
        }
    }

    rearrange();
}

DefenderBoard::~DefenderBoard()
{
    GLOBAL->Defenders->removeAllObjects();
}

DefenderBoard* DefenderBoard::create()
{
	DefenderBoard* layer = NULL;

	do 
	{
		layer = new DefenderBoard();
		CC_BREAK_IF(!layer);

        layer->autorelease();

        return layer;

	} while(0);

	return NULL;
}

void DefenderBoard::draw()
{
    BoardLayer::draw();
}

float DefenderBoard::getPositionYByIndex(int y)
{
    return origin.y + (BOARD_HEIGHT - y - 1) * BASE_SIZE;
}
