#include "EnemyFactory.h"
#include "EnemyType.h"
#include "GameUtils.h"
#include <vector>
#include <stdexcept>

// 初始化静态成员
DifficultyLevel EnemyFactory::s_currentDifficulty = DifficultyLevel::MODERATE;

void EnemyFactory::setDifficulty(DifficultyLevel level) {
    s_currentDifficulty = level;
    GameUtils::log("Enemy Factory Difficulty Set To: " + std::to_string((int)level));
}

DifficultyLevel EnemyFactory::getDifficulty() {
    return s_currentDifficulty;
}

// [核心逻辑] 根据难度随机生成敌人
std::shared_ptr<Enemy> EnemyFactory::createRandomEnemy(const cocos2d::Vec2& position)
{
    std::string targetEnemyName = "NormalEnemyA"; // 默认保底

    try {
        int roll = GameUtils::randomInt(0, 100);

        switch (s_currentDifficulty)
        {
        case DifficultyLevel::MILD:
            // [Mild]: 两种近战敌人 (50% 普通, 50% 敏捷型)
            if (roll < 50) targetEnemyName = "NormalEnemyA";
            else targetEnemyName = "EliteMonsterB"; // 将敏捷怪作为第二种近战
            break;

        case DifficultyLevel::MODERATE:
            // [Moderate]: 两种近战 + 一种远程 (各 1/3 概率)
            if (roll < 33) targetEnemyName = "NormalEnemyA";
            else if (roll < 66) targetEnemyName = "EliteMonsterB";
            else targetEnemyName = "NormalEnemyB"; // 陆地远程
            break;

        case DifficultyLevel::SEVERE:
            // [Severe]: 60% 近战, 30% 远程, 10% 精英
            if (roll < 60) {
                // 近战池 (60%): 普通与敏捷各半
                if (GameUtils::randomBool()) targetEnemyName = "NormalEnemyA";
                else targetEnemyName = "EliteMonsterB";
            }
            else if (roll < 90) {
                // 远程池 (30%): 陆地与飞行各半
                if (GameUtils::randomBool()) targetEnemyName = "NormalEnemyB";
                else targetEnemyName = "NormalEnemyC"; // 飞行远程
            }
            else {
                // 精英池 (10%): 强壮精英怪
                targetEnemyName = "EliteMonsterA";
            }
            break;
        }
    }
    catch (const std::exception& e) {
        GameUtils::log("[Factory Error] Random generation failed: " + std::string(e.what()));
        // 出错时不崩溃，返回默认敌人
        targetEnemyName = "NormalEnemyA";
    }

    // 调用通用创建接口
    return createEnemy(targetEnemyName, position);
}

