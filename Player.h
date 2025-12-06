// Player.h - ���ӵ��Է���
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

    // �ƶ�����
    void moveLeft(float delta);
    void moveRight(float delta);
    void stopMoving();

    // ״̬��ȡ
    PlayerState getCurrentState() const { return _currentState; }
    void setCurrentState(PlayerState state);

    // ���봦��
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);

    // ���Է���
    cocos2d::Vec2 getCurrentVelocity() const { return _velocity; }

private:
    PlayerState _currentState;
    cocos2d::Vec2 _velocity;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _facingRight;
    float _moveSpeed;
    std::string _currentAnimationKey;  // 跟踪当前播放的动画键

    // ��������
    void setupAnimations();
    void loadAnimations();
    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);

    std::unordered_map<std::string, cocos2d::Animation*> _animations;
};

#endif // PLAYER_H