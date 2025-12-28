// RangedEnemy.h
#pragma once
#ifndef RANGED_ENEMY_H
#define RANGED_ENEMY_H

#include "Enemy.h"

class RangedEnemy : public Enemy {
public:
    static RangedEnemy* create(const std::string& enemyType = "ranged");
    virtual bool init(const std::string& enemyType) override;

    // Implement attack method
    virtual void attack() override;

protected:
    // Initialize enemy data
    virtual void initEnemyData() override;
    
    // Setup animations
    virtual void setupAnimations() override;

private:
    // Ranged enemy specific properties
    float _projectileSpeed;
    float _projectileDamage;
};

#endif // RANGED_ENEMY_H