#include "BattleField.h"
#include "Soldier.h"
#include "Spriter/SpriterNode.h"

//BattleField///////////////////////////////////////////////////////////////////////////////////////////////
BattleField::BattleField()
    :touchedSoldier(NULL)
    ,lastIndex(GridIndexOrigin)
    ,allSoldiers(CCArray::create())
    ,reinforcements(CCArray::create())
    ,movedSoldiers(CCArray::create())
    ,needToRearrange(CCArray::create())
{
    memset(field, 0, FIELD_COLUMN_MAX * FIELD_ROW_MAX * sizeof(Soldier*));
    size = CCSize(FIELD_GRID_WIDTH * FIELD_COLUMN_MAX, FIELD_GRID_HEIGHT * FIELD_ROW_MAX);

    allSoldiers->retain();
    reinforcements->retain();
    movedSoldiers->retain();
    needToRearrange->retain();
}

BattleField::~BattleField()
{
    allSoldiers->release();
    reinforcements->release();
    movedSoldiers->release();
    needToRearrange->release();
}

void BattleField::draw()
{
    CCLayer::draw();

    CHECK_GL_ERROR_DEBUG();

    ccDrawColor4B(255,0,255,64);
    for (int j = 0; j <= FIELD_ROW_MAX; j++) 
    {
        ccDrawLine(ccp(origin.x, origin.y + j * FIELD_GRID_HEIGHT), 
            ccp(origin.x + FIELD_COLUMN_MAX * FIELD_GRID_WIDTH, origin.y + j * FIELD_GRID_HEIGHT));
    }

    ccDrawColor4B(255,0,255,64);
    for (int i = 0; i <= FIELD_COLUMN_MAX; i ++) 
    {
        ccDrawLine(ccp(origin.x + i * FIELD_GRID_WIDTH, origin.y), 
            ccp(origin.x + i * FIELD_GRID_WIDTH, origin.y + FIELD_ROW_MAX * FIELD_GRID_HEIGHT));
    }

    //field position origin
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

bool BattleField::changeIntoAttackFormation(int column, int row)
{  
    int startRow = row - 2;
    int sameCount = 0;

    Soldier* lastSoldier = NULL;
    CCArray* attackFormation = CCArray::create();
    
    for (int i = 0; i < 5; i++)
    {
        int nextRow = startRow + i;
        if (nextRow >= FIELD_ROW_MAX || nextRow < 0) 
            continue;

        Soldier* currSoldier = getSoldierByIndex(GridIndexMake(column, nextRow));
        if (currSoldier == NULL || !currSoldier->isAlone()) 
        {
            sameCount = 0;
            attackFormation->removeAllObjects();

            continue;
        }

        if (lastSoldier)
        {
            if (lastSoldier->getColor() != currSoldier->getColor())
            {
                sameCount = 0;
                attackFormation->removeAllObjects();
            }
        }

        lastSoldier = currSoldier;
        attackFormation->addObject(currSoldier);

        if (++sameCount >= 3)
        {
            //move to top after walls
            removeSoldiers(attackFormation);
            for (int j = 0; j < 3; j++)
            {
                currSoldier = (Soldier*)attackFormation->objectAtIndex(j);
                currSoldier->charge();

                NOTIFY->postNotification(MSG_ATK_SOLDIER_MOVE, currSoldier);

                insertSoldier(column, j, currSoldier);
            }
            
            return true;
        }
    }

    return false;
}

bool BattleField::changeIntoDefendWall(int column, int row)
{
    int startColumn = column - 2;
    int sameCount = 0;

    Soldier* lastSoldier = NULL;
    CCArray* walls = CCArray::create();

    for (int i = 0; i < 5; i++)
    {
        int nextColumn = startColumn + i;
        if (nextColumn >= FIELD_COLUMN_MAX || nextColumn < 0) 
            continue;

        Soldier* currSoldier = getSoldierByIndex(GridIndexMake(nextColumn, row));
        if (currSoldier == NULL || !currSoldier->isAlone())
        {
            sameCount = 0;
            walls->removeAllObjects();

            continue;
        }

        if (lastSoldier)
        {
            if (lastSoldier->getColor() != currSoldier->getColor())
            {
                sameCount = 0;
                walls->removeAllObjects();
            }
        }

        lastSoldier = currSoldier;
        walls->addObject(currSoldier);
        
        if (++sameCount >= 3)
        {
            //move to top after walls
            removeSoldiers(walls);
            for (int j = 0; j < 3; j++)
            {
                currSoldier = (Soldier*)walls->objectAtIndex(j);
                currSoldier->wall();

                NOTIFY->postNotification(MSG_ATK_SOLDIER_MOVE, currSoldier);

                insertSoldier(currSoldier->getGridIndex().column, 0, currSoldier);
            }

            return true;
        }
    }

    return false;
}

Soldier** BattleField::soldiersInColumn(int column)
{
    CCAssert(column >= 0 && column < FIELD_COLUMN_MAX, sprintf("Invalid column index:%d", column));

    return field[column];
}

Soldier** BattleField::soldiersInRow(int row)
{
    CCAssert(row >= 0 && row < FIELD_ROW_MAX, sprintf("Invalid row index:%d", row));

    memset(rowSoldiers, 0, FIELD_COLUMN_MAX * sizeof(Soldier*));
    for (int i = 0; i < FIELD_COLUMN_MAX; i++)
    {
        rowSoldiers[i] = field[i][row];
    }

    return rowSoldiers;
}

Soldier* BattleField::lastSoldierInColumn(int column)
{
    Soldier** soldiers = soldiersInColumn(column);
    for (int i = FIELD_ROW_MAX - 1; i >= 0; i--)
    {
        if (soldiers[i]) 
            return soldiers[i];
    }

    return NULL;
}

Soldier* BattleField::firstSoldierInColumn(int column)
{
    return soldiersInColumn(column)[0];
}

Soldier* BattleField::getSoldierByIndex(const GridIndex& index)
{
    CCAssert(index.row >= 0 && index.row < FIELD_ROW_MAX, sprintf("Invalid row index:%d", index.row));

    return soldiersInColumn(index.column)[index.row];
}

Soldier* BattleField::getSoldierByPosition(CCPoint position)
{
    int column = position.x / FIELD_GRID_WIDTH;
    int row = position.y / FIELD_GRID_HEIGHT;

    return getSoldierByIndex(GridIndexMake(column, row));
}
    
bool BattleField::appendSoldier(int column, Soldier* soldier)
{
    Soldier** soldiers = soldiersInColumn(column);

    for (int i = 0; i < FIELD_ROW_MAX; i++)
    {
        if (soldiers[i] == NULL)
        {
            soldiers[i] = soldier;
            soldier->setGridIndex(GridIndexMake(column, i));
            //soldier->setPosition(getPositionByIndex(GridIndexMake(column, i)));
            return true;
        }
    }

    return false;
}

void BattleField::insertSoldier(int column, int row, Soldier* soldier)
{
    CCAssert(row >= 0 && row < FIELD_ROW_MAX, sprintf("Invalid row index:%d", row));

    Soldier** soldiers = soldiersInColumn(column);
    int moveCount = FIELD_ROW_MAX - row -1;

    if (moveCount <= 0)
        return;
    
    int lastRow = FIELD_ROW_MAX - 1;
    CCPoint position;
    for (int i = 0; i < moveCount; i++)
    {
        soldiers[FIELD_ROW_MAX - i - 1] = soldiers[FIELD_ROW_MAX - i - 2];
        if (soldiers[FIELD_ROW_MAX - i - 1])
        {
            position = getPositionByIndex(GridIndexMake(column, FIELD_ROW_MAX - i - 1));
            soldiers[FIELD_ROW_MAX - i - 1]->setGridIndex(GridIndexMake(column, FIELD_ROW_MAX - i - 1));
            soldiers[FIELD_ROW_MAX - i - 1]->moveTo(position);

            if (movedSoldiers->containsObject(soldiers[FIELD_ROW_MAX - i - 1]))
            {
                int index = movedSoldiers->indexOfObject(soldiers[FIELD_ROW_MAX - i - 1]);
                movedSoldiers->replaceObjectAtIndex(index, soldiers[FIELD_ROW_MAX - i - 1]);
            }
            else
            {
                movedSoldiers->addObject(soldiers[FIELD_ROW_MAX - i - 1]);
            } 
            
            if (needToRearrange->containsObject(soldiers[FIELD_ROW_MAX - i - 1]))
            {
                int index = needToRearrange->indexOfObject(soldiers[FIELD_ROW_MAX - i - 1]);
                needToRearrange->replaceObjectAtIndex(index, soldiers[FIELD_ROW_MAX - i - 1]);
            }
            else
            {
                needToRearrange->addObject(soldiers[FIELD_ROW_MAX - i - 1]);
            } 
        }
    }

    position = getPositionByIndex(GridIndexMake(column, row));
    soldier->setGridIndex(GridIndexMake(column, row));
    soldier->moveTo(position);
    movedSoldiers->addObject(soldier);
    soldiers[row] = soldier;
}

void BattleField::removeSoldier(Soldier* soldier)
{
    for (int i = 0; i < FIELD_COLUMN_MAX; i++)
    {
        for (int j = 0; j < FIELD_ROW_MAX; j++)
        {
            if (field[i][j] == soldier)
            {
                field[i][j] = NULL;
                reformSoldierInColumn(i);
            }
        }
    }
}

void BattleField::removeSoldiers(CCArray* soldiers)
{
    CCObject* soldier = NULL;
    CCARRAY_FOREACH(soldiers, soldier)
    {
        CC_BREAK_IF(!soldier);

        removeSoldier((Soldier*)soldier);
    }
}

void BattleField::reformSoldierInColumn(int column)
{
    Soldier** soldiers = soldiersInColumn(column);
    for (int i = 0; i < FIELD_ROW_MAX - 1; i++)
    {
        if (soldiers[i] == NULL)
        {
            soldiers[i] = soldiers[i + 1];
            soldiers[i + 1] = NULL;

            if (soldiers[i])
            {
                soldiers[i]->setGridIndex(GridIndexMake(column, i));
                needToRearrange->addObject(soldiers[i]);
            }
        }
    }
}

void BattleField::removeSoldierByIndex(const GridIndex& index)
{
    Soldier* soldier = getSoldierByIndex(index);
    if (soldier)
    {
        removeSoldier(soldier);
    }
}

float BattleField::getPositionXByColumn(int column)
{
    return origin.x + column * FIELD_GRID_WIDTH;
}

GridIndex BattleField::getIndexByPosition(CCPoint point)
{
    return GridIndexMake(getColumnByPosition(point.x), getRowByPosition(point.y));
}

CCPoint BattleField::getPositionByIndex(const GridIndex& index)
{
    return CCPointMake(getPositionXByColumn(index.column), getPositionYByRow(index.row));
}

int BattleField::getColumnByPosition(float x)
{
    return (x - origin.x) / FIELD_GRID_WIDTH;
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
    int column = 0;
    int row = 0;

    CCObject* soldier = NULL;
    CCARRAY_FOREACH(reinforcements, soldier)
    {
        if (soldier == NULL) 
            break;

        if (getEnableGridIndex(column, row))
        {
            field[column][row] = (Soldier*)soldier;
            field[column][row]->setPosition(getPositionByIndex(GridIndexMake(column, row)));
            field[column][row]->setGridIndex(GridIndexMake(column, row));
            addChild(field[column][row], -getPositionYByRow(row));
            reinforcements->fastRemoveObject(soldier);
        }

        if (++column >= FIELD_COLUMN_MAX) 
            column = 0;
    }
}

bool BattleField::getEnableGridIndex(int &column, int &row)
{
    for (int i = 0; i < FIELD_ROW_MAX; i++)
    {
        if (field[column][i] != NULL) 
            continue;

        row = i;
        //soldier size?
        return true;
    }

    return false;
}

//AttackerField///////////////////////////////////////////////////////////////////////////////////////////////
AttackerField::AttackerField()
{
    GLOBAL->Attackers = allSoldiers;

    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height - FIELD_GRID_HEIGHT) / 2 - size.height);

    init();
}

