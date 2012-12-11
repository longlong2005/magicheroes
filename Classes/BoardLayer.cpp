#include "BoardLayer.h"
#include "Global.h"
#include "Soldier.h"
#include "Spriter\SpriterNode.h"
#include "Spriter\CCSpriterX.h"

BoardLayer::BoardLayer()
{
    size = CCSize(BASE_SIZE * BOARD_WIDTH, BASE_SIZE * BOARD_HEIGHT);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    atkBoardOrigin =  CCPoint((winSize.width - size.width) / 2, winSize.height / 2 - size.height - BASE_SIZE / 2);
    dfnBoardOrigin =  CCPoint((winSize.width - size.width) / 2, winSize.height / 2 + size.height + BASE_SIZE / 2);

    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        for (int j = 0; j < BOARD_HEIGHT; j++) 
        {
            atkBoard[i][j] = NULL;
            dfnBoard[i][j] = NULL;

            if (rand() % 100 < 30)
            {
                atkBoard[i][j] = Soldier::create();
                atkBoard[i][j]->setPosition(atkPosition(i, j));

                GLOBAL->Attackers->addObject(atkBoard[i][j]);
                this->addChild(atkBoard[i][j], BOARD_HEIGHT * 2 - j);
            }

            if (rand() % 100 < 30)
            {
                dfnBoard[i][j] = Soldier::create();
                dfnBoard[i][j]->setPosition(dfnPosition(i, j));

                GLOBAL->Defenders->addObject(dfnBoard[i][j]);
                this->addChild(dfnBoard[i][j], j);
            }
        }
    }

    rearrange();
}

BoardLayer::~BoardLayer()
{
}

BoardLayer* BoardLayer::create()
{
	BoardLayer* layer = NULL;

	do 
	{
		layer = new BoardLayer();
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

void BoardLayer::draw()
{
    CCLayer::draw();

    CHECK_GL_ERROR_DEBUG();

    //glEnable(GL_LINE_SMOOTH);

    //attacker board
    ccDrawColor4B(255,0,255,64);
    for (int j = 0; j <= BOARD_WIDTH; j++) 
    {
        ccDrawLine(ccp(atkBoardOrigin.x, atkBoardOrigin.y + j * BASE_SIZE), 
            ccp(atkBoardOrigin.x + BOARD_WIDTH * BASE_SIZE, atkBoardOrigin.y + j * BASE_SIZE));
    }

    for (int i = 0; i <= BOARD_WIDTH; i ++) 
    {
        ccDrawLine(ccp(atkBoardOrigin.x + i * BASE_SIZE, atkBoardOrigin.y), 
            ccp(atkBoardOrigin.x + i * BASE_SIZE, atkBoardOrigin.y + BOARD_HEIGHT * BASE_SIZE));
    }

    //attacker board origin, index(0,0)
    ccDrawColor4B(255,0,0,128);
    ccDrawCircle(atkBoardOrigin, 10, 360, 360, false, 1, 1);

    //defender board
    ccDrawColor4B(255,0,255,64);
    for (int j = 0; j <= BOARD_WIDTH; j++) 
    {
        ccDrawLine(ccp(dfnBoardOrigin.x, dfnBoardOrigin.y - j * BASE_SIZE), 
            ccp(dfnBoardOrigin.x + BOARD_WIDTH * BASE_SIZE, dfnBoardOrigin.y - j * BASE_SIZE));
    }

    for (int i = 0; i <= BOARD_WIDTH; i ++) 
    {
        ccDrawLine(ccp(dfnBoardOrigin.x + i * BASE_SIZE, dfnBoardOrigin.y), 
            ccp(dfnBoardOrigin.x + i * BASE_SIZE, dfnBoardOrigin.y - BOARD_HEIGHT * BASE_SIZE));
    }

    //defender board origin, index(0,0)
    ccDrawColor4B(255,0,0,128);
    ccDrawCircle(dfnBoardOrigin, 10, 360, 360, false, 1, 1);
}

void BoardLayer::update()
{
}

void BoardLayer::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCNode::onEnter();
}

void BoardLayer::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool BoardLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!atkBoardRect().containsPoint(touchPoint))
        return false;

    lastTouchedX = (touchPoint.x - atkBoardOrigin.x) / BASE_SIZE;
    lastTouchedY = (touchPoint.y - atkBoardOrigin.y) / BASE_SIZE;

    if (!atkBoard[lastTouchedX][lastTouchedY])
        return false;

    CCLog("move index x:%d, y:%d\n", lastTouchedX, lastTouchedY);

    atkBoard[lastTouchedX][lastTouchedY]->selected();
    this->reorderChild(atkBoard[lastTouchedX][lastTouchedY], BOARD_HEIGHT * 2);

    return true;
}

void BoardLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!atkBoardRect().containsPoint(touchPoint))
        return;

    touchedSoldier = atkBoard[lastTouchedX][lastTouchedY];
    if (!touchedSoldier)
        return;
    
    int x = (touchPoint.x - atkBoardOrigin.x) / BASE_SIZE;
    int y = (touchPoint.y - atkBoardOrigin.y) / BASE_SIZE;
    x = x == BOARD_WIDTH ? x - 1 : x;
    y = y == BOARD_HEIGHT ? y - 1 : y;

    //already have a soldier in this place
    if ((x == lastTouchedX && y == lastTouchedY) || atkBoard[x][y])
    {
        return;
    }

    atkBoard[x][y] = touchedSoldier;
    atkBoard[lastTouchedX][lastTouchedY] = NULL;
    lastTouchedX = x;
    lastTouchedY = y;

    touchedSoldier->setPosition(atkPosition(x, y));
}

void BoardLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    this->reorderChild(atkBoard[lastTouchedX][lastTouchedY], BOARD_HEIGHT * 2 - lastTouchedY);
    atkBoard[lastTouchedX][lastTouchedY]->stand();

    rearrange();
}

CCRect BoardLayer::atkBoardRect()
{
    return CCRectMake(atkBoardOrigin.x, atkBoardOrigin.y, size.width, size.height);
}

void BoardLayer::rearrange()
{
    //need to reorder child for render.
    for (int i = 0; i < BOARD_WIDTH; i++) 
    {
        int y = 0;
        for (int j = BOARD_HEIGHT - 1; j >= 0; j--) 
        {
            if (atkBoard[i][j])
            {
                y = moveToTop((Soldier***)atkBoard, i, j);
                atkBoard[i][y]->setPosition(atkPosition(i, y));
            }
            
            if (dfnBoard[i][j])
            {
                y = moveToTop((Soldier***)dfnBoard, i, j);
                dfnBoard[i][y]->setPosition(dfnPosition(i, y));
            }
        }
    }
}

int BoardLayer::moveToTop(Soldier*** board, int x, int y)
{
    int finalY = y;

    Soldier** soldier = (Soldier**)board + BOARD_HEIGHT * x + y;

    if (y + 1 < BOARD_HEIGHT && *(soldier + 1) == NULL)
    {
        *(soldier + 1) = *soldier;
        
        (*soldier) = NULL;
        finalY = moveToTop(board, x, y + 1);
    }

    return finalY;
}

CCPoint BoardLayer::atkPosition(int x, int y)
{
    return CCPointMake(atkBoardOrigin.x + x * BASE_SIZE, atkBoardOrigin.y + y * BASE_SIZE);
}

CCPoint BoardLayer::dfnPosition(int x, int y)
{
    return CCPointMake(dfnBoardOrigin.x + x * BASE_SIZE, dfnBoardOrigin.y - (y + 1) * BASE_SIZE);
}