#pragma once
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "cocos2d.h"
#include <unordered_map>
#include <string>

class AnimationManager {
public:
    static AnimationManager* getInstance();
    static void destroyInstance();

    // 预锟斤拷锟斤拷锟斤拷锟叫碉拷锟剿讹拷锟斤拷
    void preloadEnemyAnimations();

    // 锟斤拷取锟斤拷锟斤拷
    cocos2d::Animation* getAnimation(const std::string& enemyType, const std::string& state);

    // 锟斤拷取锟斤拷锟斤拷帧锟接筹拷
    float getAnimationDelay(const std::string& enemyType, const std::string& state);

    // 锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    struct EnemyConfig {
        float maxHealth;
        float attackDamage;
        float moveSpeed;
        float attackRange;
        float detectionRange;
        float attackCooldown;
        std::string name;
        bool isBoss;
        int phaseCount; // BOSS锟阶讹拷锟斤拷
    };

    const EnemyConfig& getEnemyConfig(const std::string& enemyType);

private:
    AnimationManager();
    ~AnimationManager();
    static AnimationManager* _instance;

    std::unordered_map<std::string, std::unordered_map<std::string, cocos2d::Animation*>> _animations;
    std::unordered_map<std::string, EnemyConfig> _enemyConfigs;

    void loadBoss1Animations();
    void loadBoss2Animations();
    void loadBoss3Animations();
    void loadCloseCombat1Animations();
    void loadCloseCombat2Animations();
    void loadEliteEnemyAnimations();
    void loadRemoteEnemyAnimations();

    void initEnemyConfigs();
};

#endif // ANIMATION_MANAGER_H