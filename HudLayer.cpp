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

// 创建连击数显示
void HudLayer::createCombo()
{
    
}

