// RangedEnemy.cpp
#include "RangedEnemy.h"
#include "Player.h"

USING_NS_CC;

RangedEnemy* RangedEnemy::create(const std::string& enemyType) {
    RangedEnemy* enemy = new (std::nothrow) RangedEnemy();
    if (enemy && enemy->init(enemyType)) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool RangedEnemy::init(const std::string& enemyType) {
    if (!Enemy::init(enemyType)) {
        return false;
    }

    return true;
}

void RangedEnemy::attack() {
    if (!canAttack() || !_target) {
        return;
    }

    // Set attack state
    setCurrentState(EnemyState::ATTACKING);
    
    // Play attack animation
    playAnimation("attack", false);
    
    // Set attack cooldown
    _currentAttackCooldown = _attackCooldown;
    
    // Calculate attack direction
    bool facingRight = (_target->getWorldPositionX() > _worldPositionX);
    this->setFlippedX(!facingRight);
    
    // Create attack complete callback
    auto attackComplete = CallFunc::create([this]() {
        // Return to chasing or patrolling state after attack
        if (_target && std::abs(_target->getWorldPositionX() - _worldPositionX) <= _detectionRange) {
            setCurrentState(EnemyState::CHASING);
        } else {
            setCurrentState(EnemyState::PATROLLING);
        }
    });
    
    // Delay attack complete callback for animation duration
    float animationDuration = _animations["attack"]->getDelayPerUnit() * _animations["attack"]->getFrames().size();
    this->runAction(Sequence::create(DelayTime::create(animationDuration), attackComplete, nullptr));
    
    // Create projectile callback
    auto createProjectile = CallFunc::create([this]() {
        if (!_target) return;
        
        // Create projectile
        auto projectile = Sprite::create("enemy_projectile.png");
        if (!projectile) {
            // If image not found, use a simple red square instead
            projectile = Sprite::create();
            projectile->setColor(Color3B::RED);
            projectile->setContentSize(Size(10, 10));
        }
        
        // Set projectile position (in front of enemy)
        float offsetX = _facingRight ? 20.0f : -20.0f;
        Vec2 startPos = Vec2(_worldPositionX + offsetX, _worldPositionY + 10.0f);
        projectile->setPosition(startPos);
        
        // Add to current scene
        this->getParent()->addChild(projectile, 5);
        
        // Predict player position based on movement
        Vec2 targetPosition = _target->getPosition();
        Vec2 playerVelocity = Vec2(_target->getMoveSpeed() * (_target->isFacingRight() ? 1.0f : -1.0f), 0.0f);
        
        // Calculate distance and flight time
        float distance = _target->getPosition().distance(this->getPosition());
        float flightTime = distance / _projectileSpeed;
        
        // Predict where player will be when projectile arrives
        Vec2 predictedPosition = targetPosition + playerVelocity * flightTime;
        
        // Calculate direction to predicted position
        Vec2 direction = predictedPosition - this->getPosition();
        direction.normalize();
        
        // Move projectile to predicted position
        Vec2 targetOffset = direction * distance;
        auto moveAction = MoveBy::create(flightTime, targetOffset);
        
        // Create remove projectile action
        auto removeAction = RemoveSelf::create();
        
        // Execute action sequence
        projectile->runAction(Sequence::create(moveAction, removeAction, nullptr));
        
        // Set projectile physics properties (if collision detection needed)
        auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.5f));
        if (physicsBody) {
            physicsBody->setDynamic(true);
            physicsBody->setGravityEnable(false);
            physicsBody->setCategoryBitmask(0x04);    // Projectile category
            physicsBody->setCollisionBitmask(0x01);    // Collide with player
            physicsBody->setContactTestBitmask(0x01);  // Test collision
            
            projectile->setPhysicsBody(physicsBody);
        }
        
        // Projectile collision detection logic should be added here
        // Inflict damage when projectile collides with player
    });
    
    // Delay projectile creation for more natural attack animation
    this->runAction(Sequence::create(DelayTime::create(animationDuration * 0.5f), createProjectile, nullptr));
}

void RangedEnemy::initEnemyData() {
    // Ranged enemy basic properties
    _maxHealth = 60.0f;
    _currentHealth = _maxHealth;
    _attackDamage = 15.0f;
    _moveSpeed = 40.0f;
    _attackRange = 100.0f;
    _detectionRange = 200.0f;
    _attackCooldown = 1.5f;
    
    // Patrol related properties
    _patrolLeftBound = _worldPositionX - 150.0f;
    _patrolRightBound = _worldPositionX + 150.0f;
    _patrolDuration = 3.0f;
    _patrolTimer = 0.0f;
    
    // Projectile related properties
    _projectileSpeed = 280.0f;
    _projectileDamage = _attackDamage;
}

void RangedEnemy::setupAnimations() {
    // Load animations from AnimationManager
    AnimationManager* animMgr = AnimationManager::getInstance();
    
    // For normal ranged enemies, use remote_enemy animations
    // For boss ranged enemies, use their specific animations
    std::string animationType = _enemyType;
    if (animationType == "ranged") {
        animationType = "remote_enemy";
    } else if (animationType == "BOSS3-NAILONG") {
        animationType = "boss3";
    }
    
    // Load all required animations for this enemy type
    // Add null checks to prevent crashes
    Animation* anim;
    
    anim = animMgr->getAnimation(animationType, "idle");
    if (anim) {
        _animations["idle"] = anim;
    } else {
        CCLOGERROR("Failed to load idle animation for %s", animationType.c_str());
    }
    
    anim = animMgr->getAnimation(animationType, "walk");
    if (anim) {
        _animations["walk"] = anim;
    } else {
        CCLOGERROR("Failed to load walk animation for %s", animationType.c_str());
    }
    
    anim = animMgr->getAnimation(animationType, "run");
    if (anim) {
        _animations["run"] = anim;
    } else {
        CCLOGERROR("Failed to load run animation for %s", animationType.c_str());
    }
    
    // Try attack animation first, then ranged_attack as fallback
    anim = animMgr->getAnimation(animationType, "attack");
    if (!anim) {
        anim = animMgr->getAnimation(animationType, "ranged_attack");
    }
    if (anim) {
        _animations["attack"] = anim;
    } else {
        CCLOGERROR("Failed to load attack/ranged_attack animation for %s", animationType.c_str());
    }
    
    anim = animMgr->getAnimation(animationType, "hurt");
    if (anim) {
        _animations["hurt"] = anim;
    } else {
        CCLOGERROR("Failed to load hurt animation for %s", animationType.c_str());
    }
    
    anim = animMgr->getAnimation(animationType, "dead");
    if (anim) {
        _animations["dead"] = anim;
    } else {
        CCLOGERROR("Failed to load dead animation for %s", animationType.c_str());
    }
}