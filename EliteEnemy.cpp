#include "EliteEnemy.h"
#include "GameUtils.h"
#include "ResourceManager.h"
#include "EnemyType.h"
#include <stdexcept>
#include <cmath>

extern cocos2d::Animation* getOrCreateAnimation(const std::string& folder, const std::string& prefix, int frameCount, float delay);

bool EliteEnemy::init()
{
    if (!Enemy::init()) return false;

    std::string name = this->getName();
    std::string folder = "cxk";

    if (name == "CaiXunKun") folder = "cxk";
    else if (name == "NaiLong") folder = "nl";
    else if (name == "MaoDie") folder = "md";
    else if (name == "KuMingYuanYang") folder = "kmyy"; // 苦命鸳鸯

    // [异常处理]: 初始图
    try {
        std::string path = "images/characters/enemies/" + folder + "/walk1.png";
        auto tex = ResourceManager::getInstance()->getTexture(path);
        if (!tex) tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(path);

        if (!tex) throw std::runtime_error("BOSS Img Missing: " + path);
        this->setTexture(tex);
        this->setTextureRect(cocos2d::Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    }
    catch (const std::exception& e) {
        this->setTextureRect(cocos2d::Rect(0, 0, 100, 100));
        this->setColor(cocos2d::Color3B::MAGENTA);
    }

    m_type = EnemyType::ELITE;
    m_maxHealth = 500.0f;
    m_currentPhase = 1;
    m_isEnraged = false;
    m_attackRange = 60.0f;

    return true;
}

void EliteEnemy::updateAI(float delta)
{
    try {
        if (m_aiState == EnemyState::DEAD) return;

        // --------------------------------------------------------
        // [BOSS特殊逻辑]: 苦命鸳鸯 - 飞天阶段 (Phase 2)
        // --------------------------------------------------------
        if (getName() == "KuMingYuanYang" && m_currentPhase == 2) {
            static float flyTime = 0.0f; flyTime += delta;
            float moveX = std::sin(flyTime * 0.5f) * 250.0f;
            float moveY = 350.0f + std::cos(flyTime * 1.0f) * 30.0f;

            if (m_targetPlayer) {
                cocos2d::Vec2 targetSkyPos = m_targetPlayer->getPosition() + cocos2d::Vec2(moveX, moveY);
                this->setPosition(GameUtils::lerp(this->getPosition(), targetSkyPos, delta * 1.5f));
            }
            this->setRotation(this->getRotation() + 360.0f * delta);

            m_attackTimer -= delta;
            if (m_attackTimer <= 0.0f) {
                useSpecialAttack(3); // 空中远程打击
                m_attackTimer = 1.2f;
            }
            return;
        }

        Enemy::updateAI(delta);

        // --------------------------------------------------------
        // [视觉效果]: 行走动画 (8帧)
        // --------------------------------------------------------
        if (m_aiState == EnemyState::CHASE && !this->getActionByTag(99)) {
            std::string folder = "cxk";
            if (getName() == "NaiLong") folder = "nl";
            if (getName() == "MaoDie") folder = "md";
            if (getName() == "KuMingYuanYang") folder = "kmyy";

            auto anim = getOrCreateAnimation(folder, "walk", 8, 0.15f);
            if (anim) {
                auto repeat = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
                repeat->setTag(99);
                this->runAction(repeat);
            }
        }
        else if (m_aiState != EnemyState::CHASE) {
            this->stopActionByTag(99);
        }

    }
    catch (const std::exception& e) {
        GameUtils::log(std::string("EliteAI Error: ") + e.what());
    }
}

void EliteEnemy::attack()
{
    int skillIndex = 0;
    std::string name = getName();

    if (name == "KuMingYuanYang") {
        int r = GameUtils::randomInt(0, 100);
        if (r < 40) skillIndex = 0;
        else if (r < 80) skillIndex = 1;
        else skillIndex = 2;
    }
    else {
        skillIndex = GameUtils::randomInt(0, 1);
    }
    useSpecialAttack(skillIndex);
}

void EliteEnemy::useSpecialAttack(int index)
{
    if (m_aiState == EnemyState::DEAD) return;
    setState(EnemyState::ATTACK);
    m_attackTimer = 2.5f;

    std::string name = getName();
    std::string folder = "cxk";
    if (name == "NaiLong") folder = "nl";
    if (name == "MaoDie") folder = "md";
    if (name == "KuMingYuanYang") folder = "kmyy";

    try {
        // ==========================================================
        // 1. [BOSS: 菜需捆] (CaiXunKun)
        // ==========================================================
        if (name == "CaiXunKun") {
            if (index == 0) {
                // --- 技能1: 近战冲撞 (jz系列) ---
                GameUtils::log("菜需捆: 冲撞!");
                auto anim = getOrCreateAnimation(folder, "jz", 6, 0.1f);

                auto seq = cocos2d::Sequence::create(
                    cocos2d::CallFunc::create([this, anim]() {
                        if (anim) this->runAction(cocos2d::Animate::create(anim));
                        }),
                    cocos2d::DelayTime::create(0.3f), // 前摇
                    cocos2d::MoveBy::create(0.3f, cocos2d::Vec2(isFlippedX() ? -300 : 300, 0)), // 位移
                    cocos2d::CallFunc::create([this]() {
                        // [判定]
                        if (m_targetPlayer && this->getPosition().distance(m_targetPlayer->getPosition()) < 60.0f) {
                            GameUtils::log("菜需捆 冲撞命中!");
                            // damage...
                        }
                        }),
                    nullptr
                );
                this->runAction(seq);
            }
            else {
                // --- 技能2: 远战投掷 (yz系列) ---
                GameUtils::log("菜需捆: 投掷!");
                auto anim = getOrCreateAnimation(folder, "yz", 6, 0.1f);

                this->runAction(cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::CallFunc::create([this]() {
                        auto ball = cocos2d::Sprite::create("images/characters/enemies/Projectile_Basketball.png");
                        if (!ball) ball = cocos2d::Sprite::create();
                        ball->setPosition(this->getPosition());
                        this->getParent()->addChild(ball, 10);
                        auto move = cocos2d::MoveTo::create(0.8f, m_targetPlayer->getPosition());
                        ball->runAction(cocos2d::Sequence::create(move, cocos2d::RemoveSelf::create(), nullptr));
                        }),
                    nullptr
                ));
            }
        }
        // ==========================================================
        // 2. [BOSS: 奶龙] (NaiLong)
        // ==========================================================
        else if (name == "NaiLong") {
            if (index == 0) {
                // --- 技能1: 近战抬举 (jz系列) ---
                GameUtils::log("奶龙: 抬举!");
                auto anim = getOrCreateAnimation(folder, "jz", 6, 0.1f);

                auto seq = cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::CallFunc::create([this]() {
                        // [判定]
                        if (m_targetPlayer && this->getPosition().distance(m_targetPlayer->getPosition()) < 60.0f) {
                            GameUtils::log("奶龙 抬举命中!");
                            // JumpBy...
                        }
                        }),
                    nullptr
                );
                this->runAction(seq);
            }
            else {
                // --- 技能2: 远战音波 (yz系列) ---
                GameUtils::log("奶龙: 音波!");
                auto anim = getOrCreateAnimation(folder, "yz", 6, 0.1f);

                this->runAction(cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::CallFunc::create([this]() {
                        auto wave = cocos2d::Sprite::create("images/characters/enemies/Effect_SonicWave.png");
                        if (!wave) wave = cocos2d::Sprite::create();
                        wave->setPosition(this->getPosition());
                        this->addChild(wave);
                        wave->runAction(cocos2d::Spawn::create(cocos2d::ScaleTo::create(0.6f, 8.0f), cocos2d::FadeOut::create(0.6f), nullptr));
                        }),
                    nullptr
                ));
            }
        }
        // ==========================================================
        // 3. [BOSS: 耄耋] (MaoDie)
        // ==========================================================
        else if (name == "MaoDie") {
            if (index == 0) {
                // --- 技能1: 抬举/冲刺 (jz系列) ---
                GameUtils::log("耄耋: 冲刺!");
                auto anim = getOrCreateAnimation(folder, "jz", 6, 0.1f);

                auto seq = cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::MoveBy::create(0.2f, cocos2d::Vec2(isFlippedX() ? -200 : 200, 0)),
                    cocos2d::CallFunc::create([this]() {
                        if (m_targetPlayer && this->getPosition().distance(m_targetPlayer->getPosition()) < 60.0f) {
                            GameUtils::log("耄耋 冲刺命中!");
                        }
                        }),
                    nullptr
                );
                this->runAction(seq);
            }
            else {
                // --- 技能2: 远战音波 (yz系列) ---
                GameUtils::log("耄耋: 音波!");
                auto anim = getOrCreateAnimation(folder, "yz", 6, 0.1f);

                this->runAction(cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::CallFunc::create([this]() {
                        GameUtils::log("耄耋 音波释放");
                        }),
                    nullptr
                ));
            }
        }
        // ==========================================================
        // 4. [BOSS: 苦命鸳鸯] (KuMingYuanYang)
        // ==========================================================
        else if (name == "KuMingYuanYang") {
            if (index == 0) {
                // 技能1: 旋转 (jz系列)
                auto anim = getOrCreateAnimation(folder, "jz", 6, 0.1f);
                this->runAction(cocos2d::Spawn::create(
                    cocos2d::RotateBy::create(1.0f, 720.0f),
                    cocos2d::Animate::create(anim), nullptr
                ));
            }
            else if (index == 1) {
                // 技能2: 念力弹 (yz系列)
                auto anim = getOrCreateAnimation(folder, "yz", 6, 0.1f);
                this->runAction(cocos2d::Sequence::create(
                    cocos2d::Animate::create(anim),
                    cocos2d::CallFunc::create([this]() { /* 发射 bullet */ }), nullptr
                ));
            }
            else if (index == 2) {
                // 技能3: 飞天
                m_currentPhase = 2;
                GameUtils::log("苦命鸳鸯: 升空!");
                // 升空动作
                this->runAction(cocos2d::MoveBy::create(1.5f, cocos2d::Vec2(0, 300)));
            }
            else if (index == 3) {
                // 技能3-2: 空对地 (空中触发)
                auto bullet = cocos2d::Sprite::create("images/characters/enemies/Magic_Bullet.png");
                if (!bullet) bullet = cocos2d::Sprite::create();
                bullet->setPosition(this->getPosition());
                this->getParent()->addChild(bullet, 10);
                auto move = cocos2d::MoveTo::create(0.8f, m_targetPlayer->getPosition());
                bullet->runAction(cocos2d::Sequence::create(move, cocos2d::RemoveSelf::create(), nullptr));
            }
        }
    }
    catch (...) {}
}

void EliteEnemy::die()
{
    this->stopAllActions();
    m_aiState = EnemyState::DEAD;
    std::string name = getName();
    std::string folder = "cxk";
    if (name == "NaiLong") folder = "nl";
    if (name == "MaoDie") folder = "md";
    if (name == "KuMingYuanYang") folder = "kmyy";

    // [中文注释]: 播放死亡动画 (hit系列 5帧)
    auto anim = getOrCreateAnimation(folder, "hit", 5, 0.2f);

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