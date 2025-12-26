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
    if (!canAttack() || !_target) {
        log("Enemy cannot attack: canAttack=%d, has target=%d", canAttack(), _target != nullptr);
        return;
    }

    // 记录攻击信息
    log("Enemy starting attack! Distance to player: %.0f, Attack range: %.0f", 
        std::abs(_target->getWorldPositionX() - _worldPositionX), _attackRange);

    // 设置攻击状态
    setCurrentState(EnemyState::ATTACKING);
    
    // 播放攻击动画
    playAnimation("attack", false);
    
    // 设置攻击冷却
    _currentAttackCooldown = _attackCooldown;
    log("Enemy attack cooldown set to: %.2f seconds", _attackCooldown);
    
    // 计算攻击方向
    bool facingRight = (_target->getWorldPositionX() > _worldPositionX);
    this->setFlippedX(!facingRight);
    
    // 获取攻击动画时长
    float animationDuration = 0.3f; // 默认时长
    if (_animations.count("attack") > 0 && !_animations["attack"]->getFrames().empty()) {
        animationDuration = _animations["attack"]->getDelayPerUnit() * _animations["attack"]->getFrames().size();
        log("Enemy attack animation duration: %.2f seconds", animationDuration);
    }
    
    // 应用伤害到玩家的回调（立即执行，确保伤害不会丢失）
    auto damagePlayer = CallFunc::create([this]() {
        // 检查攻击范围内是否有玩家
        if (_target) {
            float distance = std::abs(_target->getWorldPositionX() - _worldPositionX);
            log("Enemy damage check: Distance to player: %.0f, Attack range: %.0f", distance, _attackRange);
            
            if (distance <= _attackRange) {
                // 对玩家造成伤害
                _target->takeDamage(_attackDamage);
                log("Enemy attacks player! Damage: %.0f, Player health: %.0f, Shield: %d", 
                    _attackDamage, _target->getCurrentHealth(), _target->getCurrentShield());
            }
        } else {
            log("Enemy damage check failed: No target");
        }
    });
    
    // 创建攻击完成的回调
    auto attackComplete = CallFunc::create([this]() {
        // 攻击完成后回到追击或巡逻状态
        log("Enemy attack complete, returning to chase/patrol");
        
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
    
    // 创建完整的攻击序列：伤害检测 -> 攻击完成
    auto attackSequence = Sequence::create(
        DelayTime::create(0.1f), // 短暂延迟后立即造成伤害
        damagePlayer,
        DelayTime::create(0.4f), // 剩余动画时间
        attackComplete,
        nullptr);
    
    // 运行攻击序列
    this->runAction(attackSequence);
    log("Enemy attack sequence started");
}

void MeleeEnemy::initEnemyData() {
    // 近战兵的基本属性
    _maxHealth = 100.0f;
    _currentHealth = _maxHealth;
    _attackDamage = 20.0f;
    _moveSpeed = 80.0f; // 增加移动速度，使敌人更容易追上玩家
    _attackRange = 50.0f; // 增加攻击范围，使敌人更容易进入攻击范围
    _detectionRange = 150.0f;
    _attackCooldown = 1.0f;
    
    // 巡逻相关
    _patrolLeftBound = _worldPositionX - 100.0f;
    _patrolRightBound = _worldPositionX + 100.0f;
    _patrolDuration = 2.0f;
    _patrolTimer = 0.0f;
    
    // 攻击相关
    _attackDuration = 0.5f;
    _attackTimer = 0.0f;
}

void MeleeEnemy::setupAnimations() {
    // Melee enemy animations - using BOSS1-CAIXUNKUN resources as placeholder
    std::string enemyDir = "images/characters/enemies/BOSS1-CAIXUNKUN/";
    std::string prefix = enemyDir + "cxk-";
    std::string suffix = "_resized.png";
    
    // Idle animation
    std::vector<std::string> idleFrames;
    for (int i = 1; i <= 10; i++) {
        idleFrames.push_back(prefix + "idle" + std::to_string(i) + suffix);
    }
    loadAnimation("idle", idleFrames, 0.3f);
    
    // Walking animation
    std::vector<std::string> walkFrames;
    for (int i = 1; i <= 10; i++) {
        walkFrames.push_back(prefix + "walk" + std::to_string(i) + suffix);
    }
    loadAnimation("walk", walkFrames, 0.15f);
    
    // Running animation (using walk frames with faster speed)
    loadAnimation("run", walkFrames, 0.1f);
    
    // Attack animation
    std::vector<std::string> attackFrames;
    for (int i = 1; i <= 10; i++) {
        attackFrames.push_back(prefix + "attack" + std::to_string(i) + suffix);
    }
    loadAnimation("attack", attackFrames, 0.1f);
    
    // Hurt animation
    std::vector<std::string> hurtFrames;
    for (int i = 1; i <= 10; i++) {
        hurtFrames.push_back(prefix + "hit" + std::to_string(i) + suffix);
    }
    loadAnimation("hurt", hurtFrames, 0.1f);
    
    // Dead animation (using hit frames as dead for now)
    loadAnimation("dead", hurtFrames, 0.15f);
}