AttackerField::~AttackerField()
{
    NOTIFY->removeObserver(this, MSG_ATK_SOLDIER_COMPLETE_MOVE);
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

bool AttackerField::init()
{
    //register msg
    NOTIFY->addObserver(this, callfuncO_selector(AttackerField::onAtkSoldierCompleteMove) , MSG_ATK_SOLDIER_COMPLETE_MOVE, NULL);

    return true;
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

    GridIndex currIndex = getIndexByPosition(touchPoint);
    Soldier* soldier = NULL;

    if (!touchedSoldier)
    {
        Soldier* soldier = lastSoldierInColumn(currIndex.column);
        if (!soldier || !soldier->isAlone())
            return false;

        soldier->selected();
        reorderChild(soldier, FIELD_ROW_MAX * 2);
        touchedSoldier = soldier;
        lastIndex = currIndex;
    }

    return true;
}

void AttackerField::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();

    if (!boundingBox().containsPoint(touchPoint))
        return;

    //if (touchedSoldier && touchedSoldier->isSelected())
    //{
    //    GridIndex currIndex = getIndexByPosition(touchPoint);

    //    if (abs(lastIndex.column - currIndex.column) >= 1)
    //    {
    //        //set game state 
    //        if (movedSoldiers->count() > 0)
    //            movedSoldiers->removeAllObjects();

    //        if (needToRearrange->count() > 0)
    //            needToRearrange->removeAllObjects();

    //        removeSoldier(touchedSoldier);
    //        this->removeChild(touchedSoldier, false);

    //        touchedSoldier = NULL;
    //    }
    //    else
    //    {
    //        Soldier* soldier = getSoldierByIndex(currIndex);
    //        if (!soldier || !soldier->isAlone())
    //            return;

    //        soldier->selected();
    //        reorderChild(soldier, FIELD_ROW_MAX * 2);
    //        touchedSoldier->stand();
    //        touchedSoldier = soldier;
    //        lastIndex = currIndex;
    //    }
    //}
}

