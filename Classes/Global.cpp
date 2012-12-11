#include "Global.h"

DELCARE_SINGLETON_MEMBER(Global);

Global::Global()
{
    this->Attackers = CCArray::create();
    this->Defenders = CCArray::create();

    BattleBG = NULL;
}


Global::~Global()
{
}
