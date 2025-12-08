#include "MeleeEnemy.h"
#include "GameUtils.h"
#include "Hitbox.h" // 假设存在
#include <stdexcept> // [新增] 引入标准异常库

bool MeleeEnemy::init()
{
    // 基础初始化失败直接返回，无需异常处理
    if (!Enemy::init()) return false;

    // 基础数值
    float baseHealth = 80.0f;
    float baseDamage = 15.0f;

    // 获取当前对象的名称
    std::string name = this->getName();
    std::string imagePath = "";

    // 1. 判定图片路径
    if (name == "NormalEnemyA" || name.empty()) {
        imagePath = "NormalEnemyA.png";
        m_speed = 150.0f;
        m_attackCooldown = 1.5f;
    }
    else if (name == "EliteMonsterA") {
        imagePath = "EliteMonsterA.png";
        baseHealth = 150.0f;
        baseDamage = 25.0f;
        m_speed = 100.0f;
        m_attackCooldown = 2.0f;
        this->setScale(1.2f);
    }
    else if (name == "EliteMonsterB") {
        imagePath = "EliteMonsterB.png";
        baseHealth = 60.0f;
        baseDamage = 10.0f;
        m_speed = 250.0f;
        m_attackCooldown = 0.8f;
        this->setColor(cocos2d::Color3B(200, 200, 255));
    }

    // ============================================================
    // [异常处理模块 - 图片加载]
    // ============================================================
    try {
        // [Try 块]: 尝试执行危险操作（资源加载）
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);

        // 如果纹理为空，我们主动抛出一个运行时错误，强制进入 catch 流程
        if (!texture) {
            throw std::runtime_error("Resource Load Failed: " + imagePath);
        }

        // 如果成功，正常设置
        this->setTexture(texture);
        this->setTextureRect(cocos2d::Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
        this->setColor(cocos2d::Color3B::WHITE);
    }
    catch (const std::exception& e) {
        // [Catch 块]: 捕获异常，进行“优雅降级”处理，保证游戏不崩
        GameUtils::log(std::string("[系统异常捕获]: ") + e.what());
        GameUtils::log("启动后备渲染方案 (Fallback Rendering)...");

        // 后备方案：绘制色块
        this->setTextureRect(cocos2d::Rect(0, 0, 50, 80));
        if (name == "EliteMonsterA") this->setColor(cocos2d::Color3B::GREEN);
        else if (name == "EliteMonsterB") this->setColor(cocos2d::Color3B::ORANGE);
        else this->setColor(cocos2d::Color3B::RED);
    }
    // ============================================================

    // 应用数值
    float diffMult = 1.0f;
    m_maxHealth = baseHealth * diffMult;
    m_health = m_maxHealth;
    m_damage = baseDamage * diffMult;

    return true;
}

void MeleeEnemy::updateAI(float delta)
{
    if (getName() == "EliteMonsterB") {
        m_speed = 280.0f;
    }

    // 这里也可以加 try-catch，防止 AI 逻辑中的空指针
    try {
        Enemy::updateAI(delta);
    }
    catch (...) {
        GameUtils::log("Critical Error in AI Update!");
    }
}

void MeleeEnemy::attack()
{
    if (m_aiState == EnemyState::DEAD) return;

    m_attackTimer = m_attackCooldown;

    auto attackSeq = cocos2d::Sequence::create(
        cocos2d::TintTo::create(0.2f, 255, 100, 100),
        cocos2d::DelayTime::create(0.3f),
        cocos2d::CallFunc::create([this]() {
            // [异常处理模块 - 攻击判定]
            try {
                auto hitbox = this->createMeleeHitbox();
                if (!hitbox) throw std::logic_error("Hitbox creation returned null"); // 检查逻辑错误

                if (m_targetPlayer && this->getPosition().distance(m_targetPlayer->getPosition()) < 60.0f) {
                    GameUtils::log(this->getName() + " 击中了玩家！");
                    // m_targetPlayer->takeDamage(m_damage);
                }
            }
            catch (const std::exception& e) {
                GameUtils::log(std::string("Attack Logic Error: ") + e.what());
            }
            this->setColor(cocos2d::Color3B::WHITE);
            }),
        nullptr
    );

    this->runAction(attackSeq);
}

std::shared_ptr<Hitbox> MeleeEnemy::createMeleeHitbox()
{
    return std::make_shared<Hitbox>();
}