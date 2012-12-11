#include "BattleScene.h"
#include "BoardLayer.h"
#include "BackGroundLayer.h"
#include "BattleController.h"
#include "Global.h"

BattleScene::BattleScene()
{
}

BattleScene::~BattleScene()
{
}

bool BattleScene::init()
{
	return CCScene::init();
}

BattleScene* BattleScene::create()
{
	BattleScene* scene = NULL;
    BoardLayer* layer = NULL;
    BackGroundLayer* bg = NULL;
    BattleController* controller = NULL;

	do 
	{
		scene = new BattleScene();
		CC_BREAK_IF(!scene);

        layer = BoardLayer::create();
        CC_BREAK_IF(!layer);
        scene->addChild(layer, 1);

        bg = BackGroundLayer::create();
        CC_BREAK_IF(!bg);
        scene->addChild(bg, 0);
        //for test///////////
        GLOBAL->BattleBG = bg;

        /////////////////////
        controller = BattleController::create();
        CC_BREAK_IF(!controller);
        scene->addChild(controller, 99);

        scene->autorelease();

        return scene;

	} while(0);

	return NULL;
}
