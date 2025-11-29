#pragma once
#ifndef __SHOCKWAVE_SKILL_H__
#define __SHOCKWAVE_SKILL_H__

#include "Skill.h"

class ShockwaveSkill : public Skill
{
private:
    float m_damage;
    float m_range;
    float m_force;

public:
    ShockwaveSkill();
    virtual void activate(Player* player) override;

private:
    void createShockwaveEffect(const cocos2d::Vec2& position, bool facingRight);
    std::shared_ptr<Hitbox> createShockwaveHitbox(const cocos2d::Vec2& position, bool facingRight);
};

#endif // __SHOCKWAVE_SKILL_H__