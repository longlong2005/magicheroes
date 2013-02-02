#include "GameMainMenu.h"
#include "BattleScene.h"
#include "Spriter/SpriterNode.h"
#include "Net/HttpUtils.h"
#include "Data/StrParser.h"

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
    CCMenuItemFont* item5 = CCMenuItemFont::create("Test StrParser", this, menu_selector(GameMainMenu::testStr));
    CCMenuItemFont* item4 = CCMenuItemFont::create("Quit", this, menu_selector(GameMainMenu::onQuit));
    
    CCMenu* menu = CCMenu::create(item1, item2, item3, item5, item4, NULL);
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
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    
    /* Read a ccbi file. */
    CCNode * node = ccbReader->readNodeGraphFromFile("aniTest.ccbi",this);
    
    node->setPosition(100, 200);
    
    if(node != NULL) {
        this->addChild(node);
        //获取所有的动作序列
        CCArray* allSeq = ccbReader->getAnimationManager()->getSequences();
        
        for (int i=0; i<allSeq->count(); i++) {
            //获取到每一个Seq
            CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
            //获取每个序列动作周期、名字以及id
            everySeq->getDuration();
            everySeq->getName();
            everySeq->getSequenceId();
        }
        
        ccbReader->getAnimationManager()->runAnimations("Default Timeline");
    }
    
    ccbReader->release();
    
////    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "monster.plist" );
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "s1.plist" );
//    SpriterNode *n1 = SpriterNode::create( "a1.scml", "s1.png" );
//    //SpriterNode *n2 = SpriterNode::create( "monster.scml", "monster.png" );
//
//    n1->setPosition( ccp(260, 200) );
//    n1->setScale(3);
//    n1->runAnimation( "daiji" );
    
//    //    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "monster.plist" );
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "monster.plist" );
//    SpriterNode *n1 = SpriterNode::create( "Example.SCML", "monster.png" );
//    //SpriterNode *n2 = SpriterNode::create( "monster.scml", "monster.png" );
//    
//    n1->setPosition( ccp(260, 200) );
//    n1->setScale(3);
//    n1->runAnimation( "Idle" );

    //n2->setPosition( ccp(500, 100) );
    //n2->setScale(0.8f);
    //n2->runAnimation( "Idle" );
    
//    scene->addChild( n1 );
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

void GameMainMenu::testStr(CCObject* sender)
{
    StrParser parser;
    CCDictionary dic;
    parser.parsePackageFile("s_en.txt", &dic);
    CCArray *allKeys = dic.allKeys();
    CCObject *key = NULL;
    CCObject *value = NULL;
    const char *ckey;
    const char *cvalue;
    CCARRAY_FOREACH(allKeys, key)
    {
        ckey = ((CCString*)key)->getCString();
        std::string strKey(ckey);
        value = dic.objectForKey(strKey);
        cvalue = ((CCString*)value)->getCString();
        printf("%s:%s\n",ckey, cvalue);
    }
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