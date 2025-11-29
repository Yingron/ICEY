#pragma once
#ifndef __DASH_ATTACK_SKILL_H__
#define __DASH_ATTACK_SKILL_H__

#include "Skill.h"

class DashAttackSkill : public Skill
{
private:
    float m_damage;
    float m_dashDistance;
    float m_dashSpeed;

public:
    DashAttackSkill();
    virtual void activate(Player* player) override;

private:
    void performDash(Player* player);
    std::shared_ptr<Hitbox> createDashHitbox(Player* player);
};

#endif // __DASH_ATTACK_SKILL_H__