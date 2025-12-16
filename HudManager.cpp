#include "HudManager.h"
#include "HudLayer.h"
#include "HealthBar.h"
#include "SheldBar.h"
#include"DashBar.h"

USING_NS_CC;

// 静态成员初始化
HudManager* HudManager::s_instance = nullptr;

// 获取单例实例
HudManager* HudManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new (std::nothrow) HudManager();
        CCASSERT(s_instance, "Failed to create HudManager instance");
    }
    return s_instance;
}

// 销毁单例
void HudManager::destroyInstance()
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

// 设置HUD层
void HudManager::setHudLayer(HudLayer* hudLayer)
{
    m_hudLayer = hudLayer;

    if (m_hudLayer)
    {
        log("HudManager: HUD layer set successfully");
    }
    else
    {
        log("HudManager: HUD layer set to null");
    }
}

// 更新血量
void HudManager::updateHealth(float health)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot update health, HUD layer not available");
        return;
    }

    // 调用HudLayer的更新方法（需要HudLayer中有这些方法）
    if (auto healthBar = instance->m_hudLayer->getHealthBar())
    {
        healthBar->setHealthWithAnimation(health,0.5);
        log("HudManager: Updated health to %.1f", health);
    }
    else
    {
        log("HudManager: HealthBar not found in HUD layer");
    }
}

// 更新护盾
void HudManager::updateSheld(int sheld)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot update sheld, HUD layer not available");
        return;
    }

    // 调用HudLayer的更新方法（需要HudLayer中有这些方法）
    if (auto sheldBar = instance->m_hudLayer->getSheldBar())
    {
        sheldBar->setSheld(sheld);
        log("HudManager: Updated sheld to %d", sheld);
    }
    else
    {
        log("HudManager: SheldBar not found in HUD layer");
    }
}

// 设置最大血量
void HudManager::setMaxHealth(float maxHealth)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot set max health, HUD layer not available");
        return;
    }

    // 如果需要在HudLayer中添加setMaxHealth方法
     instance->m_hudLayer->setMaxHealth(maxHealth);
    log("HudManager: setMaxHealth called with %.1f", maxHealth);
}

// 设置最大护盾
void HudManager::setMaxSheld(int maxSheld)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot set max sheld, HUD layer not available");
        return;
    }

    // 如果需要在HudLayer中添加setMaxSheld方法
     instance->m_hudLayer->setMaxSheld(maxSheld);
    log("HudManager: setMaxSheld called with %d", maxSheld);
}

void HudManager::addCombo()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot add combo, HUD layer not available");
        return;
    }

    instance->m_hudLayer->addCombo();
    log("HudManager: Combo added");
}

// 重置连击数
void HudManager::resetCombo()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot reset combo, HUD layer not available");
        return;
    }

    instance->m_hudLayer->resetCombo();
    log("HudManager: Combo reset");
}

// 设置连击数
void HudManager::setCombo(int combo)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot set combo, HUD layer not available");
        return;
    }

    instance->m_hudLayer->setCombo(combo);
    log("HudManager: Combo set to %d", combo);
}

// 获取当前连击数
int HudManager::getCurrentCombo()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot get combo, HUD layer not available");
        return 0;
    }

    return instance->m_hudLayer->getCurrentCombo();
}

float HudManager::getCurrentHealth()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot get current health, HUD layer not available");
        return 0.0f;
    }

    if (auto healthBar = instance->m_hudLayer->getHealthBar())
    {
        return healthBar->getCurrentHealth();
    }

    log("HudManager: HealthBar not found in HUD layer");
    return 0.0f;
}

// 显示/隐藏HUD
void HudManager::showHud(bool show)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot show/hide HUD, HUD layer not available");
        return;
    }

    instance->m_hudLayer->setVisible(show);
    log("HudManager: HUD visibility set to %s", show ? "visible" : "hidden");
}

bool HudManager::useDash()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot use dash, HUD layer not available");
        return false;
    }

    // 直接从 dashBar 调用 useDash()
    if (auto dashBar = instance->m_hudLayer->getDashBar())
    {
        bool result = dashBar->useDash();  // 改为 dashBar->useDash()
        log("HudManager: Dash used. Result: %s", result ? "success" : "failed");
        return result;
    }
    else
    {
        log("HudManager: DashBar not found");
        return false;
    }
}

// 恢复一次冲刺
void HudManager::rechargeDash()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot recharge dash, HUD layer not available");
        return;
    }

    instance->m_hudLayer->rechargeDash();
    log("HudManager: Dash recharged");
}

// 立即恢复所有冲刺
void HudManager::rechargeAllDashes()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot recharge all dashes, HUD layer not available");
        return;
    }

    instance->m_hudLayer->rechargeAllDashes();
    log("HudManager: All dashes recharged");
}

// 获取可用冲刺次数
int HudManager::getAvailableDashes()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot get available dashes, HUD layer not available");
        return 0;
    }

    return instance->m_hudLayer->getAvailableDashes();
}

// 设置最大冲刺次数
void HudManager::setMaxDashes(int maxDashes)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot set max dashes, HUD layer not available");
        return;
    }

    instance->m_hudLayer->setMaxDashes(maxDashes);
    log("HudManager: Max dashes set to %d", maxDashes);
}

// 设置冲刺恢复时间
void HudManager::setDashRechargeTime(float time)
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot set dash recharge time, HUD layer not available");
        return;
    }

    instance->m_hudLayer->setDashRechargeTime(time);
    log("HudManager: Dash recharge time set to %.1f seconds", time);
}

// 是否正在充能
bool HudManager::isDashRecharging()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot check dash recharge status, HUD layer not available");
        return false;
    }

    if (auto dashBar = instance->m_hudLayer->getDashBar())
    {
        return dashBar->isRecharging();
    }

    return false;
}

// 获取剩余充能时间
float HudManager::getDashRemainingRechargeTime()
{
    auto instance = getInstance();

    if (!instance->isHudLayerAvailable())
    {
        log("HudManager: Cannot get dash remaining time, HUD layer not available");
        return 0.0f;
    }

    if (auto dashBar = instance->m_hudLayer->getDashBar())
    {
        return dashBar->getRemainingRechargeTime();
    }

    return 0.0f;
}
