#pragma once
#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "cocos2d.h"
#include "PlayerState.h"
#include <unordered_map>

class AnimationManager
{
private:
    std::unordered_map<PlayerState, cocos2d::Animation*> m_playerAnimations;
    std::unordered_map<std::string, cocos2d::Animation*> m_enemyAnimations;

public:
    static AnimationManager* getInstance();

    void loadPlayerAnimations();
    void loadEnemyAnimations(const std::string& enemyType);

    cocos2d::Animation* getPlayerAnimation(PlayerState state);
    cocos2d::Animation* getEnemyAnimation(const std::string& enemyType, const std::string& action);

    cocos2d::Animate* createPlayerAnimate(PlayerState state);
    cocos2d::Animate* createEnemyAnimate(const std::string& enemyType, const std::string& action);

private:
    AnimationManager() = default;
    ~AnimationManager() = default;

    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);
};

#endif // __ANIMATION_MANAGER_H__