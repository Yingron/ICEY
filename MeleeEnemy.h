#pragma once
#ifndef __MELEE_ENEMY_H__
#define __MELEE_ENEMY_H__

#include "Enemy.h"

class MeleeEnemy : public Enemy
{
private:
    float m_attackWidth;
    float m_attackHeight;

public:
    virtual bool init() override;
    virtual void updateAI(float delta) override;
    virtual void attack() override;

private:
    std::shared_ptr<Hitbox> createMeleeHitbox();

    CREATE_FUNC(MeleeEnemy);
};

#endif // __MELEE_ENEMY_H__