// EnemyManager.cpp
// 在文件头部已包含关键的头文件
#include "EnemyManager.h"
#include "Player.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "ShieldEnemy.h"
#include "LevelManager.h"
#include <algorithm>

USING_NS_CC; 

EnemyManager* EnemyManager::_instance = nullptr;

EnemyManager* EnemyManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new EnemyManager();
    }
    return _instance;
}

void EnemyManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

EnemyManager::EnemyManager()
{
    _player = nullptr;
    initEnemyDatabase();
    initLevelEnemySequences();
}

EnemyManager::~EnemyManager()
{
    // Clean up all enemies
    removeAllEnemies();
    
    // Clear all lists to avoid dangling pointers
    _enemies.clear();
    _enemiesToRemove.clear();
    _enemyDatabase.clear();
    
    // Release player reference
    _player = nullptr;
    
    log("EnemyManager destructor called, all resources cleaned up");
}

void EnemyManager::initialize()
{
    // Initialize enemy manager
    _enemies.clear();
    _enemiesToRemove.clear();
    
    // Initialize enemy database and level enemy sequences
    initEnemyDatabase();
    initLevelEnemySequences();
}

void EnemyManager::cleanup()
{
    // Clean up all enemies
    removeAllEnemies();
    
    // Clean up enemy database
    _enemyDatabase.clear();
}

Enemy* EnemyManager::createEnemy(const std::string& enemyType, float worldX, float worldY)
{
    Enemy* enemy = nullptr;
    
    // Create different enemy instances based on enemy type
    if (enemyType == "melee" || enemyType == "BOSS1-CAIXUNKUN")
    {
        // BOSS1-CAIXUNKUN is a melee boss
        enemy = MeleeEnemy::create(enemyType);
    } else if (enemyType == "ranged" || enemyType == "BOSS3-NAILONG")
    {
        // BOSS3-NAILONG is a ranged boss
        enemy = RangedEnemy::create(enemyType);
    } else if (enemyType == "shield" || enemyType == "BOSS2-MAODIE")
    {
        // BOSS2-MAODIE is a shield boss
        enemy = ShieldEnemy::create(enemyType);
    } else
    {
        CCLOG("Unknown enemy type: %s", enemyType.c_str());
        return nullptr;
    }
    
    if (enemy)
    {
        // Set enemy position
        enemy->setWorldPosition(worldX, worldY);
        
        // Set enemy's target to player
        if (_player)
        {
            enemy->setTarget(_player);
        }
        
        // Add to enemy list
        addEnemy(enemy);
    }
    
    return enemy;
}

void EnemyManager::addEnemy(Enemy* enemy)
{
    if (enemy)
    {
        _enemies.push_back(enemy);
        enemy->retain(); // EnemyManager now owns this enemy
    }
}

void EnemyManager::removeEnemy(Enemy* enemy) {
    if (!enemy) {
        log("ERROR: Attempted to remove null enemy");
        return;
    }

    // 检查敌人是否已经在移除队列
    for (int i = 0; i < (int)_enemiesToRemove.size(); i++) {
        if (_enemiesToRemove[i] == enemy) {
            log("Enemy already in removal queue, skipping");
            return;
        }
    }

    // 检查敌人是否在主列表
    bool foundInMainList = false;
    for (int i = 0; i < (int)_enemies.size(); i++) {
        if (_enemies[i] == enemy) {
            foundInMainList = true;
            break;
        }
    }

    if (!foundInMainList) {
        log("Enemy not found in main list, may have been already removed");
        safelyRemoveEnemy(enemy); // 直接安全删除
        return;
    }

    // 标记为正在移除
    enemy->setTag(-1); // 使用特殊标签标记正在移除

    // 加入延迟移除队列
    _enemiesToRemove.push_back(enemy);
    log("Enemy added to removal queue, total in queue: %d", _enemiesToRemove.size());
}
void EnemyManager::removeAllEnemies()
{
    // 创建临时列表，避免遍历时修改原列表
    std::vector<Enemy*> allEnemies;
    
    // 先处理已在移除队列中的敌人
    if (!_enemiesToRemove.empty()) {
        allEnemies.insert(allEnemies.end(), _enemiesToRemove.begin(), _enemiesToRemove.end());
        _enemiesToRemove.clear();
    }
    
    // 再添加当前敌人列表中的敌人
    allEnemies.insert(allEnemies.end(), _enemies.begin(), _enemies.end());
    _enemies.clear();
    
    // 安全移除所有敌人
    for (int i = 0; i < (int)allEnemies.size(); i++) {
        Enemy* enemy = allEnemies[i];
        if (enemy) {
            safelyRemoveEnemy(enemy);
        }
    }
}

