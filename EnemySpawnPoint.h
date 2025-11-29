#pragma once
#ifndef __ENEMY_SPAWN_POINT_H__
#define __ENEMY_SPAWN_POINT_H__

#include "cocos2d.h"
#include "EnemyType.h"

class EnemySpawnPoint
{
private:
    cocos2d::Vec2 m_position;
    EnemyType m_enemyType;
    bool m_hasSpawned;

public:
    EnemySpawnPoint(const cocos2d::Vec2& position, EnemyType type);

    cocos2d::Vec2 getPosition() const { return m_position; }
    EnemyType getEnemyType() const { return m_enemyType; }
    bool hasSpawned() const { return m_hasSpawned; }

    void markAsSpawned() { m_hasSpawned = true; }
    void reset() { m_hasSpawned = false; }
};

#endif // __ENEMY_SPAWN_POINT_H__