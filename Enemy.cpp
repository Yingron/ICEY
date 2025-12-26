// Enemy.cpp
#include "Enemy.h"
#include "Player.h"
#include "EnemyManager.h"
#include "cocos2d.h"

USING_NS_CC;

// Enemy is an abstract class, cannot be instantiated directly
Enemy* Enemy::create(const std::string& enemyType) {
    CCLOGERROR("Cannot instantiate abstract class Enemy directly");
    return nullptr;
}

bool Enemy::init(const std::string& enemyType) {
    if (!Sprite::init()) {
        return false;
    }

    _enemyType = enemyType;
    _currentState = EnemyState::IDLE;
    _target = nullptr;
    _facingRight = true;
    _worldPositionX = 0.0f;
    _worldPositionY = 0.0f;
    _velocity = Vec2::ZERO;
    _currentAttackCooldown = 0.0f;

    // Initialize enemy data
    initEnemyData();
    
    // Setup animations
    setupAnimations();

    // 设置初始纹理 - 从idle动画中获取第一帧
    if (_animations.count("idle") > 0) {
        Animation* idleAnim = _animations["idle"];
        if (!idleAnim->getFrames().empty()) {
            SpriteFrame* firstFrame = idleAnim->getFrames().at(0)->getSpriteFrame();
            if (firstFrame) {
                this->setSpriteFrame(firstFrame);
                log("Set initial sprite frame for enemy");
            }
        }
    }

    // Setup physics properties - 移到设置初始纹理之后
    setupPhysics();

    // Start default animation
    playAnimation("idle");

    // Start update
    this->scheduleUpdate();

    return true;
}

Enemy::~Enemy() {
    // No need to clean up animation cache as it's shared globally
    _animations.clear();
}

void Enemy::update(float delta) {
    // Update attack cooldown
    if (_currentAttackCooldown > 0) {
        _currentAttackCooldown -= delta;
    }

    // Execute different updates based on current state
    switch (_currentState) {
        case EnemyState::IDLE:
        case EnemyState::PATROLLING:
        case EnemyState::CHASING:
            updateAI(delta);
            updatePhysics(delta);
            break;
        case EnemyState::ATTACKING:
            // Don't move during attack state
            break;
        case EnemyState::HURT:
            // Handle hit stun in hurt state
            break;
        case EnemyState::DEAD:
            // No updates in dead state
            return;
    }

    // Update world position
    updateWorldPosition(delta);
}

