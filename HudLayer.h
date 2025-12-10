#pragma once
#ifndef __HUD_LAYER_H__
#define __HUD_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"HealthBar.h"
#include"SheldBar.h"

class HealthBar;
class SheldBar;

class HudLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Button* m_pauseButton;
    HealthBar* m_healthBar;      // 血条指针
    SheldBar* m_sheldBar;        // 护盾条指针

    // 连击数相关成员（简化版）
    cocos2d::Label* m_comboLabel;      // 连击数字显示
    cocos2d::Label* m_comboTextLabel;  // "COMBO"文本
    int m_currentCombo;                // 当前连击数
public:
    virtual bool init() override;

    // 设置暂停按钮回调
    void setPauseCallback(const std::function<void(cocos2d::Ref*)>& callback);

    // 创建各个UI组件
    void createHealthBar();
    void createSheldBar();
    void createCombo();
    CREATE_FUNC(HudLayer);

    // 获取UI组件（新增）
    HealthBar* getHealthBar() { return m_healthBar; }
    SheldBar* getSheldBar() { return m_sheldBar; }

    // 更新接口（新增）
    void updateHealth(float health);     // 更新血量显示
    void updateSheld(int sheld);         // 更新护盾显示
    void setMaxHealth(float health);
    void setMaxSheld(int sheld);
    // 连击数接口（新增）
    void addCombo();                    // 增加连击数
    void resetCombo();                  // 重置连击数
    void setCombo(int combo);           // 直接设置连击数
    int getCurrentCombo() const { return m_currentCombo; } // 获取当前连击数

private:
    void createPauseButton();
    void updateComboDisplay();
};

#endif // __HUD_LAYER_H__