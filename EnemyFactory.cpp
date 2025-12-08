#include "EnemyFactory.h"
#include "EnemyType.h"
#include "GameUtils.h"

// ==========================================
// [难度系统实现]
// 在 .cpp 内部维护难度状态，不修改 .h
// ==========================================
enum class DifficultyLevel {
    MILD,       // 温和 (Easy)
    MODERATE,   // 适中 (Normal)
    SEVERE      // 严酷 (Hard - 对应"期末复习过度疲劳")
};

static DifficultyLevel s_currentDifficulty = DifficultyLevel::MODERATE;

// 获取当前难度的数值倍率
static float getDifficultyMultiplier() {
    switch (s_currentDifficulty) {
    case DifficultyLevel::MILD: return 0.8f;
    case DifficultyLevel::MODERATE: return 1.0f;
    case DifficultyLevel::SEVERE: return 1.5f; // 1.5倍血量和伤害
    default: return 1.0f;
    }
}

// ==========================================
// 工厂方法实现
// ==========================================

std::shared_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, const cocos2d::Vec2& position)
{
    // 转发给带字符串的函数，使用默认空名
    return createEnemy("", position);
}

std::shared_ptr<Enemy> EnemyFactory::createEnemy(const std::string& enemyType, const cocos2d::Vec2& position)
{
    std::shared_ptr<Enemy> enemy = nullptr;

    // ----------------------------------------------------------------
    // [怪物名称解析] 对应《游戏故事+关卡设计》文档
    // ----------------------------------------------------------------

    // --- Level 1: 樱花大道 ---
    if (enemyType == "NormalEnemyA") {
        // 近战小兵
        enemy = createMeleeEnemy(position);
        if (enemy) enemy->setName("NormalEnemyA");
    }
    else if (enemyType == "NormalEnemyB") {
        // 远程小兵 (陆地)
        enemy = createRangedEnemy(position);
        if (enemy) enemy->setName("NormalEnemyB");
    }
    else if (enemyType == "CaiXunKun") {
        // BOSS 1: 菜需捆 (考试压力)
        enemy = createEliteEnemy(position);
        if (enemy) enemy->setName("CaiXunKun");
    }

    // --- Level 2: 体育馆 ---
    else if (enemyType == "NormalEnemyC") {
        // 远程飞行单位
        enemy = createRangedEnemy(position);
        if (enemy) enemy->setName("NormalEnemyC");
    }
    else if (enemyType == "NaiLong") {
        // BOSS 2: 奶龙 (体能透支)
        enemy = createEliteEnemy(position);
        if (enemy) enemy->setName("NaiLong");
    }

    // --- Level 3: 大剧院 ---
    else if (enemyType == "EliteMonsterA" || enemyType == "EliteMonsterB") {
        // 精英怪 A/B
        enemy = createMeleeEnemy(position); // 使用强化版 Melee
        if (enemy) enemy->setName(enemyType);
    }
    else if (enemyType == "MaoDie") {
        // BOSS 3: 耄耋
        enemy = createEliteEnemy(position);
        if (enemy) enemy->setName("MaoDie");
    }

    // --- Level 4: 教室 ---
    else if (enemyType == "KuMingYuanYang") {
        // BOSS 4
        enemy = createEliteEnemy(position);
        if (enemy) enemy->setName("KuMingYuanYang");
    }

    // --- 默认逻辑 ---
    else {
        // 如果传入的是类型枚举的字符串形式
        if (enemyType == "Melee" || enemyType.empty()) enemy = createMeleeEnemy(position);
        else if (enemyType == "Ranged") enemy = createRangedEnemy(position);
        else if (enemyType == "Elite") enemy = createEliteEnemy(position);
    }

    // 统一设置位置
    if (enemy) {
        enemy->setPosition(position);
    }

    return enemy;
}

std::shared_ptr<MeleeEnemy> EnemyFactory::createMeleeEnemy(const cocos2d::Vec2& position)
{
    // 使用 create() 创建 auto-release 对象，然后封装进 shared_ptr 并管理引用计数
    auto enemy = MeleeEnemy::create();
    if (enemy) {
        enemy->setPosition(position);
        enemy->retain(); // 必须 retain，否则 shared_ptr 析构时若引用计数为0会导致 double free 风险
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