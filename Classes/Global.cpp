#include "Global.h"

DELCARE_SINGLETON_MEMBER(Global);

Global::Global()
    :Attackers(NULL)
    ,Defenders(NULL)
{
    //for test
    BattleBG = NULL;

    WinSize = CCDirector::sharedDirector()->getWinSize();
}


Global::~Global()
{
}
