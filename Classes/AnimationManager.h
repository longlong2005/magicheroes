#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "cocos2d.h"
#include "Singleton.h"

USING_NS_CC;

class AnimationManager : public Singleton<AnimationManager>
{
public:
    CCAnimation* getAnimation();

    DECLARE_SINGLETON(AnimationManager); //private
};

#define ANIMATION_MGR AnimationManager::instance()

#endif  // __ANIMATION_MANAGER_H__

