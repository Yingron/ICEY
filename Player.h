// Player.h
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "PlayerState.h"
#include "DashBar.h"

class Player : public cocos2d::Sprite {
public:
    static Player* create(const std::string& spriteFile);
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

private:
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

    // 连击相关变量（保留这组定义，删除后面的重复定义）
    bool _canCombo;
    int _comboCount;
    float _comboTimer;
    const float COMBO_WINDOW = 0.3f; // 连击时间窗口

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

    // 冲刺动画键
    std::string _dashLeftAnimKey;
    std::string _dashRightAnimKey;

    // 冲刺更新方法
    void updateDash(float delta);
};
#endif // PLAYER_H