void EnemyManager::processEnemyRemoval() {
    if (_enemiesToRemove.empty()) {
        return;
    }

    // 创建临时列表处理，避免遍历时修改
    std::vector<Enemy*> toRemove = _enemiesToRemove;
    _enemiesToRemove.clear();

    for (int i = 0; i < (int)toRemove.size(); i++) {
        Enemy* enemy = toRemove[i];
        if (!enemy) {
            continue;
        }

        // 从主列表移除
        bool removedFromMainList = false;
        for (int j = 0; j < (int)_enemies.size(); j++) {
            // 检查 _enemies[j] 是否为 nullptr
            if (_enemies[j] && _enemies[j] == enemy) {
                _enemies.erase(_enemies.begin() + j);
                removedFromMainList = true;
                log("Removed enemy from main list");
                break;
            }
        }

        if (!removedFromMainList) {
            log("WARNING: Enemy not found in main list during removal processing");
        }

        // 安全删除敌人
        safelyRemoveEnemy(enemy);
    }
}

void EnemyManager::update(float delta)
{
    // Iterate through enemy list and update each enemy
    // Create a copy of the list to avoid iterator invalidation during removal
    std::vector<Enemy*> enemiesCopy = _enemies;
    
    for (int i = 0; i < (int)enemiesCopy.size(); i++)
    {
        Enemy* enemy = enemiesCopy[i];
        if (enemy)
        {
            if (!enemy->isDead())
            {
                // Only update living enemies
                enemy->update(delta);
            }
            // Dead enemies will be removed by their own death animation sequence
        }
    }
    
    // 处理延迟移除
    processEnemyRemoval();
}

void EnemyManager::setPlayer(Player* player)
{
    _player = player;
    
    // Update target for all enemies
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        Enemy* enemy = _enemies[i];
        if (enemy)
        {
            enemy->setTarget(player);
        }
    }
}

std::vector<Enemy*> EnemyManager::getEnemiesByType(const std::string& enemyType)
{
    std::vector<Enemy*> result;
    
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        Enemy* enemy = _enemies[i];
        if (enemy && enemy->getEnemyType() == enemyType && !enemy->isDead())
        {
            // 检查敌人是否在移除队列
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            if (!isInRemovalList)
            {
                result.push_back(enemy);
            }
        }
    }
    
    return result;
}

std::vector<Enemy*> EnemyManager::getAllEnemies() const
{
    std::vector<Enemy*> result;
    
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        Enemy* enemy = _enemies[i];
        if (enemy && !enemy->isDead())
        {
            // 检查敌人是否在移除队列
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            if (!isInRemovalList)
            {
                result.push_back(enemy);
            }
        }
    }
    
    return result;
}

bool EnemyManager::hasAliveEnemies() const
{
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        Enemy* enemy = _enemies[i];
        if (enemy && !enemy->isDead())
        {
            // 检查是否已在移除队列
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            // 只有未在移除队列的敌人才算存活
            if (!isInRemovalList)
            {
                return true;
            }
        }
    }
    return false;
}

int EnemyManager::getAliveEnemiesCount() const
{
    int count = 0;
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        Enemy* enemy = _enemies[i];
        if (enemy && !enemy->isDead())
        {
            // 检查是否已在移除队列
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            // 只有未在移除队列的敌人才计为存活
            if (!isInRemovalList)
            {
                ++count;
            }
        }
    }
    return count;
}

void EnemyManager::registerEnemyType(const std::string& enemyType, const std::string& name, const std::string& description)
{
    // Update or create enemy type configuration
    _enemyDatabase[enemyType].name = name;
    _enemyDatabase[enemyType].description = description;
    _enemyDatabase[enemyType].spriteName = name;
}

std::string EnemyManager::getEnemyName(const std::string& enemyType) const
{
    if (_enemyDatabase.find(enemyType) != _enemyDatabase.end())
    {
        return _enemyDatabase.at(enemyType).name;
    }
    return "Unknown Enemy";
}

