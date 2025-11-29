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

public:
    static HealthBar* create(float maxHealth);
    virtual bool init(float maxHealth);

    void setHealth(float health);
    void setMaxHealth(float maxHealth);

    void updateDisplay();

private:
    void createHealthBar();

    CREATE_FUNC(HealthBar);
};

#endif // __HEALTH_BAR_H__