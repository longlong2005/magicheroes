#ifndef __GAME_MAIN_MENU_H__
#define __GAME_MAIN_MENU_H__

#include "cocos2d.h"
#include "AnimationManager.h"

USING_NS_CC;

class GameMainMenu : public CCScene
{
public:
    static GameMainMenu* create();

    //for test
    void testBattle(CCObject* sender);
    void testHttp(CCObject* sender);
    void onQuit(CCObject* sender);

protected:
    GameMainMenu();
    ~GameMainMenu();
};

#endif  // __GAME_MAIN_MENU_H__

