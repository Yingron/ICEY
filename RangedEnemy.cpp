#include "RangedEnemy.h"
#include "GameUtils.h"
#include <stdexcept> // [新增]

bool RangedEnemy::init()
{
    if (!Enemy::init()) return false;

    std::string name = this->getName();
    std::string imagePath = "";

    if (name == "NormalEnemyB" || name.empty()) {
        imagePath = "NormalEnemyB.png";
    }
    else if (name == "NormalEnemyC") {
        imagePath = "NormalEnemyC.png";
    }

    // ============================================================
    // [异常处理模块 - 本体加载]
    // ============================================================
    try {
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);

        if (!texture) {
            throw std::runtime_error("Ranged Enemy Texture Missing: " + imagePath);
        }

        this->setTexture(texture);
        this->setTextureRect(cocos2d::Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    }
    catch (const std::exception& e) {
        GameUtils::log(std::string("[资源警告]: ") + e.what());
        // 后备方案
        this->setTextureRect(cocos2d::Rect(0, 0, 40, 60));
        if (name == "NormalEnemyC") this->setColor(cocos2d::Color3B::CYAN);
        else this->setColor(cocos2d::Color3B::BLUE);
    }
    // ============================================================

    m_type = EnemyType::RANGED;
    m_health = 60.0f;
    m_damage = 12.0f;
    m_speed = (name == "NormalEnemyC") ? 180.0f : 120.0f;
    m_attackRange = 500.0f;
    m_attackCooldown = 2.5f;

    return true;
}

void RangedEnemy::updateAI(float delta)
{
    // 在 update 中使用 try-catch 防止每帧崩溃，但要注意性能开销
    try {
        if (!m_targetPlayer || m_aiState == EnemyState::DEAD) return;

        bool isFlying = (getName() == "NormalEnemyC");

        if (isFlying) {
            cocos2d::Vec2 targetPos = m_targetPlayer->getPosition() + cocos2d::Vec2(0, 200);
            cocos2d::Vec2 newPos = GameUtils::lerp(this->getPosition(), targetPos, delta * 2.0f);
            this->setPosition(newPos);

            bool faceRight = (m_targetPlayer->getPositionX() > this->getPositionX());
            this->setFlippedX(!faceRight);

            if (this->getPosition().distance(m_targetPlayer->getPosition()) < m_attackRange) {
                if (m_attackTimer <= 0.0f) setState(EnemyState::ATTACK);
            }
        }
        else {
            // 陆地逻辑
            Enemy::updateAI(delta);
        }

        if (m_aiState == EnemyState::ATTACK) attack();
    }
    catch (const std::exception& e) {
        GameUtils::log(std::string("AI Update Error: ") + e.what());
    }
}

void RangedEnemy::attack()
{
    if (m_aiState == EnemyState::DEAD) return;
    m_attackTimer = m_attackCooldown;

    auto seq = cocos2d::Sequence::create(
        cocos2d::TintTo::create(0.2f, 50, 50, 255),
        cocos2d::DelayTime::create(0.5f),
        cocos2d::CallFunc::create([this]() {
            // 发射子弹也是危险操作（涉及资源加载）
            fireProjectile();
            this->setColor(cocos2d::Color3B::WHITE);
            }),
        cocos2d::DelayTime::create(0.5f),
        cocos2d::CallFunc::create([this]() {
            if (m_aiState != EnemyState::DEAD) setState(EnemyState::IDLE);
            }),
        nullptr
    );
    this->runAction(seq);
}

void RangedEnemy::fireProjectile()
{
    if (!m_targetPlayer) return;

    std::string bulletImage = "projectile.png";

    // ============================================================
    // [异常处理模块 - 子弹资源]
    // ============================================================
    cocos2d::Sprite* projectile = nullptr;

    try {
        projectile = cocos2d::Sprite::create(bulletImage);
        // Sprite::create 失败会返回 nullptr
        if (!projectile) {
            throw std::runtime_error("Bullet texture failed to load.");
        }
    }
    catch (const std::exception& e) {
        // 捕获异常，绘制备用子弹
        GameUtils::log(std::string("[战斗错误]: ") + e.what() + " Switching to default shape.");
        projectile = cocos2d::Sprite::create();
        projectile->setTextureRect(cocos2d::Rect(0, 0, 10, 10));
        projectile->setColor(cocos2d::Color3B::YELLOW);
    }
    // ============================================================

    // 下面的逻辑必须保证 projectile 非空（catch 块确保了这一点）
    projectile->setPosition(this->getPosition());
    this->getParent()->addChild(projectile, 10);

    cocos2d::Vec2 targetPos = m_targetPlayer->getPosition();
    cocos2d::Vec2 dir = targetPos - this->getPosition();
    dir.normalize();

    float speed = 500.0f;
    float dist = 1000.0f;

    auto move = cocos2d::MoveTo::create(dist / speed, this->getPosition() + dir * dist);
    auto remove = cocos2d::RemoveSelf::create();
    projectile->runAction(cocos2d::Sequence::create(move, remove, nullptr));