std::string EnemyManager::getEnemyDescription(const std::string& enemyType) const
{
    if (_enemyDatabase.find(enemyType) != _enemyDatabase.end() && !_enemyDatabase.at(enemyType).description.empty())
    {
        return _enemyDatabase.at(enemyType).description;
    }
    
    // Fallback to generic description if no specific description is set
    if (enemyType == "melee")
    {
        return "A common enemy with weapons that attacks players at close range.";
    } else if (enemyType == "ranged")
    {
        return "An enemy that attacks players with projectiles and has a longer attack range.";
    } else if (enemyType == "shield")
    {
        return "An enemy with a shield that can block player attacks.";
    }
    return "No description available";
}

void EnemyManager::initEnemyDatabase()
{
    // Register various enemy types
    registerEnemyType("melee", "Melee Enemy", "A common enemy with weapons that attacks players at close range.");
    registerEnemyType("ranged", "Ranged Enemy", "An enemy that attacks players with projectiles and has a longer attack range.");
    registerEnemyType("shield", "Shield Enemy", "An enemy with a shield that can block player attacks.");
    
    // Register boss enemy types
    registerEnemyType("BOSS1-CAIXUNKUN", "Caixunkun Boss", "The first boss, a powerful melee fighter with devastating attacks.");
    registerEnemyType("BOSS2-MAODIE", "Maodie Boss", "The second boss, using a shield to block attacks and counter with strong strikes.");
    registerEnemyType("BOSS3-NAILONG", "Nailong Boss", "The third boss, a ranged attacker that fires powerful projectiles.");
}

void EnemyManager::initLevelEnemySequences()
{
    // Level 1 作为新手关，不生成敌人
    _levelEnemySequence[LevelManager::LevelState::LEVEL1] = {};
    
    // Level 2-i 生成 i 个敌人，类型随机；Level 2-6 生成 BOSS1
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_1] = {"melee"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_2] = {"melee", "ranged"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_3] = {"melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_4] = {"melee", "melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_5] = {"melee", "ranged", "ranged", "shield", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_6] = {"BOSS1-CAIXUNKUN"};
    
    // Level 3 按 Level 2 规则：前 5 关按关卡编号生成敌人，最后一关生成 BOSS2
    // 按需求，LEVEL3_1 无敌人，LEVEL3_2 也无敌人
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_1] = {};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_2] = {}; // No enemies for this level
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_3] = {"melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_4] = {"melee", "melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_5] = {"melee", "ranged", "shield", "shield", "melee"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_6] = {"BOSS2-MAODIE"};
    
    // Level 4 按 Level 2 规则：前 5 关生成与关卡编号对应数量的敌人
    // 根据需求：LEVEL4_3 和 LEVEL4_5 不生成敌人，LEVEL4_4 只生成 BOSS3，LEVEL4_6 不生成敌人
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_1] = {"ranged"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_2] = {"ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_3] = {};
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_4] = {"BOSS3-NAILONG"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_5] = {};
    _levelEnemySequence[LevelManager::LevelState::LEVEL4_6] = {};
}

void EnemyManager::safelyRemoveEnemy(Enemy* enemy) {
    if (!enemy) {
        log("WARNING: Attempted to safely remove null enemy");
        return;
    }

    log("Starting safe removal of enemy");

    // 确保敌人处于死亡状态
    enemy->setCurrentState(EnemyState::DEAD);

    // 停止所有动作
    enemy->stopAllActions();
    log("Stopped all enemy actions");

    // 禁用物理碰撞体
    auto physicsBody = enemy->getPhysicsBody();
    if (physicsBody) {
        physicsBody->setEnabled(false);
        // 不直接调用 setPhysicsBody(nullptr) 以避免 cocos2d-x 内部断言
        // 物理体将在节点销毁时自动清理
        log("Disabled enemy physics body");
    }

    // 移除所有子节点并清理
    enemy->removeAllChildrenWithCleanup(true);
    log("Removed all enemy children");

    // 从父节点移除或直接释放
    if (enemy->getParent()) {
        enemy->removeFromParentAndCleanup(true);
        log("Enemy removed from parent");
    }
    else {
        // 如果敌人没有父节点，直接释放
        enemy->release();
        log("Enemy released (no parent)");
    }
    
    log("Enemy safe removal completed");
}
