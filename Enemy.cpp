#include "Enemy.h"
#include "GameUtils.h"
#include <stdexcept>

bool Enemy::init()
{
    if (!cocos2d::Sprite::init()) return false;

    // [中文注释]: 初始化基础数值
    m_health = 100.0f;
    m_maxHealth = 100.0f;
    m_damage = 10.0f;
    m_speed = 100.0f;
    m_attackRange = 50.0f;
    m_detectionRange = 300.0f;
    m_attackCooldown = 2.0f;
    m_attackTimer = 0.0f;

    m_aiState = EnemyState::IDLE;
    m_targetPlayer = nullptr;

    this->scheduleUpdate();
    return true;
}

void Enemy::update(float delta)
{
    // [异常处理]: 全局 Update 保护，防止某一帧逻辑错误导致游戏崩溃
    try {
        if (m_aiState == EnemyState::DEAD) return;

        if (m_attackTimer > 0.0f) m_attackTimer -= delta;

        // 执行核心AI
        updateAI(delta);

        // [中文注释]: 更新碰撞盒位置，使其跟随精灵移动
        updateHurtbox();
    }
    catch (const std::exception& e) {
        GameUtils::log(std::string("Enemy Update Error: ") + e.what());
    }
}

void Enemy::updateAI(float delta)
{
    if (!m_targetPlayer) return;

    // [中文注释]: 基础距离检测
    float dist = this->getPosition().distance(m_targetPlayer->getPosition());

    switch (m_aiState)
    {
    case EnemyState::IDLE:
        if (dist <= m_detectionRange) setState(EnemyState::CHASE);
        break;
    case EnemyState::CHASE:
        // [范围索敌]: 基础距离判定，具体的四方位判定在攻击函数中细化
        if (dist <= m_attackRange) {
            if (m_attackTimer <= 0.0f) setState(EnemyState::ATTACK);
        }
        else if (dist > m_detectionRange * 1.5f) {
            setState(EnemyState::IDLE);
        }
        else {
            chasePlayer(delta);
        }
        break;
    case EnemyState::ATTACK:
        attack();
        break;
    }
}

void Enemy::chasePlayer(float delta)
{
    // [中文注释]: X轴方向移动逻辑
    float dirX = (m_targetPlayer->getPositionX() > this->getPositionX()) ? 1.0f : -1.0f;
    float newX = this->getPositionX() + dirX * m_speed * delta;
    this->setPositionX(newX);
    this->setFlippedX(dirX < 0); // 根据移动方向翻转贴图
}

void Enemy::takeDamage(float damage)
{
    if (m_aiState == EnemyState::DEAD) return;
    m_health -= damage;

    // [中文注释]: 受击闪白效果
    this->runAction(cocos2d::Sequence::create(
        cocos2d::TintTo::create(0.1f, 255, 0, 0),
        cocos2d::TintTo::create(0.1f, 255, 255, 255),
        nullptr
    ));

    if (m_health <= 0) {
        die();
    }
}

void Enemy::die() {
    m_aiState = EnemyState::DEAD;
    this->stopAllActions(); // 停止所有AI和动画

    // [中文注释]: 切换到击败状态的图片
    try {
        // 子类会重写具体逻辑，这里做通用处理
        this->setColor(cocos2d::Color3B::GRAY);
        this->runAction(cocos2d::Sequence::create(
            cocos2d::FadeOut::create(1.5f), // 尸体停留一会淡出
            cocos2d::RemoveSelf::create(),
            nullptr
        ));
    }
    catch (...) {}
}

void Enemy::setState(EnemyState newState) { m_aiState = newState; }
void Enemy::updateHurtbox() {}
void Enemy::attack() {}