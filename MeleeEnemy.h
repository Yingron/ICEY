// MeleeEnemy.h
#pragma once
#ifndef MELEE_ENEMY_H
#define MELEE_ENEMY_H

#include "Enemy.h"

class MeleeEnemy : public Enemy {
public:
    static MeleeEnemy* create(const std::string& enemyType = "melee");
    virtual bool init(const std::string& enemyType) override;

    // Implement attack method
    virtual void attack() override;

protected:
    // Initialize enemy data
    virtual void initEnemyData() override;
    
    // Setup animations
    virtual void setupAnimations() override;

private:
    // Melee enemy specific properties
    float _attackDuration;
    float _attackTimer;
};

#endif // MELEE_ENEMY_H