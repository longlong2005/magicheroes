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
    CCArray* Attackers;
    CCArray* Defenders;

    BackGroundLayer* BattleBG;

    DECLARE_SINGLETON(Global); //private
};

#define GLOBAL Global::instance()

#define ATTACKERS GLOBAL->Attackers
#define DEFENDERS GLOBAL->Defenders

#define WIN_SIZE CCDirector::sharedDirector()->getWinSize()
#define NOTIFY CCNotificationCenter::sharedNotificationCenter()




#define MSG_ATK_SOLDIER_MOVE "ATK_SOLDIER_MOVE"
#define MSG_ATK_SOLDIER_COMPLETE_MOVE "ATK_SOLDIER_COMPLETE_MOVE"
#define MSG_ATK_SOLDIER_COMPLETE_REARRANGE "ATK_SOLDIER_COMPLETE_REARRANGE"

#endif  // __GLOBAL_H__

