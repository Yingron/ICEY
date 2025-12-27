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

void EnemyManager::removeEnemy(Enemy* enemy)
{
    if (!enemy)
    {
        return;
    }
    
    // 检查敌人是否已经在移除列表中
    for (int i = 0; i < (int)_enemiesToRemove.size(); i++)
    {
        if (_enemiesToRemove[i] == enemy)
        {
            return; // 已经在移除列表中，直接返回
        }
    }
    
    // 将敌人添加到延迟移除列表
    _enemiesToRemove.push_back(enemy);
    log("Enemy added to removal queue");
}

void EnemyManager::removeAllEnemies()
{
    for (int i = 0; i < (int)_enemies.size(); i++)
    {
        safelyRemoveEnemy(_enemies[i]);
    }
    _enemies.clear();
}

void EnemyManager::processEnemyRemoval()
{
    if (_enemiesToRemove.empty())
    {
        return;
    }
    
    // 处理所有需要移除的敌人
    for (int i = 0; i < (int)_enemiesToRemove.size(); i++)
    {
        Enemy* enemy = _enemiesToRemove[i];
        if (!enemy)
        {
            continue;
        }
        
        // 从敌人列表中移除
        for (int j = 0; j < (int)_enemies.size(); j++)
        {
            if (_enemies[j] == enemy)
            {
                _enemies.erase(_enemies.begin() + j);
                break;
            }
        }
        
        // 安全删除敌人
        safelyRemoveEnemy(enemy);
    }
    
    // 清空移除列表
    _enemiesToRemove.clear();
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

void EnemyManager::safelyRemoveEnemy(Enemy* enemy)
{
    if (!enemy)
    {
        return;
    }
    
    // Do not stop all actions here as it might cancel the death animation sequence
    // enemy->stopAllActions();
    
    // Remove physics collision body if it still exists
    if (enemy->getPhysicsBody())
    {
        enemy->setPhysicsBody(nullptr);
    }
    
    // Remove from parent node
    if (enemy->getParent())
    {
        enemy->removeFromParentAndCleanup(true);
    } else
    {
        // If enemy has no parent, we still need to release it
        enemy->release();
    }
}