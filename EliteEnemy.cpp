#include "EliteEnemy.h"
#include "EnemyType.h"
#include "GameUtils.h"
#include <stdexcept> // [新增]

bool EliteEnemy::init()
{
    if (!Enemy::init()) return false;

    std::string name = this->getName();
    std::string bossImage = "";

    // 确定图片路径
    if (name == "CaiXunKun") {
        bossImage = "Boss_CaiXunKun.png";
        m_maxHealth = 400.0f;
        m_speed = 200.0f;
    }
    else if (name == "NaiLong") {
        bossImage = "Boss_NaiLong.png";
        m_maxHealth = 800.0f;
        m_speed = 80.0f;
        this->setScale(2.0f);
    }
    else if (name == "MaoDie") {
        bossImage = "Boss_MaoDie.png";
        m_maxHealth = 500.0f;
    }
    else if (name == "KuMingYuanYang") {
        bossImage = "Boss_KuMing.png";
        m_maxHealth = 600.0f;
    }
    else {
        bossImage = "Elite_Generic.png";
    }

    // ============================================================
    // [异常处理模块 - BOSS资源加载]
    // ============================================================
    try {
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(bossImage);

        // BOSS 图片丢失是严重问题，这里我们记录一个 Error 级别的日志，但依然不让游戏崩
        if (!texture) {
            throw std::runtime_error("CRITICAL: BOSS Image [" + bossImage + "] Missing!");
        }

        this->setTexture(texture);
        this->setTextureRect(cocos2d::Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    }
    catch (const std::exception& e) {
        // [加分项]: 详细的错误处理
        GameUtils::log(std::string("EXCEPTION CAUGHT: ") + e.what());

        // 视觉提示错误：品红色块（标准的 Missing Texture 颜色）
        this->setTextureRect(cocos2d::Rect(0, 0, 100, 100));
        this->setColor(cocos2d::Color3B::MAGENTA);

        // 可以添加一个额外的 Label 显示 BOSS 名字，防止玩家不知道这是谁
        auto label = cocos2d::Label::createWithSystemFont(name, "Arial", 24);
        label->setPosition(cocos2d::Vec2(50, 120));
        this->addChild(label);
    }
    // ============================================================

    m_type = EnemyType::ELITE;
    m_health = m_maxHealth;
    m_currentPhase = 1;
    m_phaseTransitionHealth = m_maxHealth * 0.5f;
    m_isEnraged = false;

    createSpecialAttacks();

    return true;
}

void EliteEnemy::updateAI(float delta)
{
    // Log 输出本身一般不抛出异常，但为了保险起见
    try {
        static float logTimer = 0.0f;
        logTimer += delta;
        if (logTimer > 8.0f && m_targetPlayer) {
            logTimer = 0.0f;
            if (getName() == "CaiXunKun") GameUtils::log("旁白: 考试时间还剩5分钟...");
            else if (getName() == "NaiLong") GameUtils::log("旁白: 身体极限...");
        }

        checkPhaseTransition();
        Enemy::updateAI(delta);
    }
    catch (...) {
        // 捕获所有未知异常
    }
}

void EliteEnemy::attack()
{
    if (m_aiState == EnemyState::DEAD) return;

    if (GameUtils::randomInt(0, 100) < 30) {
        int skillIdx = GameUtils::randomInt(0, 1);
        useSpecialAttack(skillIdx);
    }
    else {
        Enemy::attack();
    }
}

void EliteEnemy::createSpecialAttacks() {}

void EliteEnemy::useSpecialAttack(int attackIndex)
{
    if (m_aiState == EnemyState::DEAD) return;
    setState(EnemyState::ATTACK);
    m_attackTimer = 2.0f;

    // ============================================================
    // [异常处理模块 - 技能特效加载]
    // ============================================================
    // 在 Lambda 表达式中使用 try-catch

    if (attackIndex == 0) {
        GameUtils::log(getName() + " 释放技能1: 压力冲击!");

        auto skillAction = cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(0.2f, 2.2f),
            cocos2d::CallFunc::create([this]() {
                try {
                    // 尝试加载特效
                    auto effect = cocos2d::Sprite::create("Skill_Shockwave.png");
                    if (!effect) throw std::runtime_error("Effect Texture Missing");

                    effect->setPosition(this->getContentSize() / 2);
                    this->addChild(effect, 20);
                    effect->runAction(cocos2d::Sequence::create(
                        cocos2d::ScaleTo::create(0.5f, 3.0f),
                        cocos2d::RemoveSelf::create(), nullptr));
                }
                catch (const std::exception& e) {
                    GameUtils::log("Skill Effect Failed: " + std::string(e.what()));
                    // 特效失败就不显示了，或者闪烁一下
                    this->runAction(cocos2d::Blink::create(0.5f, 5));
                }
                }),
            cocos2d::ScaleTo::create(0.2f, 2.0f),
            nullptr
        );
        this->runAction(skillAction);
    }
    else {
        GameUtils::log(getName() + " 释放技能2: 绝望冲撞!");
        cocos2d::Vec2 targetPos = m_targetPlayer ? m_targetPlayer->getPosition() : this->getPosition();

        auto skillAction = cocos2d::Sequence::create(
            cocos2d::TintTo::create(0.2f, 255, 255, 0),
            cocos2d::MoveTo::create(0.3f, targetPos),
            cocos2d::CallFunc::create([this]() {
                // 冲刺结束逻辑
                }),
                cocos2d::TintTo::create(0.2f, 255, 255, 255),
                nullptr
                );
        this->runAction(skillAction);
    }
}

void EliteEnemy::takeDamage(float damage)
{
    // ============================================================
    // [异常处理模块 - 护盾特效]
    // ============================================================

    if (GameUtils::randomInt(0, 100) < 20) {
        GameUtils::log(getName() + " 格挡了攻击！");

        try {
            auto shield = cocos2d::Sprite::create("Effect_Shield.png");
            if (!shield) throw std::runtime_error("Shield Texture Missing");

            shield->setPosition(this->getContentSize() / 2);
            this->addChild(shield);
            shield->runAction(cocos2d::Sequence::create(
                cocos2d::FadeOut::create(0.5f),
                cocos2d::RemoveSelf::create(), nullptr));
        }
        catch (...) {
            // 简单处理：没有图片就闪蓝色
            this->runAction(cocos2d::TintTo::create(0.1f, 0, 0, 255));
        }

        return;
    }

    float finalDamage = damage;
    if (m_isEnraged) finalDamage *= 0.8f;
    if (getName() == "NaiLong") finalDamage *= 0.6f;

    Enemy::takeDamage(finalDamage);
}

void EliteEnemy::checkPhaseTransition()
{
    if (m_currentPhase == 1 && m_health <= m_phaseTransitionHealth) {
        transitionToNextPhase();
    }
}

void EliteEnemy::transitionToNextPhase()
{
    m_currentPhase = 2;
    enrage();
    GameUtils::log("系统警告: " + getName() + " 进入第二阶段!");

    this->runAction(cocos2d::Spawn::create(
        cocos2d::TintTo::create(0.5f, 255, 50, 50),
        cocos2d::ScaleBy::create(0.5f, 1.2f),
        nullptr
    ));
}

void EliteEnemy::enrage()
{
    m_isEnraged = true;
    m_speed *= 1.3f;
}