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

    // 设置初始纹理 - 尝试多种动画作为回退
    std::vector<std::string> fallbackAnimations = {"idle", "walk", "run", "attack", "hit", "hurt"};
    bool spriteSet = false;
    
    for (const auto& animName : fallbackAnimations) {
        if (_animations.count(animName) > 0) {
            Animation* anim = _animations[animName];
            if (!anim->getFrames().empty()) {
                SpriteFrame* firstFrame = anim->getFrames().at(0)->getSpriteFrame();
                if (firstFrame) {
                    this->setSpriteFrame(firstFrame);
                    log("Set initial sprite frame for enemy using %s animation", animName.c_str());
                    spriteSet = true;
                    break;
                }
            }
        }
    }
    
    // 作为最后的回退，如果没有任何动画可用，至少设置一个默认的红色方块作为占位符
    if (!spriteSet) {
        log("WARNING: No valid animation frames found for enemy. Using fallback color.");
        this->setColor(Color3B::RED);
        this->setContentSize(Size(50, 50));
        this->setTextureRect(Rect(0, 0, 50, 50));
    }

    // 获取玩家角色大小作为基准
    float playerWidth = 50.0f;  // 默认玩家宽度
    float playerHeight = 100.0f; // 默认玩家高度
    
    // 尝试获取实际玩家大小
    Player* player = Player::getInstance();
    if (player) {
        auto playerSize = player->getContentSize();
        playerWidth = playerSize.width;
        playerHeight = playerSize.height;
    }
    
    // 获取当前敌人大小
    auto enemySize = this->getContentSize();
    
    // 根据敌人类型设置缩放比例
    float scale = 1.0f; // 默认与玩家等大
    
    if (_enemyType == "shield") {
        // 精英带盾敌人：玩家的1.5倍
        scale = 1.5f;
    } else if (_enemyType.find("BOSS") != std::string::npos) {
        // BOSS敌人：玩家的2倍
        scale = 2.0f;
    }
    
    // 设置缩放
    this->setScale(scale);
    
    // Setup physics properties - 移到设置初始纹理之后
    setupPhysics();

    // Start default animation
    playAnimation("idle");

    // Start update
    this->scheduleUpdate();

    return true;
}

