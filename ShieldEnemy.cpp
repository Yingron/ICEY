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

    // Check if blocking
    if (_isBlocking) {
        // Reduce damage when blocking
        float reducedDamage = damage * (1.0f - _blockReduction);
        
        // If damage is completely blocked
        if (reducedDamage <= 0) {
            // Can play block success animation or effect here
            return;
        }
        
        // Otherwise take reduced damage
        _currentHealth -= reducedDamage;
    } else {
        // Take full damage when not blocking
        _currentHealth -= damage;
    }
    
    if (_currentHealth <= 0) {
        _currentHealth = 0;
        setCurrentState(EnemyState::DEAD);
    } else {
        setCurrentState(EnemyState::HURT);
    }
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
        if (!_isBlocking && _currentBlockCooldown <= 0 && distanceToPlayer <= _attackRange * 1.5f) {
            // Randomly decide to block based on block chance
            if (CCRANDOM_0_1() <= _blockChance) {
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
    _detectionRange = 180.0f;
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
    // 盾牌敌人资源路径和文件名前缀
    std::string enemyDir = "images/characters/enemies/Close combat enemy2/";
    std::string prefix = enemyDir + "jz2-";
    std::string suffix = "_resized.png";
    
    // Idle animation (using walk frames as idle for now)
    std::vector<std::string> walkFrames;
    for (int i = 1; i <= 20; i++) {
        walkFrames.push_back(enemyDir + prefix + "walk" + std::to_string(i) + suffix);
    }
    loadAnimation("walk", walkFrames, 0.15f);
    
    // Attack animation
    std::vector<std::string> attackFrames;
    for (int i = 1; i <= 20; i++) {
        attackFrames.push_back(enemyDir + prefix + "attack" + std::to_string(i) + suffix);
    }
    loadAnimation("attack", attackFrames, 0.1f);
    
    // Block animation (using gj frames - which likely stands for 'guard' or 'block')
    std::vector<std::string> blockFrames = {
        enemyDir + prefix + "gj1" + suffix,
        enemyDir + prefix + "gj2" + suffix
    };
    loadAnimation("block", blockFrames, 0.2f);
    
    // Hurt animation
    std::vector<std::string> hurtFrames;
    for (int i = 1; i <= 20; i++) {
        hurtFrames.push_back(enemyDir + prefix + "hit" + std::to_string(i) + suffix);
    }
    loadAnimation("hurt", hurtFrames, 0.1f);
    
    // Idle animation
    std::vector<std::string> idleFrames;
    for (int i = 1; i <= 5; i++) {
        idleFrames.push_back(enemyDir + prefix + "walk" + std::to_string(i) + suffix);
    }
    loadAnimation("idle", idleFrames, 0.3f);
    
    // Run animation (use walk animation with faster speed)
    loadAnimation("run", walkFrames, 0.1f);
    
    // Dead animation
    std::vector<std::string> deadFrames;
    for (int i = 1; i <= 5; i++) {
        deadFrames.push_back(enemyDir + prefix + "hit" + std::to_string(i) + suffix);
    }
    loadAnimation("dead", deadFrames, 0.15f);
}