void AttackerField::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();
    if (!boundingBox().containsPoint(touchPoint))
        return;

    GridIndex currIndex = getIndexByPosition(touchPoint);
    Soldier* soldier = NULL;

    if (touchedSoldier)
    {
        if (touchedSoldier->isSelected())
        {
            touchedSoldier->moveTo(CCPointMake(touchedSoldier->getPosition().x, -100), 0.2f);
            touchedSoldier->stand();

            return;
        }

        removeSoldier(touchedSoldier);

        if (appendSoldier(currIndex.column, touchedSoldier))
        {
            touchedSoldier->setPositionX(getPositionByIndex(currIndex).x);

            //set game state 
            if (movedSoldiers->count() > 0)
                movedSoldiers->removeAllObjects();

            if (needToRearrange->count() > 0)
                needToRearrange->removeAllObjects();

            movedSoldiers->addObject(touchedSoldier);
            needToRearrange->addObject(touchedSoldier);
        }
        else
        {
            appendSoldier(lastIndex.column, touchedSoldier);
        }

        CCPoint position = getPositionByIndex(touchedSoldier->getGridIndex());
        touchedSoldier->moveTo(position, 0.2f);

        touchedSoldier = NULL;
    }
}

float AttackerField::getPositionYByRow(int row)
{
    return origin.y + (FIELD_ROW_MAX - row - 1) * FIELD_GRID_HEIGHT;
}

