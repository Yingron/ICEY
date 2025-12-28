// MeleeEnemy.cpp
#include "MeleeEnemy.h"
#include "Player.h"

USING_NS_CC;

MeleeEnemy* MeleeEnemy::create(const std::string& enemyType) {
    MeleeEnemy* enemy = new (std::nothrow) MeleeEnemy();
    if (enemy && enemy->init(enemyType)) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool MeleeEnemy::init(const std::string& enemyType) {
    if (!Enemy::init(enemyType)) {
        return false;
    }

    return true;
}

void MeleeEnemy::attack() {
    if (!canAttack() || !_target || _currentComboCooldown > 0.0f) {
        log("Enemy cannot attack: canAttack=%d, has target=%d, combo cooldown=%.2f", canAttack(), _target != nullptr, _currentComboCooldown);
        return;
    }

    // Check if within attack combo window
    if (_currentComboTimer > 0.0f && _currentComboCount < _maxComboCount) {
        // Continue combo attack
        _currentComboCount++;
        log("Enemy combo attack! Combo count: %d/%d", _currentComboCount, _maxComboCount);
    } else {
        // Reset combo
        _currentComboCount = 1;
        log("Enemy starting new attack combo!");
    }
    
    // Reset combo timer
    _currentComboTimer = _comboTimeWindow;
    
    // Record attack information
    log("Enemy starting attack! Combo: %d/%d, Distance to player: %.0f, Attack range: %.0f", 
        _currentComboCount, _maxComboCount, std::abs(_target->getWorldPositionX() - _worldPositionX), _attackRange);

    // Set attack state
    setCurrentState(EnemyState::ATTACKING);
    
    // Play attack animation (supports combo attacks)
    std::string animationName = "attack";
    // If there are different attack animations, can select based on combo count
    // if (_currentComboCount <= _maxComboCount) {
    //     animationName = "attack" + std::to_string(_currentComboCount);
    // }
    playAnimation(animationName, false);
    
    // Set attack cooldown (use shorter cooldown during combo)
    float cooldown = (_currentComboCount > 1) ? _comboCooldown : _attackCooldown;
    _currentAttackCooldown = cooldown;
    _currentComboCooldown = cooldown; // Combo cooldown
    log("Enemy attack cooldown set to: %.2f seconds", cooldown);
    
    // Calculate attack direction
    bool facingRight = (_target->getWorldPositionX() > _worldPositionX);
    this->setFlippedX(!facingRight);
    
    // Get attack animation duration
    float animationDuration = 0.3f; // Animation duration
    if (_animations.count(animationName) > 0 && !_animations[animationName]->getFrames().empty()) {
        animationDuration = _animations[animationName]->getDelayPerUnit() * _animations[animationName]->getFrames().size();
        log("Enemy attack animation duration: %.2f seconds", animationDuration);
    }
    
    // Apply damage to player callback (execute immediately, ensure damage doesn't miss)
    auto damagePlayer = CallFunc::create([this]() {
        // Check if player is within attack range
        if (_target) {
            float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
            log("Enemy damage check: Distance to player: %.0f, Attack range: %.0f", distance, _attackRange);
            
            if (distance <= _attackRange) {
                // Deal damage to player
                float damage = _attackDamage;
                // Increase damage during combo
                if (_currentComboCount > 1) {
                    damage *= (1.0f + (_currentComboCount - 1) * 0.2f); // Each combo increases damage by 20%
                }
                _target->takeDamage(damage);
                log("Enemy attacks player! Damage: %.0f, Combo: %d/%d, Player health: %.0f, Shield: %d", 
                    damage, _currentComboCount, _maxComboCount, _target->getCurrentHealth(), _target->getCurrentShield());
            }
        } else {
            log("Enemy damage check failed: No target");
        }
    });
    
    // Create attack complete callback
    auto attackComplete = CallFunc::create([this]() {
        // Return to chasing or patrolling state after attack completes
        log("Enemy attack complete");
        
        if (_target) {
            float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
            if (distance <= _detectionRange) {
                setCurrentState(EnemyState::CHASING);
            } else {
                setCurrentState(EnemyState::PATROLLING);
            }
        } else {
            setCurrentState(EnemyState::PATROLLING);
        }
    });
    
    // Create complete attack sequence: damage check -> attack complete
    auto attackSequence = Sequence::create(
        DelayTime::create(0.1f), // Short delay before dealing damage
        damagePlayer,
        DelayTime::create(animationDuration - 0.1f), // Remaining animation time
        attackComplete,
        nullptr);
    
    // Set animation state based on enemy status
    this->runAction(attackSequence);
    log("Enemy attack sequence started");
}

void MeleeEnemy::update(float delta) {
    Enemy::update(delta);
    
    // Update combo-related timers
    if (_currentComboTimer > 0.0f) {
        _currentComboTimer -= delta;
        if (_currentComboTimer <= 0.0f) {
            // Combo time window expired, reset combo
            _currentComboCount = 0;
            log("Enemy combo time window expired, combo reset");
        }
    }
    
    if (_currentComboCooldown > 0.0f) {
        _currentComboCooldown -= delta;
        if (_currentComboCooldown <= 0.0f) {
            _currentComboCooldown = 0.0f;
        }
    }
}

void MeleeEnemy::initEnemyData() {
    // Default properties for regular melee enemies
    _maxHealth = 100.0f;
    _currentHealth = _maxHealth;
    _attackDamage = 3.0f;
    _moveSpeed = 80.0f; // Normal enemy movement speed
    _attackRange = 50.0f; // Normal enemy attack range
    _detectionRange = 300.0f;
    _attackCooldown = 1.0f;
    
    // Set boss enemy attributes
    _patrolLeftBound = _worldPositionX - 100.0f;
    _patrolRightBound = _worldPositionX + 100.0f;
    _patrolDuration = 2.0f;
    _patrolTimer = 0.0f;
    
    // Set boss attack range
    _attackDuration = 0.5f;
    _attackTimer = 0.0f;
    
    // Initialize combo attack variables
    _currentComboCount = 0;
    _comboTimeWindow = 0.8f; // Time window for combo attacks
    _currentComboTimer = 0.0f;
    _maxComboCount = 3; // Maximum combo count
    _comboCooldown = 0.3f; // Cooldown between combo attacks
    _currentComboCooldown = 0.0f;
}

void MeleeEnemy::setupAnimations() {
    // Animation settings for enemies
    // Load animations from AnimationManager
    AnimationManager* animMgr = AnimationManager::getInstance();
    
    // For normal melee enemies, use close_combat animations
    // For boss melee enemies, use their specific animations
    std::string animationType = _enemyType;
    if (animationType == "melee") {
        animationType = "close_combat1";
    } else if (animationType == "BOSS1-CAIXUNKUN") {
        animationType = "boss1";
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
}