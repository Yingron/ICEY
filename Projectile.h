#pragma once
#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "cocos2d.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include <memory>

class Projectile : public cocos2d::Sprite
{
private:
    float m_speed;
    float m_damage;
    float m_lifetime;
    float m_elapsedTime;
    cocos2d::Vec2 m_direction;
    bool m_isActive;

    std::shared_ptr<Hitbox> m_hitbox;
    Hurtbox m_hurtbox;

public:
    virtual bool init() override;
    virtual void update(float delta) override;

    void setup(const cocos2d::Vec2& direction, float speed, float damage, float lifetime);
    void setDirection(const cocos2d::Vec2& direction) { m_direction = direction; }

    // »ñÈ¡Æ÷
    float getDamage() const { return m_damage; }
    bool isActive() const { return m_isActive; }

    void deactivate();

private:
    void updateHitbox();
    void updateHurtbox();

    CREATE_FUNC(Projectile);
};

#endif // __PROJECTILE_H__