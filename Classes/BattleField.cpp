#include "BattleField.h"
#include "Global.h"
#include "Soldier.h"
#include "Spriter\SpriterNode.h"

//BattleField///////////////////////////////////////////////////////////////////////////////////////////////
BattleField::BattleField()
    :touchedSoldier(NULL)
    ,lastTouchedX(0)
    ,lastTouchedY(0)
    ,allSoldiers(CCArray::create())
    ,reinforcements(CCArray::create())
{
    memset(field, 0, FIELD_WIDTH * FIELD_HEIGHT * sizeof(Soldier*));
    size = CCSize(BASE_TILE_WIDTH * FIELD_WIDTH, BASE_TILE_HEIGHT * FIELD_HEIGHT);
}

BattleField::~BattleField()
{
}

void BattleField::draw()
{
    CCLayer::draw();

    CHECK_GL_ERROR_DEBUG();

    //glEnable(GL_LINE_SMOOTH);

    ccDrawColor4B(255,0,255,64);
    for (int j = 0; j <= FIELD_HEIGHT; j++) 
    {
        ccDrawLine(ccp(origin.x, origin.y + j * BASE_TILE_HEIGHT), 
            ccp(origin.x + FIELD_WIDTH * BASE_TILE_WIDTH, origin.y + j * BASE_TILE_HEIGHT));
    }

    ccDrawColor4B(255,0,255,64);
    for (int i = 0; i <= FIELD_WIDTH; i ++) 
    {
        ccDrawLine(ccp(origin.x + i * BASE_TILE_WIDTH, origin.y), 
            ccp(origin.x + i * BASE_TILE_WIDTH, origin.y + FIELD_HEIGHT * BASE_TILE_HEIGHT));
    }

    //attacker field origin, index(0,0)
    ccDrawColor4B(255,0,0,128);
    ccDrawCircle(origin, 10, 360, 360, false, 1, 1);
}

void BattleField::update()
{
}

CCRect BattleField::boundingBox()
{
    return CCRectMake(origin.x, origin.y, size.width, size.height);
}

void BattleField::rearrange()
{
    //need to reorder child for render.
    for (int i = 0; i < FIELD_WIDTH; i++) 
    {
        int y = 0;
        for (int j = FIELD_HEIGHT - 1; j >= 0; j--) 
        {
            if (field[i][j])
            {
                y = moveToTop(i, j);
                field[i][y]->setPosition(getPositionByIndex(i, y));
            }
        }
    }
}

int BattleField::moveToTop(int x, int y)
{
    int finalY = y;

    Soldier** soldier = (Soldier**)field + FIELD_HEIGHT * x + y;

    if (y + 1 < FIELD_HEIGHT && *(soldier + 1) == NULL)
    {
        *(soldier + 1) = *soldier;

        (*soldier) = NULL;
        finalY = moveToTop(x, y + 1);
    }

    return finalY;
}

Soldier* BattleField::getLastSoldier(int x)
{
    CCAssert(x >= 0 && x < FIELD_WIDTH, sprintf("Invalid column index:%d", x));

    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        if (field[x][i]) return field[x][i];
    }

    return NULL;
}

Soldier* BattleField::getHeadSoldier(int x)
{
    CCAssert(x >= 0 && x < FIELD_WIDTH, sprintf("Invalid column index:%d", x));

    for (int i = FIELD_HEIGHT; i >= 0; i--)
    {
        if (field[x][i]) return field[x][i];
    }

    return NULL;
}

Soldier* BattleField::getSoldierByIndex(int x, int y)
{
    CCAssert(x >= 0 && x < FIELD_WIDTH, sprintf("Invalid column index:%d", x));
    CCAssert(y >= 0 && y < FIELD_HEIGHT, sprintf("Invalid row index:%d", y));

    return field[x][y];
}

Soldier* BattleField::getSoldierByPosition(CCPoint position)
{
    int x = position.x / BASE_TILE_WIDTH;
    int y = position.y / BASE_TILE_HEIGHT;

    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT)
        return field[x][y];

    return NULL;
}
    
bool BattleField::appendSoldier(int x, Soldier* soldier)
{
    CCAssert(x >= 0 && x < FIELD_WIDTH, sprintf("Invalid column index:%d", x));

    for (int i = FIELD_HEIGHT - 1; i >= 0; i--)
    {
        if (field[x][i] == NULL)
        {
            field[x][i] = soldier;
            return true;
        }
    }

    return false;
}

bool BattleField::removeSoldier(Soldier* soldier)
{
    for (int i = 0; i < FIELD_WIDTH; i++)
    {
        for (int j = 0; j < FIELD_HEIGHT; j++)
        {
            if (field[i][j] == soldier)
            {
                field[i][j] = NULL;
                return true;
            }
        }
    }

    return false;
}

bool BattleField::removeSoldierByIndex(int x, int y)
{
    if (getSoldierByIndex(x, y))
    {
        field[x][y] = NULL;
        //return to reinforcements?
        return true;
    }

    return false;
}

float BattleField::getPositionXByIndex(int x)
{
    return origin.x + x * BASE_TILE_WIDTH;
}

