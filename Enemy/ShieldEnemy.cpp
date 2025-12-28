// ShieldEnemy.cpp
#include "ShieldEnemy.h"
#include "Player.h"

USING_NS_CC;

ShieldEnemy* ShieldEnemy::create(const std::string& enemyType) {
    ShieldEnemy* enemy = new (std::nothrow) ShieldEnemy();
    if (enemy && enemy->init(enemyType)) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool ShieldEnemy::init(const std::string& enemyType) {
    if (!Enemy::init(enemyType)) {
        return false;
    }

    // Initialize blocking related variables
    _isBlocking = false;
    _currentBlockCooldown = 0.0f;

    return true;
}

void ShieldEnemy::attack() {
    if (!canAttack() || !_target || _isBlocking) {
        return;
    }

    // Stop blocking if currently blocking
    stopBlocking();

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
    
    // Apply damage to player
    auto damagePlayer = CallFunc::create([this]() {
        // Check if player is in attack range
        float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
        if (distance <= _attackRange) {
            // Should call player's takeDamage method here
            _target->takeDamage(_attackDamage);
        }
    });
    
    // Delay damage for more natural attack animation
    this->runAction(Sequence::create(DelayTime::create(animationDuration * 0.3f), damagePlayer, nullptr));
}

void ShieldEnemy::startBlocking() {
    if (_isBlocking || _currentBlockCooldown > 0) {
        return;
    }

    _isBlocking = true;
    playAnimation("block");
    
    // Set blocking duration
    auto stopBlockingAction = CallFunc::create([this]() {
        stopBlocking();
    });
    
    this->runAction(Sequence::create(DelayTime::create(_blockDuration), stopBlockingAction, nullptr));
    
    // Set blocking cooldown
    _currentBlockCooldown = _blockCooldown;
}

void ShieldEnemy::stopBlocking() {
    _isBlocking = false;
    
    // Return to appropriate animation based on current situation
    if (_target && std::abs(_target->getWorldPositionX() - _worldPositionX) <= _detectionRange) {
        playAnimation("run");
    } else {
        playAnimation("walk");
    }
}

void ShieldEnemy::takeDamage(float damage) {
    if (isDead()) {
        return;
    }

    float finalDamage = damage;

    // Check if blocking
    if (_isBlocking) {
        // Reduce damage when blocking
        finalDamage = damage * (1.0f - _blockReduction);
        
        // If damage is completely blocked
        if (finalDamage <= 0) {
            // Can play block success animation or effect here
            return;
        }
    }
    
    // Call parent class's takeDamage to handle damage display and state changes
    Enemy::takeDamage(finalDamage);
}

void ShieldEnemy::updateAI(float delta) {
    // Update block cooldown
    if (_currentBlockCooldown > 0) {
        _currentBlockCooldown -= delta;
    }

    if (!_target) {
        return;
    }

    // Check if player is in detection range
    float distanceToPlayer = std::abs(_target->getWorldPositionX() - _worldPositionX);
    
    if (distanceToPlayer <= _detectionRange) {
        // Player is in detection range
        
        // Check if should block
        if (!_isBlocking && _currentBlockCooldown <= 0 && distanceToPlayer <= _attackRange * 2.0f) {
            // More intelligent blocking logic:
            // 1. Higher chance to block when player is very close
            // 2. Higher chance to block when player is attacking
            float dynamicBlockChance = _blockChance;
            
            // Increase block chance based on distance
            if (distanceToPlayer <= _attackRange) {
                dynamicBlockChance = 0.8f; // 80% chance when very close
            }
            
            // Check if player is attacking
            // Assuming Player has an isAttacking() method
            if (_target && _target->isAttacking()) {
                dynamicBlockChance = 0.9f; // 90% chance when player is attacking
            }
            
            // Randomly decide to block based on dynamic block chance
            if (CCRANDOM_0_1() <= dynamicBlockChance) {
                startBlocking();
                return;
            }
        }
        
        // Chase player if not blocking
        if (!_isBlocking) {
            chase(delta);
            
            // Check if can attack
            if (canAttack()) {
                attack();
            }
        }
    } else {
        // Player is not in detection range, patrol
        if (!_isBlocking) {
            patrol(delta);
        } else {
            stopBlocking();
        }
    }
}

void ShieldEnemy::initEnemyData() {
    // Shield enemy basic properties
    _maxHealth = 120.0f;
    _currentHealth = _maxHealth;
    _attackDamage = 15.0f;
    _moveSpeed = 35.0f;
    _attackRange = 25.0f;
    _detectionRange = 360.0f;
    _attackCooldown = 1.2f;
    
    // Patrol related properties
    _patrolLeftBound = _worldPositionX - 100.0f;
    _patrolRightBound = _worldPositionX + 100.0f;
    _patrolDuration = 2.5f;
    _patrolTimer = 0.0f;
    
    // Blocking related properties
    _blockChance = 0.6f;      // 60% chance to block
    _blockDuration = 1.5f;    // Block duration
    _blockCooldown = 2.0f;    // Block cooldown
    _blockReduction = 0.8f;   // Reduce damage by 80%
}

void ShieldEnemy::setupAnimations() {
    // Load animations from AnimationManager
    AnimationManager* animMgr = AnimationManager::getInstance();
    
    // For normal shield enemies, use close_combat2 animations
    // For boss shield enemies, use their specific animations
    std::string animationType = _enemyType;
    if (animationType == "shield") {
        animationType = "close_combat2";
    } else if (animationType == "BOSS2-MAODIE") {
        animationType = "boss2";
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
    
    // Try attack animation first, then melee_attack as fallback
    anim = animMgr->getAnimation(animationType, "attack");
    if (!anim) {
        anim = animMgr->getAnimation(animationType, "melee_attack");
    }
    if (anim) {
        _animations["attack"] = anim;
    } else {
        CCLOGERROR("Failed to load attack/melee_attack animation for %s", animationType.c_str());
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
    
    anim = animMgr->getAnimation(animationType, "block");
    if (anim) {
        _animations["block"] = anim;
    } else {
        CCLOGERROR("Failed to load block animation for %s", animationType.c_str());
    }
}