// [基础接口] 字符串映射实现
std::shared_ptr<Enemy> EnemyFactory::createEnemy(const std::string& enemyType, const cocos2d::Vec2& position)
{
    std::shared_ptr<Enemy> enemy = nullptr;

    try {
        // ----------------------------------------------------------------
        // [名称映射] 确保这里的名字与 MeleeEnemy::init 等文件中的判断一致
        // ----------------------------------------------------------------

        // --- Melee Types ---
        if (enemyType == "NormalEnemyA" || enemyType == "EliteMonsterB" || enemyType == "EliteMonsterA") {
            // 注意：EliteMonsterA/B 虽然叫 Elite，但在代码结构上继承自 MeleeEnemy (或在 MeleeEnemy.cpp 中处理)
            // 根据之前的 MeleeEnemy.cpp 实现，它处理了这三种名字
            enemy = createMeleeEnemy(position);
            if (enemy) enemy->setName(enemyType);
        }
        // --- Ranged Types ---
        else if (enemyType == "NormalEnemyB" || enemyType == "NormalEnemyC") {
            enemy = createRangedEnemy(position);
            if (enemy) enemy->setName(enemyType);
        }
        // --- BOSS / True Elite Types ---
        else if (enemyType == "CaiXunKun" || enemyType == "NaiLong" ||
            enemyType == "MaoDie" || enemyType == "KuMingYuanYang") {
            enemy = createEliteEnemy(position);
            if (enemy) enemy->setName(enemyType);
        }
        // --- 默认回退 ---
        else {
            if (enemyType == "Melee" || enemyType.empty()) {
                enemy = createMeleeEnemy(position);
                if (enemy) enemy->setName("NormalEnemyA");
            }
            else if (enemyType == "Ranged") {
                enemy = createRangedEnemy(position);
                if (enemy) enemy->setName("NormalEnemyB");
            }
            else if (enemyType == "Elite") {
                enemy = createEliteEnemy(position);
                // 默认精英用 EliteMonsterA
                if (enemy) enemy->setName("EliteMonsterA");
            }
            else {
                // 未知类型，记录日志并创建默认小兵
                GameUtils::log("[Factory Warning] Unknown type: " + enemyType + ", spawning NormalEnemyA.");
                enemy = createMeleeEnemy(position);
                if (enemy) enemy->setName("NormalEnemyA");
            }
        }

        // [位置与生成检查]
        if (enemy) {
            enemy->setPosition(position);

            // 确保敌人被正确初始化 (init 在 create 中已被调用)
            // 这里可以做额外的安全检查，例如检查贴图是否加载成功(通过宽高判断)
            if (enemy->getContentSize().width <= 0) {
                GameUtils::log("[Factory Error] Enemy created with invalid size (Texture load failed?)");
                // 强制设置一个可见大小，保证画面中有东西（色块）
                enemy->setTextureRect(cocos2d::Rect(0, 0, 50, 50));
                enemy->setColor(cocos2d::Color3B::MAGENTA);
            }
        }
    }
    catch (const std::exception& e) {
        GameUtils::log("[Factory Critical] CreateEnemy failed: " + std::string(e.what()));
        return nullptr;
    }

    return enemy;
}

// [重载接口] 枚举转字符串
std::shared_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, const cocos2d::Vec2& position)
{
    std::string typeName = "";
    switch (type) {
    case EnemyType::MELEE: typeName = "NormalEnemyA"; break;
    case EnemyType::RANGED: typeName = "NormalEnemyB"; break;
    case EnemyType::ELITE: typeName = "EliteMonsterA"; break;
    case EnemyType::BOSS: typeName = "CaiXunKun"; break; // 默认BOSS
    default: typeName = "NormalEnemyA"; break;
    }
    return createEnemy(typeName, position);
}

// [具体创建器] 使用 shared_ptr 管理 Cocos 对象生命周期
std::shared_ptr<MeleeEnemy> EnemyFactory::createMeleeEnemy(const cocos2d::Vec2& position)
{
    auto enemy = MeleeEnemy::create();
    if (enemy) {
        enemy->setPosition(position);
        enemy->retain(); // 增加引用计数，防止智能指针析构时对象被意外销毁
        // 自定义删除器：当 shared_ptr 计数为0时，调用 release
        return std::shared_ptr<MeleeEnemy>(enemy, [](MeleeEnemy* p) { p->release(); });
    }
    return nullptr;
}

std::shared_ptr<RangedEnemy> EnemyFactory::createRangedEnemy(const cocos2d::Vec2& position)
{
    auto enemy = RangedEnemy::create();
    if (enemy) {
        enemy->setPosition(position);
        enemy->retain();
        return std::shared_ptr<RangedEnemy>(enemy, [](RangedEnemy* p) { p->release(); });
    }
    return nullptr;
}

std::shared_ptr<EliteEnemy> EnemyFactory::createEliteEnemy(const cocos2d::Vec2& position)
{
    auto enemy = EliteEnemy::create();
    if (enemy) {
        enemy->setPosition(position);
        enemy->retain();
        return std::shared_ptr<EliteEnemy>(enemy, [](EliteEnemy* p) { p->release(); });
    }
    return nullptr;
}