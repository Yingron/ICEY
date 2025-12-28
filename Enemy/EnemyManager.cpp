// EnemyManager.cpp
// 鍦ㄦ枃浠堕《閮ㄧ‘淇濆寘鍚簡蹇呰鐨勫ご鏂囦欢
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

    // 妫€鏌ユ晫浜烘槸鍚﹀凡缁忓湪绉婚櫎鍒楄〃涓?
    for (int i = 0; i < (int)_enemiesToRemove.size(); i++) {
        if (_enemiesToRemove[i] == enemy) {
            log("Enemy already in removal queue, skipping");
            return;
        }
    }

    // 妫€鏌ユ晫浜烘槸鍚﹀凡缁忓湪涓诲垪琛ㄤ腑
    bool foundInMainList = false;
    for (int i = 0; i < (int)_enemies.size(); i++) {
        if (_enemies[i] == enemy) {
            foundInMainList = true;
            break;
        }
    }

    if (!foundInMainList) {
        log("Enemy not found in main list, may have been already removed");
        safelyRemoveEnemy(enemy); // 鐩存帴瀹夊叏鍒犻櫎
        return;
    }

    // 鏍囪涓烘鍦ㄧЩ闄?
    enemy->setTag(-1); // 浣跨敤璐熷€兼爣璁颁负姝ｅ湪绉婚櫎

    // 灏嗘晫浜烘坊鍔犲埌寤惰繜绉婚櫎鍒楄〃
    _enemiesToRemove.push_back(enemy);
    log("Enemy added to removal queue, total in queue: %d", _enemiesToRemove.size());
}
void EnemyManager::removeAllEnemies()
{
    // 鍒涘缓涓存椂鍒楄〃閬垮厤鍦ㄩ亶鍘嗘椂淇敼鍘熷垪琛?
    std::vector<Enemy*> allEnemies;
    
    // 鍏堝鐞嗗欢杩熺Щ闄ら槦鍒椾腑鐨勬晫浜?
    if (!_enemiesToRemove.empty()) {
        allEnemies.insert(allEnemies.end(), _enemiesToRemove.begin(), _enemiesToRemove.end());
        _enemiesToRemove.clear();
    }
    
    // 娣诲姞褰撳墠鏁屼汉鍒楄〃涓殑鏁屼汉
    allEnemies.insert(allEnemies.end(), _enemies.begin(), _enemies.end());
    _enemies.clear();
    
    // 瀹夊叏绉婚櫎鎵€鏈夋晫浜?
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

    // 鍒涘缓涓存椂鍒楄〃澶勭悊锛岄伩鍏嶅湪閬嶅巻鏃朵慨鏀?
    std::vector<Enemy*> toRemove = _enemiesToRemove;
    _enemiesToRemove.clear();

    for (int i = 0; i < (int)toRemove.size(); i++) {
        Enemy* enemy = toRemove[i];
        if (!enemy) {
            continue;
        }

        // 浠庝富鏁屼汉鍒楄〃涓Щ闄?
        bool removedFromMainList = false;
        for (int j = 0; j < (int)_enemies.size(); j++) {
            // 妫€鏌enemies[j]鏄惁涓簄ullptr
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

        // 瀹夊叏鍒犻櫎鏁屼汉
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
    
    // 澶勭悊寤惰繜绉婚櫎
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
            // 妫€鏌ユ晫浜烘槸鍚﹀湪绉婚櫎鍒楄〃涓?
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
            // 妫€鏌ユ晫浜烘槸鍚﹀湪绉婚櫎鍒楄〃涓?
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
            // 妫€鏌ユ晫浜烘槸鍚﹀凡缁忓湪绉婚櫎鍒楄〃涓?
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            // 鍙湁涓嶅湪绉婚櫎鍒楄〃涓殑鏁屼汉鎵嶈鍏ユ椿鐫€鐨勬晫浜?
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
            // 妫€鏌ユ晫浜烘槸鍚﹀凡缁忓湪绉婚櫎鍒楄〃涓?
            bool isInRemovalList = false;
            for (int j = 0; j < (int)_enemiesToRemove.size(); j++)
            {
                if (_enemiesToRemove[j] == enemy)
                {
                    isInRemovalList = true;
                    break;
                }
            }
            
            // 鍙湁涓嶅湪绉婚櫎鍒楄〃涓殑鏁屼汉鎵嶈鍏ユ椿鐫€鐨勬晫浜?
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
    // Level 1浣滀负鏁欏鍏充笉鐢熶骇鏁屼汉
    _levelEnemySequence[LevelManager::LevelState::LEVEL1] = {};
    
    // Level 2-i鐢熸垚i涓晫浜猴紝鏁屼汉绉嶇被闅忔満鍐冲畾锛孡evel 2-6鐢熸垚BOSS1
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_1] = {"melee"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_2] = {"melee", "ranged"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_3] = {"melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_4] = {"melee", "melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_5] = {"melee", "ranged", "ranged", "shield", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL2_6] = {"BOSS1-CAIXUNKUN"};
    
    // Level 3 follows Level 2's rules, first 5 levels generate enemies corresponding to level numbers, last level generates BOSS2
    // According to requirements, LEVEL3_1 has no enemies, LEVEL3_2 also has no enemies
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_1] = {};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_2] = {}; // No enemies for this level
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_3] = {"melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_4] = {"melee", "melee", "ranged", "shield"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_5] = {"melee", "ranged", "shield", "shield", "melee"};
    _levelEnemySequence[LevelManager::LevelState::LEVEL3_6] = {"BOSS2-MAODIE"};
    
    // Level 4鍚孡evel 2鐨勯€昏緫锛屽墠5鍏崇敓鎴愬拰鍏冲崱鏁板搴旂殑鏁屼汉
    // 鏍规嵁鐢ㄦ埛瑕佹眰锛孡EVEL4_3鍜孡EVEL4_5涓嶇敓鎴愭晫浜猴紝LEVEL4_4鍙敓鎴怋OSS3锛孡EVEL4_6涓嶇敓鎴愭晫浜?
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

    // 纭繚鏁屼汉澶勪簬姝讳骸鐘舵€?
    enemy->setCurrentState(EnemyState::DEAD);

    // 鍋滄鎵€鏈夊姩浣?
    enemy->stopAllActions();
    log("Stopped all enemy actions");

    // 绉婚櫎鐗╃悊纰版挒浣?
    auto physicsBody = enemy->getPhysicsBody();
    if (physicsBody) {
        physicsBody->setEnabled(false);
        // 涓嶇洿鎺ヨ皟鐢╯etPhysicsBody(nullptr)浠ラ伩鍏峜ocos2d-x鍐呴儴鏂█澶辫触
        // 鐗╃悊浣撳皢鍦ㄨ妭鐐归攢姣佹椂鑷姩娓呯悊
        log("Disabled enemy physics body");
    }

    // 绉婚櫎鎵€鏈夊瓙鑺傜偣骞舵竻鐞?
    enemy->removeAllChildrenWithCleanup(true);
    log("Removed all enemy children");

    // 浠庣埗鑺傜偣绉婚櫎鎴栫洿鎺ラ噴鏀?
    if (enemy->getParent()) {
        enemy->removeFromParentAndCleanup(true);
        log("Enemy removed from parent");
    }
    else {
        // 濡傛灉鏁屼汉娌℃湁鐖惰妭鐐癸紝鐩存帴閲婃斁
        enemy->release();
        log("Enemy released (no parent)");
    }
    
    log("Enemy safe removal completed");
}