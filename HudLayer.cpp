#include "HUDLayer.h"
#include "HealthBar.h"
#include"SheldBar.h"

using namespace cocos2d;
using namespace ui;


// 初始化 HUD 层
bool HudLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 获取可见区域大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置 HUD 层级，确保显示在最前面
    this->setLocalZOrder(1000);

    createPauseButton();
    createHealthBar();
    createSheldBar();
    createCombo();

    return true;
}

// 创建暂停按钮
void HudLayer::createPauseButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建暂停按钮
    m_pauseButton = Button::create("pause_button.png", "pause_button_pressed.png");
    if (!m_pauseButton)
    {
        // 如果图片不存在，创建默认按钮
        m_pauseButton = Button::create();
        m_pauseButton->setTitleText("Pause");
        m_pauseButton->setTitleFontSize(20);
        m_pauseButton->setColor(Color3B::WHITE);
    }

    // 设置按钮位置（右上角）
    m_pauseButton->setPosition(Vec2(
        visibleSize.width - m_pauseButton->getContentSize().width / 2 - 20,
        visibleSize.height - m_pauseButton->getContentSize().height / 2 - 20
    ));

    // 添加到 HUD 层
    this->addChild(m_pauseButton);
}

// 设置暂停按钮回调
void HudLayer::setPauseCallback(const std::function<void(Ref*)>& callback)
{
    if (m_pauseButton && callback)
    {
        m_pauseButton->addClickEventListener(callback);
    }
}

// 创建血条
void HudLayer::createHealthBar()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float maxHealth=100.0f;//可修改为随等级，视情况决定
    m_healthBar = HealthBar::create(maxHealth); // 保存到成员变量

    // 设置血条位置（屏幕左上角）
    m_healthBar->setPosition(Vec2(
        200,
        visibleSize.height - 50
    ));

    // 添加到 HudLayer
    this->addChild(m_healthBar);
}

// 创建护盾条
void HudLayer::createSheldBar()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    int maxSheld = 10; // 可以根据游戏设计调整
    m_sheldBar = SheldBar::create(maxSheld);

    if (!m_sheldBar)
    {
        log("Failed to create sheld bar!");
        return;
    }

    // 设置护盾条位置（在血条下方）
    m_sheldBar->setPosition(Vec2(
        200, // 与血条对齐
        visibleSize.height - 100 // 血条下方50像素
    ));
    this->addChild(m_sheldBar);
}

// 新增：更新血量显示
void HudLayer::updateHealth(float health)
{
    if (m_healthBar)
    {
        m_healthBar->setHealth(health);
        log("HudLayer: Health updated to %.1f", health);
    }
}

// 新增：更新护盾显示
void HudLayer::updateSheld(int sheld)
{
    if (m_sheldBar)
    {
        m_sheldBar->setSheld(sheld);
        log("HudLayer: Sheld updated to %d", sheld);
    }
}

// 创建连击数显示***************************************
void HudLayer::createCombo()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 初始化连击数
    m_currentCombo = 0;

    // ================== COMBO文本 ==================
    m_comboTextLabel = Label::createWithTTF("COMBO", "fonts/Marker Felt.ttf", 24);
    if (!m_comboTextLabel)
    {
        m_comboTextLabel = Label::createWithSystemFont("COMBO", "Arial", 24);
    }

    // 设置COMBO文本样式
    m_comboTextLabel->setColor(Color3B::YELLOW);
    m_comboTextLabel->enableOutline(Color4B::BLACK, 2);

    // 位置：屏幕右上方
    m_comboTextLabel->setPosition(Vec2(
        visibleSize.width - 150,
        visibleSize.height - 60
    ));

    // 初始隐藏（0连击时不显示）
    m_comboTextLabel->setVisible(false);

    this->addChild(m_comboTextLabel, 3);

    // ================== 连击数字显示 ==================
    m_comboLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 36);
    if (!m_comboLabel)
    {
        m_comboLabel = Label::createWithSystemFont("0", "Arial", 36);
    }

    // 设置数字样式
    m_comboLabel->setColor(Color3B::WHITE);
    m_comboLabel->enableOutline(Color4B(255, 0, 0, 255), 3); // 红色描边

    // 位置：在COMBO文本下方
    m_comboLabel->setPosition(Vec2(
        visibleSize.width - 150,
        visibleSize.height - 100
    ));

    // 初始隐藏
    m_comboLabel->setVisible(false);

    this->addChild(m_comboLabel, 4);

    log("Combo display created successfully");
    
}

