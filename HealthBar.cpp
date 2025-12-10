#include "HealthBar.h"
//HealthBar::setHealth(float health)设置当前血量

using namespace cocos2d;
using namespace ui;

// 创建血条实例
HealthBar* HealthBar::create(float maxHealth)
{
    HealthBar* pRet = new (std::nothrow) HealthBar();
    if (pRet && pRet->init(maxHealth))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// 初始化血条
bool HealthBar::init(float maxHealth)
{
    if (!Node::init())
    {
        return false;
    }

    // 设置最大血量
    m_maxHealth = maxHealth;
    m_currentHealth = maxHealth;

    // 创建血条组件
    createHealthBar();

    // 初始显示
    updateDisplay();

    return true;
}

// 创建血条UI组件
void HealthBar::createHealthBar()
{
    // 血条背景（灰色）
    m_healthBarBackground = LoadingBar::create("health_bar_background.png");
    if (!m_healthBarBackground)
    {
        // 如果图片不存在，创建红色矩形作为背景
        m_healthBarBackground = LoadingBar::create();
        m_healthBarBackground->setScale9Enabled(true);
        m_healthBarBackground->setContentSize(Size(200, 20));
        m_healthBarBackground->setColor(Color3B(80, 80, 80));
    }

    // 血条前景（绿色，根据血量变化）
    m_healthBar = LoadingBar::create("health_bar.png");
    if (!m_healthBar)
    {
        // 如果图片不存在，创建绿色矩形作为血条
        m_healthBar = LoadingBar::create();
        m_healthBar->setScale9Enabled(true);
        m_healthBar->setContentSize(Size(200, 20));
        m_healthBar->setColor(Color3B(0, 255, 0));
    }

    // 血量文本显示
    m_healthText = Label::createWithTTF("", "fonts/Marker Felt.ttf", 16);
    if (!m_healthText)
    {
        m_healthText = Label::createWithSystemFont("", "Arial", 16);
    }

    // 设置血条属性
    m_healthBarBackground->setDirection(LoadingBar::Direction::LEFT);
    m_healthBarBackground->setPercent(100);

    m_healthBar->setDirection(LoadingBar::Direction::LEFT);
    m_healthBar->setPercent(100);

    // 设置文本颜色
    m_healthText->setColor(Color3B::WHITE);
    m_healthText->enableOutline(Color4B::BLACK, 2); // 添加黑色描边，提高可读性

    // 添加血条到节点
    this->addChild(m_healthBarBackground, 0);
    this->addChild(m_healthBar, 1);
    this->addChild(m_healthText, 2);

    // 设置位置（相对位置）
    m_healthBarBackground->setPosition(Vec2(0, 0));
    m_healthBar->setPosition(Vec2(0, 0));
    m_healthText->setPosition(Vec2(0, -15)); // 文本显示在血条下方
}

// 设置当前血量
void HealthBar::setHealth(float health)
{
    m_currentHealth = health;

    // 限制血量范围
    if (m_currentHealth <= 0)
    {
        m_currentHealth = 0;
    }
    else if (m_currentHealth > m_maxHealth)
    {
        m_currentHealth = m_maxHealth;
    }

    updateDisplay();
}


// 更新血条显示
void HealthBar::updateDisplay()
{
    // 计算血量百分比
    float percent = 0;
    if (m_maxHealth > 0)
    {
        percent = (m_currentHealth / m_maxHealth) * 100;
    }

    // 更新血条进度
    m_healthBar->setPercent(percent);

    // 根据血量变化颜色
    //updateColorByHealth(percent);

    // 更新文本显示
    std::string healthStr = StringUtils::format("%.0f/%.0f", m_currentHealth, m_maxHealth);
    m_healthText->setString(healthStr);
}

/*// 根据血量百分比更新血条颜色(有点丑先注释掉了)
void HealthBar::updateColorByHealth(float percent)
{
    Color3B color;

    if (percent >= 70)
    {
        // 绿色（健康）
        color = Color3B(0, 255, 0);
    }
    else if (percent >= 30)
    {
        // 黄色（中等）
        color = Color3B(255, 255, 0);
    }
    else
    {
        // 红色（危险）
        color = Color3B(255, 0, 0);
    }

    m_healthBar->setColor(color);
}*/

void HealthBar::setMaxHealth(float maxHealth)
{
    if (maxHealth <= 0) return;

    m_maxHealth = maxHealth;

    // 更新显示
    if (m_currentHealth > m_maxHealth)
    {
        m_currentHealth = m_maxHealth;
    }

    updateDisplay();
}


// 可选：添加动画效果的血量变化
void HealthBar::setHealthWithAnimation(float targetHealth, float duration)
{
    if (duration <= 0)
    {
        setHealth(targetHealth);
        return;
    }

    // 停止之前的动画
    this->unschedule("health_animation_update");
    m_healthBar->stopAllActions();
    this->stopAllActions();

    // 计算起始和结束百分比
    float startHealth = m_currentHealth;

    // 使用 clamp 确保血量在合理范围内
    float endHealth = targetHealth;
    if (endHealth < 0) endHealth = 0;
    if (endHealth > m_maxHealth) endHealth = m_maxHealth;

    // 计算百分比
    float startPercent = (startHealth / m_maxHealth) * 100;
    float endPercent = (endHealth / m_maxHealth) * 100;

    // 创建平滑动画
    auto progressTo = ProgressTo::create(duration, endPercent);
    m_healthBar->runAction(progressTo);

    // 使用成员变量记录动画开始时间
    m_animationStartTime = std::chrono::steady_clock::now();
    m_animationStartHealth = startHealth;
    m_animationTargetHealth = endHealth;
    m_animationDuration = duration;

    // 每帧更新文本
    this->schedule([this](float dt)
        {
        auto now = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(now - m_animationStartTime).count();
        float progress = MIN(elapsedTime / m_animationDuration, 1.0f);

        // 线性插值计算当前血量
        float currentHealth = m_animationStartHealth +
            (m_animationTargetHealth - m_animationStartHealth) * progress;

        // 更新显示文本
        std::string healthStr = StringUtils::format("%.0f/%.0f", currentHealth, m_maxHealth);
        m_healthText->setString(healthStr);

        // 动画结束后更新实际血量并停止调度
        if (progress >= 1.0f) {
            m_currentHealth = m_animationTargetHealth;
            this->unschedule("health_animation_update");

            // 最终确保显示准确
            updateDisplay();
        }
        }, "health_animation_update");
}