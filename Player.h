// Player.h
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "PlayerState.h"
#include "DashBar.h"

// 前向声明MainGameScene类
class MainGameScene;
// 前向声明EnemyManager类
class EnemyManager;

class Player : public cocos2d::Sprite {
public:
    static Player* create(const std::string& spriteFile);
    static Player* getInstance();
    std::vector<std::string> optimizeAttackFrames(const std::vector<std::string>& originalFrames, int targetFrameCount);
    virtual bool init(const std::string& spriteFile);
    virtual ~Player();

    void update(float delta);

    // 移动函数
    void moveLeft(float delta);
    void moveRight(float delta);
    void stopMoving();

    // 跳跃函数
    void jump();

    // 状态获取
    PlayerState getCurrentState() const { return _currentState; }
    void resetAnimationState();
    void setCurrentState(PlayerState state);

    // 输入处理
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);

    // 调试方法
    std::string getDebugInfo() const;

    // 测试方法
    cocos2d::Vec2 getCurrentVelocity() const { return _velocity; }

    // 世界坐标相关
    float getWorldPositionX() const { return _worldPositionX; }
    float getWorldPositionY() const { return _worldPositionY; }
    void setWorldPositionX(float x) { _worldPositionX = x; }
    void setWorldPositionY(float y) { _worldPositionY = y; }

    // 跳跃状态查询
    bool isGrounded() const { return _isGrounded; }
    bool isJumping() const { return _currentState == PlayerState::JUMPING; }
    
    // 移动相关查询
    float getMoveSpeed() const { return _moveSpeed; }
    bool isFacingRight() const { return _facingRight; }
    // 添加DashBar引用
    DashBar* _dashBar;

    // 修改冲刺相关方法声明
    bool canDash() const;  // 改为公有方法以便访问
    void setDashBar(DashBar* dashBar) { _dashBar = dashBar; }
    // 冲刺相关方法
    void dashLeft();
    void dashRight();
    void dash();  // 默认向右冲刺
    bool isDashing() const { return _currentState == PlayerState::DASHING; }
    
    // 格挡相关方法
    void startBlocking();
    void endBlocking();
    bool canBlock() const;
    bool isBlocking() const { return _currentState == PlayerState::BLOCKING; }
    
    // 闪避相关方法
    void startDodging(float direction);
    void endDodging();
    bool canDodge() const;
    bool isDodging() const { return _currentState == PlayerState::DODGING; }
    
    // 生命值相关方法和属性
    float getMaxHealth() const { return _maxHealth; }
    float getCurrentHealth() const { return _currentHealth; }
    
    // 护盾相关方法和属性
    int getMaxShield() const { return _maxShield; }
    int getCurrentShield() const { return _currentShield; }
    
    void takeDamage(float damage);
    bool isDead() const { return _currentHealth <= 0; }
    
    // 攻击相关方法
    void detectAndDamageEnemies();
    bool isAttacking() const { return _currentState == PlayerState::ATTACKING; }
    
    // 技能相关方法
    void skill1();
    void skill2();
    bool canUseSkill1() const;
    bool canUseSkill2() const;

    // 添加技能效果应用函数的声明
    void applySkill1Effect();
    void applySkill2Effect();
    
    // 游戏结束相关方法
    void setMainGameScene(MainGameScene* scene) { _mainGameScene = scene; }
    MainGameScene* getMainGameScene() const { return _mainGameScene; }
    
    // 无敌模式相关方法
    // 无敌状态相关
    bool _invincibleModeEnabled = false;      // 无敌模式开关
    void toggleInvincibleMode();
    float getDisplayHealth() const; // 修复函数声明语法错误
    bool isInvincibleModeEnabled() const; // 修复函数声明语法错误
    cocos2d::ParticleSystem* _invincibleParticle = nullptr; // 无敌状态粒子效果
    float _originalMaxHealth = 100.0f;  // 存储原始最大生命值
    float _originalCurrentHealth = 100.0f; // 存储原始当前生命值
    float _displayHealth = 100.0f;      // 显示用的生命值
    void setInvincibleModeVisualFeedback(bool enabled); // 设置无敌模式视觉反馈
    
    // 测试无敌模式功能（可选，用于调试）
    float getHealthPercentage() const;
    void testInvincibleMode(); // 测试方法，开发阶段使用

