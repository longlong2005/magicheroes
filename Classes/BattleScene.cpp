#include "BattleScene.h"
#include "BattleField.h"
#include "BackGroundLayer.h"
#include "BattleController.h"
#include "Global.h"

BattleScene::BattleScene()
    :state(StateStart)
{
    init();
}

BattleScene::~BattleScene()
{
    NOTIFY->removeObserver(this, MSG_ATK_SOLDIER_MOVE);
    NOTIFY->removeObserver(this, MSG_ATK_SOLDIER_COMPLETE_REARRANGE);
}

bool BattleScene::init()
{
    CCScene::init();

    notice = CCLabelTTF::create("Battle Start", "Marker Felt", 50);  
    notice->setPosition(CCPointMake(WIN_SIZE.width / 2, WIN_SIZE.height / 2));

    CCArray* actions = CCArray::create();
    actions->addObject(CCFadeIn::create(2));
    actions->addObject(CCFadeOut::create(2));
    actions->addObject(CCCallFunc::create(this, callfunc_selector(BattleScene::battleStartCallBack)));
    notice->runAction(CCSequence::create(actions));

    addChild(notice, 999);

    actions->release();


    //register msg
    NOTIFY->addObserver(this, callfuncO_selector(BattleScene::onAtkSoldierMove), MSG_ATK_SOLDIER_MOVE, NULL);
    NOTIFY->addObserver(this, callfuncO_selector(BattleScene::onAtkSoldierCompleteRearrange), MSG_ATK_SOLDIER_COMPLETE_REARRANGE, NULL);

	return true;
}

BattleScene* BattleScene::create()
{
	BattleScene* scene = NULL;
    BattleField* layer = NULL;
    BackGroundLayer* bg = NULL;
    BattleController* controller = NULL;
    CCLabelTTF *label = NULL;

	do 
	{
		scene = new BattleScene();
		CC_BREAK_IF(!scene);

        layer = AttackerField::create();
        CC_BREAK_IF(!layer);
        scene->addChild(layer, 2);

        layer = DefenderField::create();
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

void BattleScene::update(float dt)
{
}

void BattleScene::battleStartCallBack()
{
    CCArray* actions = CCArray::create();
    actions->addObject(CCFadeIn::create(1));
    actions->addObject(CCFadeOut::create(2));
    notice->setString("State-Attacker-Turn");
    notice->runAction(CCSequence::create(actions));

    actions->release();

    state = StateAtkTurn;
}

void BattleScene::onAtkSoldierMove(CCObject* obj)
{
    if (state == StateAtkRearrange)
        return;

    CCArray* actions = CCArray::create();
    actions->addObject(CCFadeIn::create(1));
    actions->addObject(CCFadeOut::create(1));
    notice->setString("State-Attacker-Rearrange");
    notice->runAction(CCSequence::create(actions));

    actions->release();

    state = StateAtkRearrange;
}

void BattleScene::onAtkSoldierCompleteRearrange(CCObject* obj)
{
    if (state == StateAtkTurn)
        return;

    CCArray* actions = CCArray::create();
    actions->addObject(CCFadeIn::create(1));
    actions->addObject(CCFadeOut::create(1));
    notice->setString("State-Attacker-Turn");
    notice->runAction(CCSequence::create(actions));

    actions->release();

    state = StateAtkTurn;
}