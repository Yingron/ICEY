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


private:
    void createPauseButton();
};

#endif // __HUD_LAYER_H__