private:
    static Player* _instance;
    PlayerState _currentState;
    cocos2d::Vec2 _velocity;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _facingRight;
    float _moveSpeed;
    std::string _currentAnimationKey;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, bool> _keyStates;
    // 世界坐标
    float _worldPositionX;
    float _worldPositionY;

    // 跳跃相关变量
    bool _isGrounded;
    float _jumpForce;
    float _gravity;
    int _jumpCount;           // 跳跃次数计数
    int _maxJumpCount;        // 最大跳跃次数（2表示二连跳）
    float _lastJumpTime;      // 上次跳跃时间，用于检测连跳

    // 连击相关变量
    bool _canCombo;
    int _comboCount;
    float _comboTimer;
    const float COMBO_WINDOW = 1.0f; // 连击时间窗口（1秒）
    
    // 攻击间隔相关变量
    float _attackCooldown;
    float _currentAttackCooldown;
    
    // 生命值相关变量
    float _maxHealth;
    float _currentHealth;
    
    // 护盾相关变量
    int _maxShield;
    int _currentShield;

    // 动画管理
    void setupAnimations();
    void loadAnimations();
    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);

    // 物理更新
    void updatePhysics(float delta);
    void updateWorldPosition(float delta);

    std::unordered_map<std::string, cocos2d::Animation*> _animations;
    // 冲刺相关变量
    bool _canDash;               // 是否可以冲刺
    float _dashSpeed;            // 冲刺速度
    float _dashDistance;         // 冲刺距离
    float _dashDuration;         // 冲刺持续时间
    float _dashCooldown;         // 冲刺冷却时间
    float _currentDashDistance;  // 当前已冲刺距离
    bool _isDashLeft;            // 是否向左冲刺
    
    // 格挡相关变量
    bool _isBlocking;            // 是否在格挡状态
    float _blockDuration;        // 格挡持续时间
    float _blockCooldown;        // 格挡冷却时间
    float _blockReduction;       // 格挡减伤百分比
    float _currentBlockDuration; // 当前格挡持续时间
    
    // 闪避相关变量
    bool _isDodging;             // 是否在闪避状态
    float _dodgeDuration;        // 闪避持续时间
    float _dodgeCooldown;        // 闪避冷却时间
    float _dodgeInvincibility;   // 闪避无敌时间
    float _currentDodgeDuration; // 当前闪避持续时间

    // 冲刺动画键
    std::string _dashLeftAnimKey;
    std::string _dashRightAnimKey;

    // 冲刺更新方法
    void updateDash(float delta);
    // 冲刺完成处理函数
    void onDashComplete();
    void endDash();
    
    // 技能相关变量
    float _skill1Damage;         // 技能1伤害
    float _skill1Range;          // 技能1范围
    float _skill1Cooldown;       // 技能1冷却时间
    float _skill1CooldownTimer;  // 技能1冷却计时器
    
    float _skill2Damage;         // 技能2伤害
    float _skill2Range;          // 技能2范围
    float _skill2Cooldown;       // 技能2冷却时间
    float _skill2CooldownTimer;  // 技能2冷却计时器
    
    // 技能动画键
    std::string _skill1AnimKey;
    std::string _skill2AnimKey;
    
    // 指向MainGameScene的指针，用于触发游戏结束
    MainGameScene* _mainGameScene;
    // 技能辅助方法
    EnemyManager* getSafeEnemyManager();

    // 无敌相关变量
    float _invincibleTime;    // 无敌时间
    bool _isInvincible;       // 无敌状态
    int _hurtFlashCount;      // 受伤闪烁计数

};
#endif // PLAYER_H