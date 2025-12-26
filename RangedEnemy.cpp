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
        
        // Set projectile speed and direction
        float direction = _facingRight ? 1.0f : -1.0f;
        Vec2 velocity = Vec2(direction * _projectileSpeed, 0.0f);
        
        // Move projectile
        float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
        float duration = distance / _projectileSpeed;
        
        // Create move action
        auto moveAction = MoveBy::create(duration, Vec2(distance * direction, 0.0f));
        
        // Create remove projectile action
        auto removeAction = RemoveSelf::create();
        
        // Execute action sequence
        projectile->runAction(Sequence::create(moveAction, removeAction, nullptr));
        
        // Set projectile physics properties (if collision detection needed)
        auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.5f));
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(0x04);    // Projectile category
        physicsBody->setCollisionBitmask(0x01);    // Collide with player
        physicsBody->setContactTestBitmask(0x01);  // Test collision
        
        projectile->setPhysicsBody(physicsBody);
        
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
    _projectileSpeed = 200.0f;
    _projectileDamage = _attackDamage;
}

void RangedEnemy::setupAnimations() {
    // 远程敌人资源路径和文件名前缀
    std::string enemyDir = "images/characters/enemies/Remote combat enemy/";
    std::string prefix = "yz-";
    std::string suffix = "_resized.png";
    
    // Walk animation
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
    
    // Idle animation
    std::vector<std::string> idleFrames;
    for (int i = 1; i <= 5; i++) {
        idleFrames.push_back(enemyDir + prefix + "walk" + std::to_string(i) + suffix);
    }
    loadAnimation("idle", idleFrames, 0.3f);
    
    // Run animation (use walk animation with faster speed)
    loadAnimation("run", walkFrames, 0.1f);
    
    // Hurt animation
    std::vector<std::string> hurtFrames;
    for (int i = 1; i <= 10; i++) {
        hurtFrames.push_back(enemyDir + prefix + "attack" + std::to_string(i) + suffix);
    }
    loadAnimation("hurt", hurtFrames, 0.1f);
    
    // Dead animation
    std::vector<std::string> deadFrames;
    for (int i = 1; i <= 5; i++) {
        deadFrames.push_back(enemyDir + prefix + "attack" + std::to_string(i) + suffix);
    }
    loadAnimation("dead", deadFrames, 0.15f);
}