void Enemy::setCurrentState(EnemyState state) {
    if (_currentState == state) {
        return;
    }

    EnemyState previousState = _currentState;
    _currentState = state;

    // Play animation based on new state
    switch (state) {
        case EnemyState::IDLE:
            playAnimation("idle");
            break;
        case EnemyState::PATROLLING:
            playAnimation("walk");
            break;
        case EnemyState::CHASING:
            playAnimation("run");
            break;
        case EnemyState::ATTACKING:
            playAnimation("attack");
            break;
        case EnemyState::HURT: {
            // Disable physics on hurt
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->setEnabled(false);
            }
            
            // Play hurt animation once
            if (_animations.count("hurt") > 0) {
                playAnimation("hurt", false);
            }
            
            // Create sequence to play hurt animation and then return to previous state
            auto returnToPrevious = CallFunc::create([this]() {
                // Re-enable physics
                if (this->getPhysicsBody()) {
                    this->getPhysicsBody()->setEnabled(true);
                }
                
                // Return to previous behavior based on player proximity
                if (_target) {
                    float distanceToPlayer = std::abs(_target->getWorldPositionX() - _worldPositionX);
                    if (distanceToPlayer <= _detectionRange) {
                        setCurrentState(EnemyState::CHASING);
                    } else {
                        setCurrentState(EnemyState::PATROLLING);
                    }
                } else {
                    setCurrentState(EnemyState::PATROLLING);
                }
            });
            
            float hurtDuration = 0.5f; // Default duration if no animation
            if (_animations.count("hurt") > 0 && _animations["hurt"] != nullptr) {
                hurtDuration = _animations["hurt"]->getDelayPerUnit() * _animations["hurt"]->getFrames().size();
            }
            
            // Create sequence
            auto sequence = Sequence::create(
                DelayTime::create(hurtDuration),
                returnToPrevious,
                nullptr
            );
            
            // Run the sequence to return to previous state
            this->runAction(sequence);
            break;
        }
        case EnemyState::DEAD: {
            // Disable and remove physics body
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->setEnabled(false);
                this->removeComponent(this->getPhysicsBody());
            }
            
            // Play death animation if available and wait for completion before removing
            auto enemyManager = EnemyManager::getInstance();
            if (_animations.count("dead") > 0 && _animations["dead"] != nullptr) {
                // Get dead animation
                Animation* deadAnim = _animations["dead"];
                // Create dead animation action
                auto deadAction = Animate::create(deadAnim);
                
                // Create callback to remove enemy after animation completes
                auto removeEnemyCallback = CallFunc::create([this, enemyManager]() {
                    enemyManager->removeEnemy(this);
                    log("Enemy removed from EnemyManager after death animation completed");
                });
                
                // Create sequence: play animation -> remove enemy
                auto sequence = Sequence::create(deadAction, removeEnemyCallback, nullptr);
                this->runAction(sequence);
                _currentAnimationKey = "dead"; // Update current animation key
            } else {
                // No death animation, remove immediately with a small delay
                auto removeEnemyCallback = CallFunc::create([this, enemyManager]() {
                    enemyManager->removeEnemy(this);
                    log("Enemy removed from EnemyManager after small delay (no death animation)");
                });
                auto sequence = Sequence::create(DelayTime::create(0.1f), removeEnemyCallback, nullptr);
                this->runAction(sequence);
            }
            
            break;
        }
    }
}

bool Enemy::canAttack() const {
    if (isDead() || _currentState == EnemyState::ATTACKING || _currentState == EnemyState::HURT) {
        return false;
    }
    
    if (_currentAttackCooldown > 0) {
        return false;
    }
    
    if (!_target) {
        return false;
    }
    
    // Check if target is within attack range
    float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
    return distance <= _attackRange;
}

void Enemy::takeDamage(float damage) {
    if (isDead() || _currentState == EnemyState::HURT) {
        return;
    }

    // Reduce health
    _currentHealth -= damage;
    
    // Check if enemy is dead
    if (_currentHealth <= 0) {
        _currentHealth = 0;
        setCurrentState(EnemyState::DEAD);
    } else {
        // Set to hurt state
        setCurrentState(EnemyState::HURT);
    }
}

void Enemy::setWorldPosition(float x, float y) {
    _worldPositionX = x;
    _worldPositionY = y;
    
    // Get visible size to calculate ground level
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float groundScreenY = visibleSize.height * 0.3f;
    
    // Update sprite position with correct screen coordinate mapping
    this->setPosition(x, groundScreenY);
}

void Enemy::updateAI(float delta) {
    if (!_target) {
        return;
    }

    // Check if player is within detection range
    float distanceToPlayer = std::abs(_target->getWorldPositionX() - _worldPositionX);
    
    if (distanceToPlayer <= _detectionRange) {
        // Player is within detection range, chase
        chase(delta);
        
        // Check if can attack
        if (canAttack()) {
            attack();
        }
    } else {
        // Player is not within detection range, patrol
        patrol(delta);
    }
}

void Enemy::onPlayerDetected() {
    setCurrentState(EnemyState::CHASING);
}

void Enemy::onPlayerLost() {
    setCurrentState(EnemyState::PATROLLING);
}

bool Enemy::onContactBegin(PhysicsContact& contact) {
    // Default implementation, subclasses can override
    return true;
}

