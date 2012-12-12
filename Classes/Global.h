#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "Singleton.h"

USING_NS_CC;

class Soldier;
class BackGroundLayer;

class Global : public Singleton<Global>
{
public:
    CCSize WinSize;

    CCArray* Attackers;
    CCArray* Defenders;

    BackGroundLayer* BattleBG;

    DECLARE_SINGLETON(Global); //private
};

#define GLOBAL Global::instance()

#define WIN_SIZE GLOBAL->WinSize
#define ATTACKERS GLOBAL->Attackers
#define DEFENDERS GLOBAL->Defenders

#endif  // __GLOBAL_H__

