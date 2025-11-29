#pragma once
#ifndef __ICEY_H__
#define __ICEY_H__

#include "Player.h"
#include "Skill.h"
#include <vector>
#include <memory>

class ICEY : public Player
{
private:
    int m_comboCount;
    float m_comboTimer;
    float m_dodgeCooldown;
    float m_dodgeCooldownTimer;

    std::vector<std::shared_ptr<Skill>> m_skills;
    bool m_hasSecondJump;
    bool m_usedSecondJump;

public:
    virtual bool init() override;
    virtual void update(float delta) override;

    // 战斗系统
    void performLightAttack();
    void performHeavyAttack();
    void performAirAttack();
    void useSkill(int skillIndex);

    // 特殊能力
    void triggerSecondJump();
    void resetCombo();
    void incrementCombo();

    // 获取器
    int getComboCount() const { return m_comboCount; }
    float getDodgeCooldown() const { return m_dodgeCooldownTimer; }
    bool canSecondJump() const { return m_hasSecondJump && !m_usedSecondJump; }

    // 技能管理
    void addSkill(std::shared_ptr<Skill> skill);
    std::shared_ptr<Skill> getSkill(int index);

private:
    void updateComboTimer(float delta);
    void updateDodgeCooldown(float delta);
    void createSkills();

    CREATE_FUNC(ICEY);
};

#endif // __ICEY_H__