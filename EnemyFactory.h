#pragma once
#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "Enemy.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "EliteEnemy.h"
#include <memory>

class EnemyFactory
{
public:
    static std::shared_ptr<Enemy> createEnemy(EnemyType type, const cocos2d::Vec2& position);
    static std::shared_ptr<Enemy> createEnemy(const std::string& enemyType, const cocos2d::Vec2& position);

    static std::shared_ptr<MeleeEnemy> createMeleeEnemy(const cocos2d::Vec2& position);
    static std::shared_ptr<RangedEnemy> createRangedEnemy(const cocos2d::Vec2& position);
    static std::shared_ptr<EliteEnemy> createEliteEnemy(const cocos2d::Vec2& position);
};

#endif // __ENEMY_FACTORY_H__