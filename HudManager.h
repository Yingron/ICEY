#pragma once
#ifndef __HUD_MANAGER_H__
#define __HUD_MANAGER_H__

#include "cocos2d.h"

// 前向声明
class HudLayer;

class HudManager
{
private:
    static HudManager* s_instance;    // 单例实例
    HudLayer* m_hudLayer;             // HUD层指针

    HudManager() : m_hudLayer(nullptr) {}  // 私有构造函数
    ~HudManager() = default;                // 私有析构函数

public:
    // 获取单例实例
    static HudManager* getInstance();

    // 销毁单例（在游戏结束时调用）
    static void destroyInstance();

    // 设置HUD层（在MainGameScene中调用）
    void setHudLayer(HudLayer* hudLayer);

    // 获取HUD层
    HudLayer* getHudLayer() { return m_hudLayer; }

    // 检查HUD层是否可用
    bool isHudLayerAvailable() const { return m_hudLayer != nullptr; }

    // === 便捷更新方法 ===

    // 更新血量
    static void updateHealth(float health);

    // 更新护盾
    static void updateSheld(int sheld);

    // 设置最大血量
    static void setMaxHealth(float maxHealth);

    // 设置最大护盾
    static void setMaxSheld(int maxSheld);

    // 增加连击数
    static void addCombo();

    // 重置连击数
    static void resetCombo();

    // 设置连击数
    static void setCombo(int combo);

    // 获取当前连击数
    static int getCurrentCombo();

    // 显示/隐藏HUD
    static void showHud(bool show);

private:
    // 防止拷贝
    HudManager(const HudManager&) = delete;
    HudManager& operator=(const HudManager&) = delete;
};

#endif // __HUD_MANAGER_H__