int AttackerField::getRowByPosition(float y)
{
    return FIELD_ROW_MAX - (y - origin.y) / FIELD_GRID_HEIGHT - 1;
}

void AttackerField::onAtkSoldierCompleteMove(CCObject* obj)
{
    movedSoldiers->removeObject(obj);
    if (movedSoldiers->count() == 0) //all moved soldiers have been in right position
    {
        if (needToRearrange->count() > 0)
        {
            CCLog("------------------------------------");

            do
            {
                CCObject* soldier = needToRearrange->lastObject();
                CC_BREAK_IF(!soldier);

                GridIndex index = ((Soldier*)soldier)->getGridIndex();
                CCLog("Grid index, soldier:%d, column:%d,row:%d", soldier, index.column, index.row);
                needToRearrange->removeObject(soldier);
                //check attack formation or walls
                if (changeIntoAttackFormation(index.column, index.row) || changeIntoDefendWall(index.column, index.row))
                {
                    return;
                }
            } while (true);
        }

        NOTIFY->postNotification(MSG_ATK_SOLDIER_COMPLETE_REARRANGE, this);
    }
}

//DefenderField///////////////////////////////////////////////////////////////////////////////////////////////
DefenderField::DefenderField()
{
    origin =  CCPoint((WIN_SIZE.width - size.width) / 2, (WIN_SIZE.height + FIELD_GRID_HEIGHT) / 2);

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

float DefenderField::getPositionYByRow(int row)
{
    return origin.y + row * FIELD_GRID_HEIGHT;
}

int DefenderField::getRowByPosition(float y)
{
    return (origin.y - y) / FIELD_GRID_HEIGHT;
}

