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
    float animationDuration = 0.5f; // Default duration if animation not found
    if (_animations.count("attack") > 0 && _animations["attack"] != nullptr) {
        animationDuration = _animations["attack"]->getDelayPerUnit() * _animations["attack"]->getFrames().size();
    }
    this->runAction(Sequence::create(DelayTime::create(animationDuration), attackComplete, nullptr));
    
    // Create projectile callback
        auto createProjectile = CallFunc::create([this]() {
            if (!_target) return;
            
            // Create projectile
            auto projectile = Sprite::create("images/characters/enemies/Remote combat enemy/Remote combat enemy-attack1_resized.png");
            if (!projectile) {
                // If image not found, use a simple red square instead
                projectile = Sprite::create();
                projectile->setColor(Color3B::RED);
                projectile->setContentSize(Size(10, 10));
            }
            
            // Set projectile position (in front of enemy, same Y level)
            float offsetX = _facingRight ? 20.0f : -20.0f;
            Vec2 startPos = Vec2(_worldPositionX + offsetX, _worldPositionY);
            projectile->setPosition(startPos);
            
            // Add to current scene
            auto parent = this->getParent();
            if (!parent) {
                log("Enemy parent is null, cannot add projectile");
                delete projectile; // Clean up if we can't add to parent
                return;
            }
            parent->addChild(projectile, 5);
            
            // Calculate target position with bullet prediction
            float playerX = _target->getWorldPositionX();
            float playerY = _worldPositionY; // Keep same Y level as enemy
            
            // Get player velocity for prediction
            Vec2 playerVelocity = _target->getCurrentVelocity();
            
            // Calculate distance and direction
            float distanceX = playerX - startPos.x;
            float distance = abs(distanceX);
            float flightTime = distance / _projectileSpeed;
            
            // Predict where the player will be when the bullet arrives
            float predictedX = playerX + playerVelocity.x * flightTime;
            float predictedY = playerY + playerVelocity.y * flightTime;
            
            // Apply limits to prediction to avoid extreme cases
            float maxPredictionOffset = 100.0f; // Maximum prediction distance
            predictedX = clampf(predictedX, playerX - maxPredictionOffset, playerX + maxPredictionOffset);
            
            // Set target position (with some randomness for variety)
            float randomOffset = (CCRANDOM_0_1() - 0.5f) * 30.0f; // -15 to 15 pixels
            Vec2 targetPosition = Vec2(predictedX + randomOffset, predictedY);
            
            // Move projectile horizontally to player's X position (same Y level)
            auto moveAction = MoveTo::create(flightTime, targetPosition);
            
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
    _attackDamage = 3.0f;
    _moveSpeed = 40.0f;
    _attackRange = 100.0f;
    _detectionRange = 400.0f;
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