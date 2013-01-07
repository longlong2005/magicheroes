#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

typedef enum _BattleState 
{
    StateNone = 0,
    StateStart,
    StateAtkAction,
    StateAtkTurn,
    StateAtkRearrange,
    StateDfnAction,
    StateDfnTurn,
    StateDfnRearrange,
    StateEnd
} BattleState; 

class BattleScene : public CCScene
{
public:
	static BattleScene* create();

    virtual void update(float dt);

    inline bool isAtkTurn() {return state==StateAtkTurn;}

    void onAtkSoldierMove(CCObject* obj);
    void onAtkSoldierCompleteRearrange(CCObject* obj);

    //SCENE_NODE_FUNC(BattleScene);
private:
    bool init();

    void battleStartCallBack();

    BattleState state;

    CCLabelTTF* notice;

protected:
    BattleScene();  
    ~BattleScene();
};

#endif  // __BATTLE_SCENE_H__