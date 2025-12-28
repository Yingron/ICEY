#pragma once
#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HealthBar : public cocos2d::Node
{
private:
    cocos2d::ui::LoadingBar* m_healthBar;
    cocos2d::ui::LoadingBar* m_healthBarBackground;
    cocos2d::Label* m_healthText;

    float m_currentHealth;
    float m_maxHealth;

    // 动画相关变量
    std::chrono::steady_clock::time_point m_animationStartTime;
    float m_animationStartHealth;
    float m_animationTargetHealth;
    float m_animationDuration;

public:
    static HealthBar* create(float maxHealth);
    virtual bool init(float maxHealth);
    float getCurrentHealth() const { return m_currentHealth; }

    void setHealth(float health);
    void setMaxHealth(float maxHealth);
    //void updateColorByHealth(float percent);
    void updateDisplay();
    void HealthBar::setHealthWithAnimation(float health, float duration);
    

private:
    void createHealthBar();
};

#endif // __HEALTH_BAR_H__