#pragma once
// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "PlayerState.h"

class Player : public cocos2d::Sprite {
public:
    // 创建函数
    static Player* create(const std::string& spriteFile);

    // 初始化
    virtual bool init(const std::string& spriteFile);

    // 析构函数
    virtual ~Player();

    // 更新
    void update(float delta);

    // 移动控制
    void moveLeft(float delta);
    void moveRight(float delta);
    void stopMoving();

    // 状态管理
    PlayerState getCurrentState() const { return _currentState; }
    void setCurrentState(PlayerState state);

    // 输入处理
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);

private:
    PlayerState _currentState;
    cocos2d::Vec2 _velocity;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _facingRight;  // 记录面向方向
    float _moveSpeed;

    // 动画相关
    void setupAnimations();
    void loadAnimations();
    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);

    // 动画缓存
    std::unordered_map<std::string, cocos2d::Animation*> _animations;
};

#endif // PLAYER_H