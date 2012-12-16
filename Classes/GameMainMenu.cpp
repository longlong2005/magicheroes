#include "GameMainMenu.h"
#include "BattleScene.h"
#include "Spriter\SpriterNode.h"

GameMainMenu::GameMainMenu()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCMenuItemFont::setFontSize(50);
    CCMenuItemFont::setFontName("Marker Felt");
#else
    CCMenuItemFont::setFontSize(50);
    CCMenuItemFont::setFontName("fonts/Abberancy.ttf"); //can not change fonts?
#endif
    
    CCMenuItemFont* item1 = CCMenuItemFont::create("Test Battle", this, menu_selector(GameMainMenu::testBattle));
    CCMenuItemFont* item2 = CCMenuItemFont::create("Test Animation", this, menu_selector(GameMainMenu::testAnimation));
    CCMenuItemFont* item3 = CCMenuItemFont::create("Test Http", this, menu_selector(GameMainMenu::testHttp));
    CCMenuItemFont* item4 = CCMenuItemFont::create("Quit", this, menu_selector(GameMainMenu::onQuit));
    
    CCMenu* menu = CCMenu::create(item1, item2, item3, item4, NULL);
    menu->alignItemsVertically();
    
    addChild( menu );
}


GameMainMenu::~GameMainMenu()
{
}


GameMainMenu* GameMainMenu::create()
{
	GameMainMenu* scene = NULL;

	do 
	{
		scene = new GameMainMenu();
		CC_BREAK_IF(!scene);

        scene->autorelease();

        return scene;

	} while(0);

	return NULL;
}

void GameMainMenu::testBattle(CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, BattleScene::create()));
}

void GameMainMenu::testAnimation(CCObject* sender)
{
    CCScene* scene = CCScene::create();
    

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "monster.plist" );
    SpriterNode *n = SpriterNode::create( "Example.SCML", "monster.png" );
    SpriterNode *m = SpriterNode::create( "Example.SCML", "monster.png" );

    n->setPosition( ccp(160, 100) );
    n->setScale(0.6f);

    m->setPosition( ccp(500, 100) );
    m->setScale(0.8f);

    m->runAnimation( "Posture" );
    n->runAnimation( "Idle" );

    scene->addChild( n );
    scene->addChild( m);

    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, scene));
}

void GameMainMenu::testHttp(CCObject* sender)
{
}

void GameMainMenu::onQuit(CCObject* sender)
{
}