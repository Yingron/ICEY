#pragma once
#ifndef __ELITE_ENEMY_H__
#define __ELITE_ENEMY_H__

#include "Enemy.h"
#include <vector>
#include <memory>

class SpecialAttack;

class EliteEnemy : public Enemy
{
private:
    std::vector<std::shared_ptr<SpecialAttack>> m_specialAttacks;
    int m_currentPhase;
    float m_phaseTransitionHealth;
    bool m_isEnraged;

public:
    virtual bool init() override;
    virtual void updateAI(float delta) override;
    virtual void takeDamage(float damage) override;
    virtual void attack() override;

    void useSpecialAttack(int attackIndex);
    void transitionToNextPhase();
    void enrage();

private:
    void createSpecialAttacks();
    void checkPhaseTransition();

    CREATE_FUNC(EliteEnemy);
};

#endif // __ELITE_ENEMY_H__