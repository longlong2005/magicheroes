#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

class BattleScene : public CCScene
{
public:
	static BattleScene* create();

    //SCENE_NODE_FUNC(BattleScene);
private:
    bool init();

protected:
    BattleScene();  
    ~BattleScene();
};

#endif  // __BATTLE_SCENE_H__