// EnemyManager.cpp
#include "EnemyManager.h"
#include "Player.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "ShieldEnemy.h"

USING_NS_CC;

EnemyManager* EnemyManager::_instance = nullptr;

EnemyManager* EnemyManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new EnemyManager();
    }
    return _instance;
}

void EnemyManager::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

EnemyManager::EnemyManager() {
    _player = nullptr;
    initEnemyDatabase();
}

void EnemyManager::initialize() {
    // Initialize enemy manager
    _enemies.clear();
}

void EnemyManager::cleanup() {
    // Clean up all enemies
    removeAllEnemies();
    
    // Clean up enemy database
    _enemyDatabase.clear();
}

Enemy* EnemyManager::createEnemy(const std::string& enemyType, float worldX, float worldY) {
    Enemy* enemy = nullptr;
    
    // Create different enemy instances based on enemy type
    if (enemyType == "melee") {
        enemy = MeleeEnemy::create(enemyType);
    } else if (enemyType == "ranged") {
        enemy = RangedEnemy::create(enemyType);
    } else if (enemyType == "shield") {
        enemy = ShieldEnemy::create(enemyType);
    } else {
        CCLOG("Unknown enemy type: %s", enemyType.c_str());
        return nullptr;
    }
    
    if (enemy) {
        // Set enemy position
        enemy->setWorldPosition(worldX, worldY);
        
        // Set enemy's target to player
        if (_player) {
            enemy->setTarget(_player);
        }
        
        // Add to enemy list
        addEnemy(enemy);
    }
    
    return enemy;
}

void EnemyManager::addEnemy(Enemy* enemy) {
    if (enemy) {
        _enemies.push_back(enemy);
        enemy->retain(); // EnemyManager now owns this enemy
    }
}

void EnemyManager::removeEnemy(Enemy* enemy) {
    if (!enemy) {
        return;
    }
    
    // Remove enemy from list
    auto it = std::find(_enemies.begin(), _enemies.end(), enemy);
    if (it != _enemies.end()) {
        _enemies.erase(it);
    }
    
    // Safely delete enemy
    safelyRemoveEnemy(enemy);
}

void EnemyManager::removeAllEnemies() {
    for (auto enemy : _enemies) {
        safelyRemoveEnemy(enemy);
    }
    _enemies.clear();
}

void EnemyManager::update(float delta) {
    // Iterate through enemy list and update each enemy
    // Create a copy of the list to avoid iterator invalidation during removal
    std::vector<Enemy*> enemiesCopy = _enemies;
    
    for (auto enemy : enemiesCopy) {
        if (enemy) {
            if (!enemy->isDead()) {
                // Only update living enemies
                enemy->update(delta);
            }
            // Dead enemies will be removed by their own death animation sequence
        }
    }
}

void EnemyManager::setPlayer(Player* player) {
    _player = player;
    
    // Update target for all enemies
    for (auto enemy : _enemies) {
        if (enemy) {
            enemy->setTarget(player);
        }
    }
}

std::vector<Enemy*> EnemyManager::getEnemiesByType(const std::string& enemyType) {
    std::vector<Enemy*> result;
    
    for (auto enemy : _enemies) {
        if (enemy && enemy->getEnemyType() == enemyType) {
            result.push_back(enemy);
        }
    }
    
    return result;
}

bool EnemyManager::hasAliveEnemies() const {
    for (auto enemy : _enemies) {
        if (enemy && !enemy->isDead()) {
            return true;
        }
    }
    return false;
}

int EnemyManager::getAliveEnemiesCount() const {
    int count = 0;
    for (auto enemy : _enemies) {
        if (enemy && !enemy->isDead()) {
            ++count;
        }
    }
    return count;
}

void EnemyManager::registerEnemyType(const std::string& enemyType, const std::string& name, const std::string& description) {
    // Check if enemy type already exists
    auto it = _enemyDatabase.find(enemyType);
    if (it != _enemyDatabase.end()) {
        // Update existing enemy type
        it->second.name = name;
        it->second.description = description;
    } else {
        // Create new enemy type configuration
        EnemyConfig data;
        data.spriteName = name;
        data.name = name;
        data.description = description;
        _enemyDatabase[enemyType] = data;
    }
}

std::string EnemyManager::getEnemyName(const std::string& enemyType) const {
    auto it = _enemyDatabase.find(enemyType);
    if (it != _enemyDatabase.end()) {
        return it->second.name;
    }
    return "Unknown Enemy";
}

std::string EnemyManager::getEnemyDescription(const std::string& enemyType) const {
    auto it = _enemyDatabase.find(enemyType);
    if (it != _enemyDatabase.end() && !it->second.description.empty()) {
        return it->second.description;
    }
    
    // Fallback to generic description if no specific description is set
    if (enemyType == "melee") {
        return "A common enemy with weapons that attacks players at close range.";
    } else if (enemyType == "ranged") {
        return "An enemy that attacks players with projectiles and has a longer attack range.";
    } else if (enemyType == "shield") {
        return "An enemy with a shield that can block player attacks.";
    }
    return "No description available";
}

void EnemyManager::initEnemyDatabase() {
    // Register various enemy types
    registerEnemyType("melee", "Melee Enemy", "");
    registerEnemyType("ranged", "Ranged Enemy", "");
    registerEnemyType("shield", "Shield Enemy", "");
}

void EnemyManager::safelyRemoveEnemy(Enemy* enemy) {
    if (!enemy) {
        return;
    }
    
    // Do not stop all actions here as it might cancel the death animation sequence
    // enemy->stopAllActions();
    
    // Remove physics collision body if it still exists
    if (enemy->getPhysicsBody()) {
        enemy->setPhysicsBody(nullptr);
    }
    
    // Remove from parent node
    if (enemy->getParent()) {
        enemy->removeFromParentAndCleanup(true);
    } else {
        // If enemy has no parent, we still need to release it
        enemy->release();
    }
}