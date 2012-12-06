#include "BattleScene.h"
#include "BoardLayer.h"

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

	do 
	{
		scene = new BattleScene();
		CC_BREAK_IF(!scene);

        layer = BoardLayer::create();
        CC_BREAK_IF(!layer);

        scene->addChild(layer, 1);
        scene->autorelease();

        return scene;

	} while(0);

	return NULL;
}