void Enemy::loadAnimation(const std::string& animationName, const std::vector<std::string>& frames, float delay) {
    Animation* animation = Animation::create();
    auto fileUtils = FileUtils::getInstance();
    
    for (const auto& framePath : frames) {
        // Try to load the frame from file
        std::vector<std::string> possiblePaths = {
            framePath,
            "images/characters/enemies/" + framePath,
            "Resources/images/characters/enemies/" + framePath,
            "C:/aishi/test3/Resources/images/characters/enemies/" + framePath
        };
        
        SpriteFrame* frame = nullptr;
        
        // Try each possible path
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                // Load the texture
                auto texture = Director::getInstance()->getTextureCache()->addImage(path);
                if (texture) {
                    // Create sprite frame from texture
                    frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                    log("Loaded enemy animation frame from: %s", path.c_str());
                    break;
                }
            }
        }
        
        if (frame) {
            animation->addSpriteFrame(frame);
        } else {
            CCLOGERROR("Failed to load enemy animation frame: %s", framePath.c_str());
        }
    }
    
    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(true);
    
    AnimationCache::getInstance()->addAnimation(animation, animationName);
    _animations[animationName] = animation;
}

void Enemy::playAnimation(const std::string& animationName, bool loop) {
    if (_currentAnimationKey == animationName) {
        return;
    }
    
    _currentAnimationKey = animationName;
    
    Action* action;
    if (loop) {
        action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(animationName)));
    } else {
        action = Animate::create(AnimationCache::getInstance()->getAnimation(animationName));
    }
    
    this->stopAllActions();
    this->runAction(action);
}

void Enemy::patrol(float delta) {
    // Default patrol behavior: move back and forth between left and right bounds
    if (_currentState != EnemyState::PATROLLING) {
        setCurrentState(EnemyState::PATROLLING);
    }

    _patrolTimer += delta;
    
    // Simple patrol logic: move left and right
    if (_patrolTimer >= _patrolDuration) {
        _patrolTimer = 0.0f;
        _facingRight = !_facingRight;
    }
    
    // Set velocity
    float direction = _facingRight ? 1.0f : -1.0f;
    _velocity.x = direction * _moveSpeed;
    
    // Flip sprite
    this->setFlippedX(!_facingRight);
}

void Enemy::chase(float delta) {
    if (_currentState != EnemyState::CHASING) {
        setCurrentState(EnemyState::CHASING);
    }
    
    if (!_target) {
        return;
    }
    
    // Calculate direction to player
    float direction = (_target->getWorldPositionX() > _worldPositionX) ? 1.0f : -1.0f;
    
    // Set velocity
    _velocity.x = direction * _moveSpeed;
    
    // Flip sprite
    this->setFlippedX(direction < 0);
}

void Enemy::checkTargetInRange() {
    // Check if target is within attack range
    if (_target) {
        float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
        if (distance <= _attackRange && canAttack()) {
            attack();
        }
    }
}

void Enemy::updatePhysics(float delta) {
    // Update position
    _worldPositionX += _velocity.x * delta;
    
    // Remove gravity application to prevent enemies from falling
    // _velocity.y -= 98.0f * delta; - This was causing continuous downward movement
    
    // Keep enemies on same level as player
    if (_target) {
        _worldPositionY = _target->getWorldPositionY();
    } else {
        _worldPositionY = 0; // Default to ground if no target
    }
    _velocity.y = 0;
    
    // Don't update sprite position here - let updateWorldPosition handle it
    // to ensure proper screen coordinate mapping
}

void Enemy::updateWorldPosition(float delta) {
    // Get visible size to calculate ground level
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float groundScreenY = visibleSize.height * 0.3f;
    
    // Update sprite position with correct screen coordinate mapping
    this->setPosition(_worldPositionX, groundScreenY);
}

void Enemy::setupPhysics() {
    // Create physics collision body
    auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.5f));
    if (physicsBody) {
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false);    // Disable physics engine gravity for enemies
        physicsBody->setCategoryBitmask(0x04);    // Enemy category
        physicsBody->setCollisionBitmask(0x00);    // No physical collision with any object
        physicsBody->setContactTestBitmask(0x01);  // Detect collision with player
        
        this->setPhysicsBody(physicsBody);
    }
}