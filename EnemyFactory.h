#pragma once
#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "Enemy.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "EliteEnemy.h"
#include "EnemyFactory.h"  
#include <stdexcept>
#include <memory>
#include <string>

// [新增] 难度/生成模式枚举
enum class DifficultyLevel {
    MILD,       // 温和: 两种近战
    MODERATE,   // 适中: 两种近战 + 一种远程
    SEVERE      // 严酷: 60%近战 + 30%远程 + 10%精英
};

class EnemyFactory {  
public:  
   // [配置] 设置当前生成的难度模式
   static void setDifficulty(DifficultyLevel level);
   static DifficultyLevel getDifficulty();

   // [核心接口] 根据当前难度模式，在指定位置随机生成一个敌人
   static std::shared_ptr<Enemy> createRandomEnemy(const cocos2d::Vec2& position);

   // [基础接口] 指定类型生成
   static std::shared_ptr<Enemy> createEnemy(EnemyType type, const cocos2d::Vec2& position);
   static std::shared_ptr<Enemy> createEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
   
   // [具体实现]
   static std::shared_ptr<MeleeEnemy> createMeleeEnemy(const cocos2d::Vec2& position);
   static std::shared_ptr<RangedEnemy> createRangedEnemy(const cocos2d::Vec2& position);
   static std::shared_ptr<EliteEnemy> createEliteEnemy(const cocos2d::Vec2& position);
    // 将字符串类型转换为EnemyType
    static EnemyType stringToEnemyType(const std::string& typeName) {
        if (typeName == "Melee") return EnemyType::MELEE;
        if (typeName == "Ranged") return EnemyType::RANGED;
        if (typeName == "Elite") return EnemyType::ELITE;
        throw std::invalid_argument("Unknown EnemyType: " + typeName);
    }
    
private:
    static DifficultyLevel s_currentDifficulty;
};

    
#endif // __ENEMY_FACTORY_H__