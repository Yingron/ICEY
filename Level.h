#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Platform.h"
#include "Enemy.h"
#include "SceneTrigger.h"
#include <vector>
#include <memory>

class Level : public cocos2d::Node
{
private:
    cocos2d::TMXTiledMap* m_tileMap;
    std::vector<std::shared_ptr<Platform>> m_platforms;
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    std::vector<std::shared_ptr<SceneTrigger>> m_triggers;
    std::vector<cocos2d::Vec2> m_enemySpawnPoints;

    cocos2d::Rect m_worldBounds;
    std::string m_levelName;

public:
    static Level* createWithTMX(const std::string& tmxFile);
    virtual bool initWithTMX(const std::string& tmxFile);
    virtual void update(float delta) override;

    void spawnEnemies();
    void spawnEnemyAtPoint(const cocos2d::Vec2& point, EnemyType type);
    void checkTriggers(const cocos2d::Vec2& playerPosition);

    // Åö×²¼ì²â
    bool checkCollision(const cocos2d::Rect& rect) const;
    cocos2d::Vec2 getGroundPosition(const cocos2d::Vec2& position) const;

    // »ñÈ¡Æ÷
    cocos2d::TMXTiledMap* getTileMap() const { return m_tileMap; }
    cocos2d::Rect getWorldBounds() const { return m_worldBounds; }
    std::string getLevelName() const { return m_levelName; }

private:
    void parseTileMap();
    void createPhysicsBodies();
    void createPlatforms();
    void createTriggers();
    void setupCamera();

    CREATE_FUNC(Level);
};

#endif // __LEVEL_H__