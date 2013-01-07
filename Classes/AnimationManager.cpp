#include "AnimationManager.h"

DELCARE_SINGLETON_MEMBER(AnimationManager);

AnimationManager::AnimationManager()
{
}


AnimationManager::~AnimationManager()
{
}

CCAnimation* AnimationManager::getAnimation()
{
    CCArray* animFrames = CCArray::create();
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("s2.png");
    animFrames->addObject(CCSpriteFrame::createWithTexture(texture, CCRectMake(120 * 0, 0, 120, 120)));
    animFrames->addObject(CCSpriteFrame::createWithTexture(texture, CCRectMake(120 * 1, 0, 120, 120)));
    animFrames->addObject(CCSpriteFrame::createWithTexture(texture, CCRectMake(120 * 2, 0, 120, 120)));
    animFrames->addObject(CCSpriteFrame::createWithTexture(texture, CCRectMake(120 * 3, 0, 120, 120)));

    return CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
}