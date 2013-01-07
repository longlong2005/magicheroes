#ifndef __GAME_BASIC_H__
#define __GAME_BASIC_H__

#include "cocos2d.h"
#include "Global.h"
#include "AnimationManager.h"

USING_NS_CC;

#define FIELD_COLUMN_MAX 8
#define FIELD_ROW_MAX 6
#define FIELD_GRID_WIDTH 74
#define FIELD_GRID_HEIGHT 56

#define GridIndexOrigin GridIndexMake(0, 0)

typedef struct _GridIndex 
{
    int column;
    int row;
} GridIndex;

//! helper function to create a GridIndex
static inline GridIndex
GridIndexMake(const int column, const int row)
{
    GridIndex v = {column, row};
    return v;
};

#endif  // __GAME_BASIC_H__