#include "Global.h"

DELCARE_SINGLETON_MEMBER(Global);

Global::Global()
{
    Attackers = CCArray::create();
    Defenders = CCArray::create();

    BattleBG = NULL;

    Attackers->retain();
    Defenders->retain();

    WinSize = CCDirector::sharedDirector()->getWinSize();
}


Global::~Global()
{
    Attackers->release();
    Defenders->release();
}