Enemy::~Enemy() {
    // Clean up animations in _animations map
    for (auto& animPair : _animations) {
        if (animPair.second) {
            animPair.second->release();
        }
    }
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
            // Disable physics body but don't remove it here - let safelyRemoveEnemy handle it
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->setEnabled(false);
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
                    // 检查enemyManager是否为nullptr
                    if (enemyManager) {
                        enemyManager->removeEnemy(this);
                        log("Enemy removed from EnemyManager after death animation completed");
                    } else {
                        // 如果EnemyManager已经被销毁，直接安全删除自己
                        this->stopAllActions();
                        if (this->getPhysicsBody()) {
                            this->setPhysicsBody(nullptr);
                        }
                        if (this->getParent()) {
                            this->removeFromParentAndCleanup(true);
                            log("Enemy removed directly (EnemyManager was destroyed)");
                        }
                    }
                });
                
                // Create sequence: play animation -> remove enemy
                auto sequence = Sequence::create(deadAction, removeEnemyCallback, nullptr);
                this->runAction(sequence);
                _currentAnimationKey = "dead"; // Update current animation key
            } else {
                // No death animation, remove immediately with a small delay
                auto removeEnemyCallback = CallFunc::create([this, enemyManager]() {
                    // 检查enemyManager是否为nullptr
                    if (enemyManager) {
                        enemyManager->removeEnemy(this);
                        log("Enemy removed from EnemyManager after small delay (no death animation)");
                    } else {
                        // 如果EnemyManager已经被销毁，直接安全删除自己
                        this->stopAllActions();
                        if (this->getPhysicsBody()) {
                            this->setPhysicsBody(nullptr);
                        }
                        if (this->getParent()) {
                            this->removeFromParentAndCleanup(true);
                            log("Enemy removed directly (EnemyManager was destroyed)");
                        }
                    }
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

void Enemy::showDamageNumber(float damage) {
    // 创建伤害数字标签
    auto damageLabel = Label::createWithTTF(StringUtils::format("%.0f", damage), "fonts/arial.ttf", 24);
    if (!damageLabel) {
        log("Failed to create damage label");
        return;
    }
    
    // 设置伤害数字位置（敌人上方）
    Vec2 position = this->getPosition();
    position.y += this->getContentSize().height / 2 + 10;
    damageLabel->setPosition(position);
    
    // 设置颜色（根据伤害值显示不同颜色）
    if (damage < 50) {
        damageLabel->setColor(Color3B(255, 255, 255));  // 白色
    } else if (damage < 100) {
        damageLabel->setColor(Color3B(255, 255, 0));  // 黄色
    } else {
        damageLabel->setColor(Color3B(255, 0, 0));  // 红色
    }
    
    // 添加到父节点
    auto parent = this->getParent();
    if (parent) {
        parent->addChild(damageLabel, 100);  // 确保显示在最上层
        
        // 创建浮动动画
        auto moveUp = MoveBy::create(1.0f, Vec2(0, 50));
        auto fadeOut = FadeOut::create(1.0f);
        auto delay = DelayTime::create(0.5f);
        auto removeSelf = RemoveSelf::create(true);
        
        // 组合动画：向上移动并淡出，然后移除
        auto sequence = Sequence::create(moveUp, fadeOut, removeSelf, nullptr);
        damageLabel->runAction(sequence);
    }
}

void Enemy::takeDamage(float damage) {
    if (isDead() || _currentState == EnemyState::HURT) {
        return;
    }

    // Reduce health
    _currentHealth -= damage;
    
    // 显示伤害数字
    showDamageNumber(damage);
    
    // 添加伤害粒子效果
    auto damageParticles = ParticleSystemQuad::create();
    log("Creating damage particles");
    damageParticles->setPosition(this->getPosition());
    damageParticles->setDuration(0.5f);
    damageParticles->setEmissionRate(100.0f);
    damageParticles->setLife(0.5f);
    damageParticles->setLifeVar(0.2f);
    damageParticles->setSpeed(50.0f);
    damageParticles->setSpeedVar(20.0f);
    damageParticles->setGravity(Vec2(0, 20));
    damageParticles->setAngle(0.0f);
    damageParticles->setAngleVar(360.0f);
    damageParticles->setRadialAccel(-50.0f);
    damageParticles->setTangentialAccel(0.0f);
    damageParticles->setStartSize(5.0f);
    damageParticles->setStartSizeVar(2.0f);
    damageParticles->setStartColor(Color4F(1.0f, 0.3f, 0.0f, 1.0f));
    damageParticles->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 0.0f));
    damageParticles->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
    damageParticles->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
    damageParticles->setBlendFunc(BlendFunc::ADDITIVE);
    
    // 设置纹理，使用简单的白色纹理作为回退
    auto director = Director::getInstance();
    auto textureCache = director->getTextureCache();
    auto texture = textureCache->addImage("particle_texture.png");
    log("Attempting to load particle_texture.png, result: %p", texture);
    if (!texture) {
        // 如果找不到纹理，创建一个简单的白色纹理
        unsigned char pixels[4] = { 255, 255, 255, 255 };
        Texture2D::PixelFormat format = Texture2D::PixelFormat::RGBA8888;
        texture = new Texture2D();
        if (texture) {
            bool initResult = texture->initWithData(pixels, sizeof(pixels), format, 1, 1, Size(1, 1));
            log("Created fallback texture, init result: %d", initResult);
            texture->autorelease();
        } else {
            log("Failed to create fallback texture!");
        }
    }
    damageParticles->setTexture(texture);
    log("Set damage particles texture: %p", texture);
    
    if (this->getParent()) {
        this->getParent()->addChild(damageParticles, 100);
        damageParticles->runAction(Sequence::create(
            DelayTime::create(0.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }
    
    // Check if enemy is dead
    if (_currentHealth <= 0) {
        _currentHealth = 0;
        
        // 添加死亡爆炸粒子效果
        auto explosionParticles = ParticleSystemQuad::create();
        log("Creating explosion particles");
        explosionParticles->setPosition(this->getPosition());
        explosionParticles->setDuration(1.0f);
        explosionParticles->setEmissionRate(200.0f);
        explosionParticles->setLife(1.0f);
        explosionParticles->setLifeVar(0.5f);
        explosionParticles->setSpeed(100.0f);
        explosionParticles->setSpeedVar(50.0f);
        explosionParticles->setGravity(Vec2(0, 0));
        explosionParticles->setAngle(0.0f);
        explosionParticles->setAngleVar(360.0f);
        explosionParticles->setRadialAccel(-100.0f);
        explosionParticles->setTangentialAccel(50.0f);
        explosionParticles->setStartSize(8.0f);
        explosionParticles->setStartSizeVar(4.0f);
        explosionParticles->setStartColor(Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        explosionParticles->setEndColor(Color4F(1.0f, 1.0f, 0.0f, 0.0f));
        explosionParticles->setStartColorVar(Color4F(0.2f, 0.2f, 0.2f, 0.0f));
        explosionParticles->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
        explosionParticles->setBlendFunc(BlendFunc::ADDITIVE);
        
        // 设置纹理，使用简单的白色纹理作为回退
        auto director = Director::getInstance();
        auto textureCache = director->getTextureCache();
        auto texture = textureCache->addImage("particle_texture.png");
        log("Attempting to load particle_texture.png for explosion, result: %p", texture);
        if (!texture) {
            // 如果找不到纹理，创建一个简单的白色纹理
            unsigned char pixels[4] = { 255, 255, 255, 255 };
            Texture2D::PixelFormat format = Texture2D::PixelFormat::RGBA8888;
            texture = new Texture2D();
            if (texture) {
                bool initResult = texture->initWithData(pixels, sizeof(pixels), format, 1, 1, Size(1, 1));
                log("Created fallback texture for explosion, init result: %d", initResult);
                texture->autorelease();
            } else {
                log("Failed to create fallback texture for explosion!");
            }
        }
        explosionParticles->setTexture(texture);
        log("Set explosion particles texture: %p", texture);
        
        if (this->getParent()) {
            this->getParent()->addChild(explosionParticles, 100);
            explosionParticles->runAction(Sequence::create(
                DelayTime::create(1.0f),
                RemoveSelf::create(),
                nullptr
            ));
        }
        
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
    
    // Retain animation to ensure it's not deleted prematurely
    animation->retain();
    
    AnimationCache::getInstance()->addAnimation(animation, animationName);
    _animations[animationName] = animation;
}

void Enemy::playAnimation(const std::string& animationName, bool loop) {
    if (_currentAnimationKey == animationName) {
        return;
    }
    
    _currentAnimationKey = animationName;
    
    // Check if animation exists in _animations map
    if (_animations.count(animationName) == 0) {
        CCLOGERROR("Animation '%s' not found in _animations map", animationName.c_str());
        return;
    }
    
    Animation* animation = _animations[animationName];
    if (!animation) {
        CCLOGERROR("Animation '%s' is null in _animations map", animationName.c_str());
        return;
    }
    
    Action* action;
    if (loop) {
        action = RepeatForever::create(Animate::create(animation));
    } else {
        action = Animate::create(animation);
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
    // Create physics collision body with the scaled size
    auto scaledSize = this->getContentSize() * this->getScale();
    auto physicsBody = PhysicsBody::createBox(scaledSize, PhysicsMaterial(0.1f, 0.0f, 0.5f));
    if (physicsBody) {
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false);    // Disable physics engine gravity for enemies
        physicsBody->setCategoryBitmask(0x04);    // Enemy category
        physicsBody->setCollisionBitmask(0x00);    // No physical collision with any object
        physicsBody->setContactTestBitmask(0x01);  // Detect collision with player
        
        this->setPhysicsBody(physicsBody);
    }
}