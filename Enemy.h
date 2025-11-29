#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "EnemyState.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include "Player.h"
#include <memory>

class Enemy : public cocos2d::Sprite
{
protected:
    float m_health;
    float m_maxHealth;
    float m_damage;
    float m_speed;
    float m_attackRange;
    float m_detectionRange;

    EnemyState m_aiState;
    EnemyType m_type;

    Player* m_targetPlayer;
    Hurtbox m_hurtbox;

    float m_attackCooldown;
    float m_attackTimer;

public:
    virtual bool init() override;
    virtual void update(float delta) override;

    // AI 行为
    virtual void updateAI(float delta);
    virtual void patrol(float delta);
    virtual void chasePlayer(float delta);
    virtual void attack();

    // 战斗
    virtual void takeDamage(float damage);
    virtual void die();

    // 状态控制
    void setState(EnemyState newState);
    EnemyState getState() const { return m_aiState; }
    EnemyType getType() const { return m_type; }

    // 目标设置
    void setTargetPlayer(Player* player) { m_targetPlayer = player; }
    Player* getTargetPlayer() const { return m_targetPlayer; }

    // 属性获取器
    float getHealth() const { return m_health; }
    float getMaxHealth() const { return m_maxHealth; }
    float getDamage() const { return m_damage; }

    // 碰撞检测
    virtual Hurtbox& getHurtbox() { return m_hurtbox; }
    virtual void updateHurtbox();

    virtual bool isPlayerInRange() const;
    virtual bool isPlayerInAttackRange() const;

protected:
    virtual std::shared_ptr<Hitbox> createAttackHitbox();

    CREATE_FUNC(Enemy);
};

#endif // __ENEMY_H__