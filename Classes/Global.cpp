#include "Global.h"

DELCARE_SINGLETON_MEMBER(Global);

Global::Global()
    :Attackers(NULL)
    ,Defenders(NULL)
{
    //for test
    BattleBG = NULL;
}


Global::~Global()
{
    Attackers = NULL;
    Defenders = NULL;
}
