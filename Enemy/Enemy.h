// Enemy.h
#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "cocos2d.h"
#include "EnemyState.h"
#include "AnimationManager.h"

// Forward declaration
class Player;

class Enemy : public cocos2d::Sprite {
protected:
    // Enemy data
    EnemyState _currentState;
    std::string _enemyType;

public:
    static Enemy* create(const std::string& enemyType);
    virtual bool init(const std::string& enemyType);
    virtual ~Enemy();

    virtual void update(float delta) override;

    // State management
    EnemyState getCurrentState() const { return _currentState; }
    virtual void setCurrentState(EnemyState state);

    // Attack related
    virtual void attack() = 0; // Pure virtual function, must be implemented by subclasses
    virtual bool canAttack() const;
    virtual void takeDamage(float damage);
    
    // Health management
    float getMaxHealth() const { return _maxHealth; }
    float getCurrentHealth() const { return _currentHealth; }
    
    // Get enemy type
    std::string getEnemyType() const { return _enemyType; }
    bool isDead() const { return _currentState == EnemyState::DEAD; }

    // Target tracking
    void setTarget(Player* target) { _target = target; }
    Player* getTarget() const { return _target; }

    // World coordinates
    float getWorldPositionX() const { return _worldPositionX; }
    float getWorldPositionY() const { return _worldPositionY; }
    void setWorldPosition(float x, float y);

    // AI related
    virtual void updateAI(float delta);
    virtual void onPlayerDetected();
    virtual void onPlayerLost();

    // Physics related
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    // Damage display
    void showDamageNumber(float damage);

protected:
    // State related
    Player* _target; // Attack target (usually the player)

    // Properties
    float _maxHealth;
    float _currentHealth;
    float _attackDamage;
    float _moveSpeed;
    float _attackRange;
    float _detectionRange;
    float _attackCooldown;
    float _currentAttackCooldown;

    // Movement related
    cocos2d::Vec2 _velocity;
    bool _facingRight;
    float _worldPositionX;
    float _worldPositionY;

    // Animation related
    std::string _currentAnimationKey;
    std::unordered_map<std::string, cocos2d::Animation*> _animations;

    // Patrol related
    float _patrolLeftBound;
    float _patrolRightBound;
    float _patrolTimer;
    float _patrolDuration;
    
    // Patrol points system
    std::vector<cocos2d::Vec2> _patrolPoints;
    int _currentPatrolPointIndex;
    bool _patrolClockwise;

    // Initialization methods
    virtual void initEnemyData() = 0; // Initialize enemy data
    virtual void setupAnimations() = 0; // Setup animations
    void loadAnimation(const std::string& animationName, const std::vector<std::string>& frames, float delay);
    void playAnimation(const std::string& animationName, bool loop = true);

    // AI behaviors
    virtual void patrol(float delta);
    virtual void chase(float delta);
    virtual void checkTargetInRange();

    // Physics updates
    void updatePhysics(float delta);
    void updateWorldPosition(float delta);

    // Collision detection
    void setupPhysics();
};

#endif // ENEMY_H