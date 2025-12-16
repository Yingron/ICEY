#include "MeleeEnemy.h"
#include "GameUtils.h"
#include "Hitbox.h"
#include "ResourceManager.h"
#include <stdexcept>

// 声明外部辅助函数
extern cocos2d::Animation* getOrCreateAnimation(const std::string& folder, const std::string& prefix, int frameCount, float delay);

bool MeleeEnemy::init()
{
    if (!Enemy::init()) return false;

    float baseHealth = 80.0f;
    std::string name = this->getName();
    std::string folder = "jz1"; // 默认资源

    // [中文注释]: 资源映射与属性配置
    if (name == "NormalEnemyA" || name.empty()) {
        folder = "jz1";
        m_speed = 150.0f;
        m_attackWidth = 60.0f;
        m_attackHeight = 40.0f;
    }
    else if (name == "NormalEnemyB") {
        // 这里的 NormalEnemyB 如果作为近战出现 (工厂里的特殊情况)，使用 jz2
        folder = "jz2";
        m_speed = 160.0f;
        m_attackWidth = 65.0f;
        m_attackHeight = 40.0f;
    }
    else if (name == "EliteMonsterA") {
        folder = "jingying";
        baseHealth = 150.0f;
        m_speed = 100.0f;
        m_attackWidth = 120.0f;
        m_attackHeight = 80.0f;
        this->setScale(1.3f);
    }

    // [异常处理]: 加载初始站立图 (walk_1)
    try {
        std::string path = "images/characters/enemies/" + folder + "/walk_1.png";
        auto tex = ResourceManager::getInstance()->getTexture(path);
        if (!tex) tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(path);

        if (!tex) throw std::runtime_error("Melee Texture Missing: " + path);

        this->setTexture(tex);
        this->setTextureRect(cocos2d::Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    }
    catch (...) {
        this->setTextureRect(cocos2d::Rect(0, 0, 50, 80));
        this->setColor(cocos2d::Color3B::RED);
    }

    m_maxHealth = baseHealth;
    m_health = m_maxHealth;
    return true;
}

void MeleeEnemy::updateAI(float delta)
{
    try {
        if (m_aiState == EnemyState::ATTACK || m_aiState == EnemyState::DEAD) return;

        // [中文注释]: 行走动画 (8帧)
        if (m_aiState == EnemyState::CHASE) {
            if (!this->getActionByTag(99)) {
                std::string folder = "jz1";
                if (getName() == "NormalEnemyB") folder = "jz2";
                if (getName() == "EliteMonsterA") folder = "jingying";

                auto anim = getOrCreateAnimation(folder, "walk", 8, 0.1f);
                if (anim) {
                    auto repeat = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
                    repeat->setTag(99);
                    this->runAction(repeat);
                }
            }
        }
        else {
            this->stopActionByTag(99);
        }

        Enemy::updateAI(delta);
    }
    catch (...) {}
}

void MeleeEnemy::attack()
{
    if (m_aiState == EnemyState::DEAD) return;
    m_attackTimer = m_attackCooldown;
    this->stopActionByTag(99);

    std::string folder = "jz1";
    if (getName() == "NormalEnemyB") folder = "jz2";
    if (getName() == "EliteMonsterA") folder = "jingying";

    // [中文注释]: 加载攻击动画 (gj系列 6帧)
    auto anim = getOrCreateAnimation(folder, "gj", 6, 0.1f);

    auto seq = cocos2d::Sequence::create(
        cocos2d::CallFunc::create([this, anim]() {
            if (anim) this->runAction(cocos2d::Animate::create(anim));
            else this->setColor(cocos2d::Color3B(255, 100, 100)); // Fallback
            }),
        cocos2d::DelayTime::create(0.3f), // 等待判定帧

        cocos2d::CallFunc::create([this]() {
            // [判定]: 四方位索敌
            if (m_targetPlayer) {
                cocos2d::Vec2 myPos = this->getPosition();
                cocos2d::Vec2 targetPos = m_targetPlayer->getPosition();
                float distX = std::abs(myPos.x - targetPos.x);
                float distY = std::abs(myPos.y - targetPos.y);
                bool facing = (targetPos.x > myPos.x) == !this->isFlippedX();

                if (facing && distX < m_attackWidth && distY < m_attackHeight) {
                    // m_targetPlayer->takeDamage(m_damage);
                    GameUtils::log(getName() + " 命中!");
                }
            }
            }),
        cocos2d::DelayTime::create(0.3f), // 后摇

        cocos2d::CallFunc::create([this]() {
            this->setColor(cocos2d::Color3B::WHITE);
            if (m_aiState != EnemyState::DEAD) setState(EnemyState::CHASE);
            }),
        nullptr
    );
    this->runAction(seq);
}

void MeleeEnemy::die()
{
    this->stopAllActions();
    m_aiState = EnemyState::DEAD;

    std::string folder = "jz1";
    if (getName() == "NormalEnemyB") folder = "jz2";
    if (getName() == "EliteMonsterA") folder = "jingying";

    // [中文注释]: 死亡动画 (hit系列 5帧)
    auto anim = getOrCreateAnimation(folder, "hit", 5, 0.15f);

    if (anim) {
        this->runAction(cocos2d::Sequence::create(
            cocos2d::Animate::create(anim),
            cocos2d::FadeOut::create(0.5f),
            cocos2d::RemoveSelf::create(),
            nullptr
        ));
    }
    else {
        Enemy::die();
    }
}

std::shared_ptr<Hitbox> MeleeEnemy::createMeleeHitbox() { return std::make_shared<Hitbox>(); }