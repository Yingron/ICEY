#include "RangedEnemy.h"
#include "GameUtils.h"
#include "ResourceManager.h"
#include <stdexcept>
#include <cmath>

extern cocos2d::Animation* getOrCreateAnimation(const std::string& folder, const std::string& prefix, int frameCount, float delay);

bool RangedEnemy::init()
{
    if (!Enemy::init()) return false;

    std::string name = this->getName();
    std::string folder = "jz2"; // 默认陆地

    if (name == "NormalEnemyC") folder = "yz"; // 飞行

    // [异常处理]: 初始图
    try {
        std::string path = "images/characters/enemies/" + folder + "/walk_1.png";
        auto tex = ResourceManager::getInstance()->getTexture(path);
        if (!tex) tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(path);
        if (!tex) throw std::runtime_error("Ranged Img Missing");

        this->setTexture(tex);
        this->setTextureRect(cocos2d::Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    }
    catch (...) {
        this->setTextureRect(cocos2d::Rect(0, 0, 40, 60));
        this->setColor(cocos2d::Color3B::BLUE);
    }

    m_speed = (name == "NormalEnemyC") ? 180.0f : 120.0f;
    m_attackRange = 500.0f;
    return true;
}

void RangedEnemy::updateAI(float delta)
{
    try {
        if (!m_targetPlayer || m_aiState == EnemyState::DEAD) return;

        bool isFlying = (getName() == "NormalEnemyC");
        std::string folder = isFlying ? "yz" : "jz2";

        // [中文注释]: 视觉效果 (8帧循环)
        if (m_aiState == EnemyState::CHASE && !this->getActionByTag(99)) {
            auto anim = getOrCreateAnimation(folder, "walk", 8, 0.1f);
            if (anim) {
                auto repeat = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
                repeat->setTag(99);
                this->runAction(repeat);
            }
        }

        if (isFlying) {
            // [AI]: 悬停
            static float time = 0.0f; time += delta;
            float offsetY = std::sin(time * 3.0f) * 20.0f;
            cocos2d::Vec2 target = m_targetPlayer->getPosition() + cocos2d::Vec2(0, 250 + offsetY);
            this->setPosition(GameUtils::lerp(this->getPosition(), target, delta * 2.0f));

            bool faceRight = m_targetPlayer->getPositionX() > this->getPositionX();
            this->setFlippedX(!faceRight);

            if (this->getPosition().distance(m_targetPlayer->getPosition()) < m_attackRange) {
                if (m_attackTimer <= 0.0f) setState(EnemyState::ATTACK);
            }
        }
        else {
            // [AI]: 陆地风筝
            float dist = this->getPosition().distance(m_targetPlayer->getPosition());
            if (dist < 200.0f) {
                float dir = (this->getPositionX() > m_targetPlayer->getPositionX()) ? 1.0f : -1.0f;
                this->setPositionX(this->getPositionX() + dir * m_speed * delta);
            }
            else if (dist > m_attackRange) {
                Enemy::chasePlayer(delta);
            }
            else {
                if (m_attackTimer <= 0.0f) setState(EnemyState::ATTACK);
            }
        }

        if (m_aiState == EnemyState::ATTACK) attack();

    }
    catch (...) {}
}

void RangedEnemy::attack()
{
    if (m_aiState == EnemyState::DEAD) return;
    m_attackTimer = 2.5f;
    this->stopActionByTag(99);

    std::string folder = (getName() == "NormalEnemyC") ? "yz" : "jz2";
    // [图片位置]: gj_1 ~ gj_6
    auto anim = getOrCreateAnimation(folder, "gj", 6, 0.1f);

    auto seq = cocos2d::Sequence::create(
        cocos2d::CallFunc::create([this, anim]() {
            if (anim) this->runAction(cocos2d::Animate::create(anim));
            else this->setColor(cocos2d::Color3B::BLUE);
            }),
        cocos2d::DelayTime::create(0.3f), // 发射帧

        cocos2d::CallFunc::create([this]() {
            fireProjectile();
            }),
        cocos2d::DelayTime::create(0.3f),

        cocos2d::CallFunc::create([this]() {
            this->setColor(cocos2d::Color3B::WHITE);
            if (m_aiState != EnemyState::DEAD) setState(EnemyState::IDLE);
            }),
        nullptr
    );
    this->runAction(seq);
}

void RangedEnemy::fireProjectile()
{
    if (!m_targetPlayer) return;

    // [中文注释]: 发射子弹
    cocos2d::Sprite* bullet = nullptr;
    try {
        bullet = cocos2d::Sprite::create("images/characters/enemies/projectile.png");
        if (!bullet) throw std::runtime_error("Bullet missing");
    }
    catch (...) {
        bullet = cocos2d::Sprite::create();
        bullet->setTextureRect(cocos2d::Rect(0, 0, 10, 10));
        bullet->setColor(cocos2d::Color3B::YELLOW);
    }

    bullet->setPosition(this->getPosition());
    this->getParent()->addChild(bullet, 10);

    cocos2d::Vec2 dir = m_targetPlayer->getPosition() - this->getPosition();
    dir.normalize();
    float speed = (getName() == "NormalEnemyC") ? 600.0f : 400.0f;
    float range = 1000.0f;

    auto move = cocos2d::MoveTo::create(range / speed, this->getPosition() + dir * range);
    bullet->runAction(cocos2d::Sequence::create(move, cocos2d::RemoveSelf::create(), nullptr));
}

void RangedEnemy::die()
{
    // yz 没有死亡动画
    if (getName() == "NormalEnemyC") {
        Enemy::die();
        return;
    }

    this->stopAllActions();
    m_aiState = EnemyState::DEAD;
    // jz2 有 hit 动画
    auto anim = getOrCreateAnimation("jz2", "hit", 5, 0.15f);
    if (anim) {
        this->runAction(cocos2d::Sequence::create(
            cocos2d::Animate::create(anim),
            cocos2d::RemoveSelf::create(), nullptr
        ));
    }
    else {
        Enemy::die();
    }
}