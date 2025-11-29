#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "PlayerState.h"
#include "Hitbox.h"
#include "Hurtbox.h"

class Player : public cocos2d::Sprite
{
protected:
    float m_health;
    float m_maxHealth;
    float m_speed;
    float m_jumpForce;
    bool m_isFacingRight;
    bool m_isGrounded;
    bool m_isInvincible;
    float m_invincibleTimer;

    PlayerState m_currentState;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> m_actions;

    Hurtbox m_hurtbox;

public:
    virtual bool init() override;
    virtual void update(float delta) override;

    // ÒÆ¶¯¿ØÖÆ
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stopMoving();
    virtual void jump();
    virtual void attack();
    virtual void dodge();

    // ×´Ì¬¿ØÖÆ
    void takeDamage(float damage);
    void setState(PlayerState newState);
    PlayerState getState() const { return m_currentState; }

    // ÊôÐÔ»ñÈ¡Æ÷
    float getHealth() const { return m_health; }
    float getMaxHealth() const { return m_maxHealth; }
    bool isFacingRight() const { return m_isFacingRight; }
    bool isGrounded() const { return m_isGrounded; }
    bool isInvincible() const { return m_isInvincible; }

    // Åö×²¼ì²â
    virtual Hurtbox& getHurtbox() { return m_hurtbox; }
    virtual void updateHurtbox();

    CREATE_FUNC(Player);
};

#endif // __PLAYER_H__