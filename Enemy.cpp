#include "Enemy.h"
#include "GameUtils.h"

// 假设 Hitbox 和 Player 的基本实现在项目中已存在
// 我们基于头文件接口进行逻辑编写

bool Enemy::init()
{
    if (!cocos2d::Sprite::init()) return false;

    // 默认属性初始化 (会被子类覆盖)
    m_health = 100.0f;
    m_maxHealth = 100.0f;
    m_damage = 10.0f;
    m_speed = 100.0f;
    m_attackRange = 50.0f;
    m_detectionRange = 300.0f;
    m_attackCooldown = 2.0f;
    m_attackTimer = 0.0f;

    m_aiState = EnemyState::IDLE;
    m_type = EnemyType::MELEE;
    m_targetPlayer = nullptr;

    this->scheduleUpdate();
    return true;
}

void Enemy::update(float delta)
{
    if (m_aiState == EnemyState::DEAD) return;

    // 更新攻击冷却
    if (m_attackTimer > 0.0f) m_attackTimer -= delta;

    // 执行 AI 决策
    updateAI(delta);

    // 更新受击框位置
    updateHurtbox();
}

void Enemy::updateAI(float delta)
{
    // [AI 状态机核心逻辑]
    if (!m_targetPlayer) {
        // 如果没有目标，尝试寻找玩家（简单实现：假设场景管理器会设置目标）
        // 或者处于待机状态
        return;
    }

    float distToPlayer = this->getPosition().distance(m_targetPlayer->getPosition());

    switch (m_aiState)
    {
    case EnemyState::IDLE:
        // 索敌逻辑
        if (distToPlayer <= m_detectionRange) {
            setState(EnemyState::CHASE);
        }
        else {
            // 简单的随机巡逻切换
            if (GameUtils::randomInt(0, 100) < 1) setState(EnemyState::PATROL);
        }
        break;

    case EnemyState::PATROL:
        patrol(delta);
        if (distToPlayer <= m_detectionRange) {
            setState(EnemyState::CHASE);
        }
        else if (GameUtils::randomInt(0, 200) < 1) {
            setState(EnemyState::IDLE);
        }
        break;

    case EnemyState::CHASE:
        if (distToPlayer <= m_attackRange) {
            // 进入攻击范围
            if (m_attackTimer <= 0.0f) {
                setState(EnemyState::ATTACK);
            }
            // 否则等待冷却，保持朝向
            bool faceRight = (m_targetPlayer->getPositionX() > this->getPositionX());
            this->setFlippedX(!faceRight);
        }
        else if (distToPlayer > m_detectionRange * 1.5f) {
            // 玩家跑太远，放弃追击
            setState(EnemyState::IDLE);
        }
        else {
            chasePlayer(delta);
        }
        break;

    case EnemyState::ATTACK:
        attack(); // 执行攻击逻辑（通常包含动作序列）
        break;

    case EnemyState::HURT:
        // 受击状态通常由 takeDamage 触发，由动画回调结束
        break;
    }
}

void Enemy::patrol(float delta)
{
    // 简单的左右徘徊逻辑
    static float moveDir = 1.0f;
    // 边界检查逻辑通常依赖 Level 类，这里简化为定时反向
    if (GameUtils::randomInt(0, 100) < 2) moveDir *= -1.0f;

    float newX = getPositionX() + moveDir * m_speed * 0.5f * delta;
    this->setPositionX(newX);
    this->setFlippedX(moveDir < 0);
}

void Enemy::chasePlayer(float delta)
{
    if (!m_targetPlayer) return;

    cocos2d::Vec2 myPos = this->getPosition();
    cocos2d::Vec2 targetPos = m_targetPlayer->getPosition();

    // 只在 X 轴移动 (2D 横版)
    float dirX = (targetPos.x > myPos.x) ? 1.0f : -1.0f;
    float newX = myPos.x + dirX * m_speed * delta;

    this->setPositionX(newX);
    this->setFlippedX(dirX < 0);
}

void Enemy::attack()
{
    // 基类攻击逻辑，具体由子类重写
    m_attackTimer = m_attackCooldown;
    // 默认行为：稍微停顿后切回 CHASE
    this->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.5f),
        cocos2d::CallFunc::create([this]() {
            if (m_aiState != EnemyState::DEAD) setState(EnemyState::CHASE);
            }),
        nullptr
    ));
}

void Enemy::takeDamage(float damage)
{
    if (m_aiState == EnemyState::DEAD) return;

    m_health -= damage;
    setState(EnemyState::HURT);

    // 视觉反馈：变红
    this->runAction(cocos2d::Sequence::create(
        cocos2d::TintTo::create(0.1f, 255, 0, 0),
        cocos2d::TintTo::create(0.1f, 255, 255, 255),
        cocos2d::CallFunc::create([this]() {
            if (m_health <= 0) die();
            else if (m_aiState != EnemyState::DEAD) setState(EnemyState::CHASE);
            }),
        nullptr
    ));
}

void Enemy::die()
{
    m_aiState = EnemyState::DEAD;
    this->stopAllActions();
    // 死亡动画：淡出并移除
    this->runAction(cocos2d::Sequence::create(
        cocos2d::FadeOut::create(0.5f),
        cocos2d::RemoveSelf::create(),
        nullptr
    ));
}

void Enemy::setState(EnemyState newState)
{
    if (m_aiState == newState) return;
    m_aiState = newState;
    // 可以在这里添加进入状态的初始化逻辑，如播放特定动画
}

void Enemy::updateHurtbox()
{
    // 简单的矩形 hurtbox 跟随 Sprite
    // m_hurtbox.rect = this->getBoundingBox();
}

bool Enemy::isPlayerInRange() const {
    if (!m_targetPlayer) return false;
    return getPosition().distance(m_targetPlayer->getPosition()) <= m_detectionRange;
}

bool Enemy::isPlayerInAttackRange() const {
    if (!m_targetPlayer) return false;
    return getPosition().distance(m_targetPlayer->getPosition()) <= m_attackRange;
}

std::shared_ptr<Hitbox> Enemy::createAttackHitbox() { return nullptr; }}