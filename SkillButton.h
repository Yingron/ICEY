#pragma once
#ifndef __SKILL_BUTTON_H__
#define __SKILL_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SkillButton : public cocos2d::Node
{
private:
    cocos2d::ui::Button* m_button;
    cocos2d::ui::LoadingBar* m_cooldownBar;
    cocos2d::Label* m_keyLabel;

    int m_skillIndex;
    float m_cooldown;
    float m_currentCooldown;
    bool m_isReady;

public:
    static SkillButton* create(int skillIndex, const std::string& normalImage, const std::string& disabledImage = "");
    virtual bool init(int skillIndex, const std::string& normalImage, const std::string& disabledImage);

    void updateCooldown(float delta);
    void setCooldown(float cooldown);
    void startCooldown();
    void resetCooldown();

    void setClickCallback(const std::function<void(cocos2d::Ref*)>& callback);

    bool isReady() const { return m_isReady; }

private:
    void createButton(const std::string& normalImage, const std::string& disabledImage);
    void createCooldownBar();
    void createKeyLabel();

    CREATE_FUNC(SkillButton);
};

#endif // __SKILL_BUTTON_H__