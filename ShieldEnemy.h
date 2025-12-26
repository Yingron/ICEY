// ShieldEnemy.h
#pragma once
#ifndef SHIELD_ENEMY_H
#define SHIELD_ENEMY_H

#include "Enemy.h"

class ShieldEnemy : public Enemy {
public:
    static ShieldEnemy* create(const std::string& enemyType = "shield");
    virtual bool init(const std::string& enemyType) override;

    // Implement attack method
    virtual void attack() override;
    
    // Shield related methods
    bool isBlocking() const { return _isBlocking; }
    void startBlocking();
    void stopBlocking();
    
    // Override takeDamage method, considering shield effects
    virtual void takeDamage(float damage) override;

protected:
    // Initialize enemy data
    virtual void initEnemyData() override;
    
    // Setup animations
    virtual void setupAnimations() override;
    
    // Override AI update, adding shield behavior
    virtual void updateAI(float delta) override;

private:
    // Shield enemy specific properties
    bool _isBlocking;
    float _blockChance;
    float _blockDuration;
    float _blockCooldown;
    float _currentBlockCooldown;
    float _blockReduction;
};

#endif // SHIELD_ENEMY_H