void HudLayer::addCombo()
{
    m_currentCombo++;
    updateComboDisplay();

    // 简单的动画效果
    if (m_comboLabel && m_currentCombo > 1)
    {
        m_comboLabel->stopAllActions();

        // 缩放动画
        auto scaleUp = ScaleTo::create(0.1f, 1.3f);
        auto scaleDown = ScaleTo::create(0.1f, 1.0f);
        auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
        m_comboLabel->runAction(sequence);

        // 连击数高时有颜色变化
        if (m_currentCombo >= 20)
        {
            // 20连击以上：彩虹色/闪烁效果
            m_comboLabel->setColor(Color3B(255, 0, 255)); // 紫色
            m_comboLabel->enableGlow(Color4B(255, 105, 180, 255)); // 粉色辉光

            // 添加闪烁动画
            /*auto blink = Sequence::create(
                FadeTo::create(0.3f, 150),
                FadeTo::create(0.3f, 255),
                nullptr
            );
            m_comboLabel->runAction(RepeatForever::create(blink));*/
        }
        else if (m_currentCombo >= 15)
        {
            // 15-19连击：金色+闪烁效果
            m_comboLabel->setColor(Color3B(255, 215, 0)); // 金色
            m_comboLabel->enableGlow(Color4B(255, 255, 0, 200)); // 金色辉光
        }
        else if (m_currentCombo >= 10)
        {
            // 10-14连击：橙色
            m_comboLabel->setColor(Color3B(255, 140, 0)); // 亮橙色
            m_comboLabel->enableGlow(Color4B(255, 165, 0, 150)); // 橙色辉光
        }
        else if (m_currentCombo >= 5)
        {
            // 5-9连击：黄色
            m_comboLabel->setColor(Color3B(255, 255, 0)); // 黄色
            m_comboLabel->enableOutline(Color4B(255, 140, 0, 255), 3); // 橙色描边
        }
        else if (m_currentCombo >= 3)
        {
            // 3-4连击：浅黄色
            m_comboLabel->setColor(Color3B(255, 255, 150)); // 浅黄色
            m_comboLabel->enableOutline(Color4B(255, 200, 0, 255), 2); // 橙黄色描边
        }
        else if (m_currentCombo >= 1)
        {
            // 1-2连击：白色（默认）
            m_comboLabel->setColor(Color3B::WHITE);
            m_comboLabel->enableOutline(Color4B(255, 0, 0, 255), 2); // 红色描边
        }
    }

    log("Combo added: %d", m_currentCombo);
}

// 重置连击数
void HudLayer::resetCombo()
{
    if (m_currentCombo > 0)
    {
        log("Combo reset from %d to 0", m_currentCombo);
    }

    m_currentCombo = 0;
    updateComboDisplay();
}

// 直接设置连击数
void HudLayer::setCombo(int combo)
{
    m_currentCombo = combo;
    updateComboDisplay();
}

// 更新连击显示
void HudLayer::updateComboDisplay()
{
    if (m_comboLabel && m_comboTextLabel)
    {
        if (m_currentCombo > 0)
        {
            // 有连击时显示
            m_comboLabel->setVisible(true);
            m_comboTextLabel->setVisible(true);
            m_comboLabel->setString(StringUtils::format("%d", m_currentCombo));
        }
        else
        {
            // 没有连击时隐藏
            m_comboLabel->setVisible(false);
            m_comboTextLabel->setVisible(false);
        }
    }
}