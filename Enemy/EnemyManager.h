// EnemyManager.h
#pragma once

#include "cocos2d.h"
#include "Enemy.h"
#include "LevelManager.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// Forward declaration
class Player;

// Enemy configuration structure
struct EnemyConfig {
    std::string spriteName;
    float maxHealth;
    float attackDamage;
    float moveSpeed;
    float attackRange;
    float detectionRange;
    float attackCooldown;
    std::string name;
    std::string description;
};

class EnemyManager : public cocos2d::Ref
{
private:
    EnemyManager();
    static EnemyManager* _instance;
    
    Player* _player;
    std::vector<Enemy*> _enemies;
    std::vector<Enemy*> _enemiesToRemove; // 锟接筹拷锟狡筹拷锟叫憋拷
    std::map<std::string, EnemyConfig> _enemyDatabase;
    
    // 锟斤拷锟节关匡拷锟侥碉拷锟斤拷锟斤拷锟斤拷映锟斤拷锟?
    std::map<LevelManager::LevelState, std::vector<std::string>> _levelEnemySequence;
    
    void processEnemyRemoval(); // 锟斤拷锟斤拷锟接筹拷锟狡筹拷
    void initLevelEnemySequences(); // 锟斤拷始锟斤拷锟截匡拷锟斤拷锟斤拷锟斤拷锟斤拷
  
public:
    static EnemyManager* getInstance();
    static void destroyInstance();
    
    void initialize();
    void cleanup();
    
    void setPlayer(Player* player);
    Player* getPlayer() const { return _player; }
    
    Enemy* createEnemy(const std::string& enemyType, float worldX, float worldY);
    void addEnemy(Enemy* enemy);
    void removeEnemy(Enemy* enemy);
    void removeAllEnemies();
    
    std::vector<Enemy*> getEnemies() const { return _enemies; }
    int getEnemyCount() const { return _enemies.size(); }
    
    void update(float delta);
    
    // Additional functions
    std::vector<Enemy*> getEnemiesByType(const std::string& enemyType);// Getters
    bool hasAliveEnemies() const;
    int getAliveEnemiesCount() const;
    std::vector<Enemy*> getAllEnemies() const; // 鍙繑鍥炵湡姝ｆ椿鐫€鐨勬晫浜?
    
    // 锟斤拷取锟截讹拷锟截匡拷锟侥碉拷锟斤拷锟斤拷锟斤拷
    // 锟斤拷取锟截讹拷锟截匡拷锟侥碉拷锟斤拷锟斤拷锟斤拷
    std::vector<std::string> getLevelEnemySequence(LevelManager::LevelState level) const
    {
        std::vector<std::string> result;
        auto it = _levelEnemySequence.find(level);
        if (it != _levelEnemySequence.end())
        {
            result = it->second;
        }
        return result;
    }
    
    void registerEnemyType(const std::string& enemyType, const std::string& name, const std::string& description);
    std::string getEnemyName(const std::string& enemyType) const;
    std::string getEnemyDescription(const std::string& enemyType) const;
    
private:
    void initEnemyDatabase();
    EnemyConfig getEnemyConfig(const std::string& enemyType) const;
    void safelyRemoveEnemy(Enemy* enemy);
    
    // Destructor
    ~EnemyManager();
};
