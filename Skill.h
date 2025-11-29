#pragma once
#ifndef __SKILL_H__
#define __SKILL_H__

#include "cocos2d.h"
#include "Hitbox.h"
#include <memory>

class Player;

class Skill
{
protected:
    std::string m_name;
    float m_cooldown;
    float m_currentCooldown;
    bool m_isReady;
    float m_manaCost;

public:
    Skill(const std::string& name, float cooldown, float manaCost = 0.0f);
    virtual ~Skill() = default;

    virtual void activate(Player* player) = 0;
    void updateCooldown(float delta);
    void resetCooldown();

    // »ñÈ¡Æ÷
    std::string getName() const { return m_name; }
    float getCooldown() const { return m_cooldown; }
    float getCurrentCooldown() const { return m_currentCooldown; }
    bool isReady() const { return m_isReady; }
    float getManaCost() const { return m_manaCost; }

    float getCooldownPercentage() const { return m_currentCooldown / m_cooldown; }
};

#endif // __SKILL_H__