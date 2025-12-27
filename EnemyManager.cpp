// EnemyManager.cpp
#include "EnemyManager.h"
#include "Player.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "ShieldEnemy.h"
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

    // 检查敌人是否已经在移除列表中
    for (int i = 0; i < (int)_enemiesToRemove.size(); i++) {
        if (_enemiesToRemove[i] == enemy) {
            log("Enemy already in removal queue, skipping");
            return;
        }
    }

    // 检查敌人是否已经在主列表中
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
    enemy->setTag(-1); // 使用负值标记为正在移除

    // 将敌人添加到延迟移除列表
    _enemiesToRemove.push_back(enemy);
    log("Enemy added to removal queue, total in queue: %d", _enemiesToRemove.size());
}
void EnemyManager::removeAllEnemies()
{
    // 创建临时列表避免在遍历时修改原列表
    std::vector<Enemy*> allEnemies;
    
    // 先处理延迟移除队列中的敌人
    if (!_enemiesToRemove.empty()) {
        allEnemies.insert(allEnemies.end(), _enemiesToRemove.begin(), _enemiesToRemove.end());
        _enemiesToRemove.clear();
    }
    
    // 添加当前敌人列表中的敌人
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

    // 创建临时列表处理，避免在遍历时修改
    std::vector<Enemy*> toRemove = _enemiesToRemove;
    _enemiesToRemove.clear();

    for (int i = 0; i < (int)toRemove.size(); i++) {
        Enemy* enemy = toRemove[i];
        if (!enemy) {
            continue;
        }

        // 从主敌人列表中移除
        bool removedFromMainList = false;
        for (int j = 0; j < (int)_enemies.size(); j++) {
            // 检查_enemies[j]是否为nullptr
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
        if (enemy && enemy->getEnemyType() == enemyType)
        {
            result.push_back(enemy);
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
            return true;
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
            ++count;
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

    // 移除物理碰撞体
    auto physicsBody = enemy->getPhysicsBody();
    if (physicsBody) {
        physicsBody->setEnabled(false);
        // 不直接调用setPhysicsBody(nullptr)以避免cocos2d-x内部断言失败
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
    
    // EnemyManager的引用已经在removeFromParentAndCleanup或直接release中处理了

    // 从移除列表中清除（如果存在）
    auto it = std::find(_enemiesToRemove.begin(), _enemiesToRemove.end(), enemy);
    if (it != _enemiesToRemove.end()) {
        _enemiesToRemove.erase(it);
        log("Enemy removed from removal queue");
    }

    // 从主敌人列表中清除（如果存在）
    it = std::find(_enemies.begin(), _enemies.end(), enemy);
    if (it != _enemies.end()) {
        _enemies.erase(it);
        log("Enemy removed from main enemies list");
    }

    log("Enemy safe removal completed");
}