CCPoint BattleField::getPositionByIndex(int x, int y)
{
    return CCPointMake(getPositionXByIndex(x), getPositionYByIndex(y));
}

void BattleField::createReinforcements()
{
    int unitCount = 30;

    for (int i = 0; i < unitCount; i++)
    {
        //set color, set type, get them from hero
        Soldier* soldier = Soldier::create();
        allSoldiers->addObject(soldier);
        reinforcements->addObject(soldier);
    }
}

void BattleField::sendSoldiersToField()
{
    int x = 0;
    int y = 0;

    CCObject* soldier = NULL;
    CCARRAY_FOREACH(reinforcements, soldier)
    {
        if (soldier == NULL) break;

        if (getEnableTileIndex(x, y))
        {
            field[x][y] = (Soldier*)soldier;
            field[x][y]->setPosition(getPositionByIndex(x, y));
            addChild(field[x][y], y);
        }

        if (++x >= FIELD_WIDTH) x = 0;
    }
}

bool BattleField::getEnableTileIndex(int &x, int &y)
{
    for (int i = FIELD_HEIGHT - 1; i >= 0; i--)
    {
        if (field[x][i] != NULL) continue;

        y = i;
        //soldier size?
        return true;
    }

    return false;
}

//AttackerField///////////////////////////////////////////////////////////////////////////////////////////////
AttackerField::AttackerField()
{
    GLOBAL->Attackers = allSoldiers;

    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height - BASE_TILE_HEIGHT) / 2 - size.height);
}

AttackerField::~AttackerField()
{
}


AttackerField* AttackerField::create()
{
    AttackerField* layer = NULL;

    do 
    {
        layer = new AttackerField();
        CC_BREAK_IF(!layer);

        layer->createReinforcements();
        layer->sendSoldiersToField();

        layer->autorelease();

        return layer;

    } while(0);

    return NULL;
}

void AttackerField::draw()
{
    BattleField::draw();
}

void AttackerField::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCNode::onEnter();
}

void AttackerField::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool AttackerField::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!boundingBox().containsPoint(touchPoint))
        return false;

    return true;
}

void AttackerField::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!boundingBox().containsPoint(touchPoint))
        return;
    /*
    touchedSoldier = field[lastTouchedX][lastTouchedY];
    if (!touchedSoldier)
    return;

    int x = (touchPoint.x - origin.x) / BASE_TILE_WIDTH;
    int y = (touchPoint.y - origin.y) / BASE_TILE_HEIGHT;
    x = x == FIELD_WIDTH ? x - 1 : x;
    y = y == FIELD_HEIGHT ? y - 1 : y;

    //already have a soldier in this place
    if ((x == lastTouchedX && y == lastTouchedY) || field[x][y])
    {
    return;
    }

    field[x][y] = touchedSoldier;
    field[lastTouchedX][lastTouchedY] = NULL;
    lastTouchedX = x;
    lastTouchedY = y;

    touchedSoldier->setPosition(getPositionByIndex(x, y));
    */
}

void AttackerField::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    //this->reorderChild(field[lastTouchedX][lastTouchedY], FIELD_HEIGHT * 2 - lastTouchedY);
    //field[lastTouchedX][lastTouchedY]->stand();

    CCPoint touchPoint = touch->getLocation();
    int currTouchedX = (touchPoint.x - origin.x) / BASE_TILE_WIDTH;
    int currTouchedY = (touchPoint.y - origin.y) / BASE_TILE_HEIGHT;

    if (!touchedSoldier)
    {
        Soldier* soldier = getLastSoldier(currTouchedX);
        if (!soldier)
            return;

        CCLog("move index x:%d, y:%d\n", lastTouchedX, lastTouchedY);

        soldier->selected();
        this->reorderChild(soldier, FIELD_HEIGHT * 2);
        touchedSoldier = soldier;
        lastTouchedX = currTouchedX;
        lastTouchedY = currTouchedY;
    }
    else
    {
        if (currTouchedX != lastTouchedX)
        {
            removeSoldier(touchedSoldier);
            if (!appendSoldier(currTouchedX, touchedSoldier))
            {
                appendSoldier(lastTouchedX, touchedSoldier);
            }
        }

        touchedSoldier->stand();
        touchedSoldier = NULL;
    }

    rearrange();
}

float AttackerField::getPositionYByIndex(int y)
{
    return origin.y + y * BASE_TILE_HEIGHT;
}


//DefenderField///////////////////////////////////////////////////////////////////////////////////////////////
DefenderField::DefenderField()
{
    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height + BASE_TILE_HEIGHT) / 2);

    GLOBAL->Defenders = allSoldiers;
}

DefenderField::~DefenderField()
{
}

DefenderField* DefenderField::create()
{
    DefenderField* layer = NULL;

    do 
    {
        layer = new DefenderField();
        CC_BREAK_IF(!layer);

        layer->createReinforcements();
        layer->sendSoldiersToField();

        layer->autorelease();

        return layer;

    } while(0);

    return NULL;
}

void DefenderField::draw()
{
    BattleField::draw();
}

float DefenderField::getPositionYByIndex(int y)
{
    return origin.y + (FIELD_HEIGHT - y - 1) * BASE_TILE_HEIGHT;
}
