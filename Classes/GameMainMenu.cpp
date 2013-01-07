#include "GameMainMenu.h"
#include "BattleScene.h"
#include "Spriter/SpriterNode.h"
#include "Net/HttpUtils.h"

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
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "s1/s1.plist" );
    SpriterNode *n1 = SpriterNode::create( "s1/a1.scml", "s1/s1.png" );
    //SpriterNode *n2 = SpriterNode::create( "monster.scml", "monster.png" );

    n1->setPosition( ccp(160, 100) );
    n1->setScale(3);
    n1->runAnimation( "daiji" );

    //n2->setPosition( ccp(500, 100) );
    //n2->setScale(0.8f);
    //n2->runAnimation( "Idle" );
    
    scene->addChild( n1 );
    //scene->addChild( n2);

            //n1->setIsFlipX(false);
        //n2->setIsFlipX(false);

    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, scene));
}

void GameMainMenu::testHttp(CCObject* sender)
{
    GetParam    gp;
    gp.url = "http://www.cmqa3.kabam.asia/ajax/upgrade.php?type=string&version=01001&worldId=0&mobileid=17c3a84ac65a61483ed70aee0b0007f7&platformid=201&become_user_id=&become_password=&debug=1&gameSlot=2&gameNumber=2&download=1";
    gp.ok = httpOk_selector(GameMainMenu::getOk);
    gp.error = httpError_selector(GameMainMenu::getError);
//    HTTP->get(&gp);
    HTTP->download(&gp, "abctest");
}

void    GameMainMenu::getOk(CCDictionary*ok)
{
    printf("getOk");
}
void    GameMainMenu::postOk(CCDictionary*ok)
{
    printf("postOk");
}

void    GameMainMenu::getError(const char*)
{
    printf("geterror");
}

void    GameMainMenu::postError(const char*)
{
    printf("posterror");
}

void GameMainMenu::onQuit(CCObject* sender)
{
}