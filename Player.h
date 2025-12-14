// Player.h
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "PlayerState.h"

class Player : public cocos2d::Sprite {
public:
    static Player* create(const std::string& spriteFile);
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

private:
    PlayerState _currentState;
    cocos2d::Vec2 _velocity;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _facingRight;
    float _moveSpeed;
    std::string _currentAnimationKey;

    // 世界坐标
    float _worldPositionX;
    float _worldPositionY;

    // 跳跃相关变量
    bool _isGrounded;
    float _jumpForce;
    float _gravity;
    bool _canJump;

    // 动画管理
    void setupAnimations();
    void loadAnimations();
    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);

    // 物理更新
    void updatePhysics(float delta);
    void updateWorldPosition(float delta);

    std::unordered_map<std::string, cocos2d::Animation*> _animations;
};

#endif // PLAYER_H