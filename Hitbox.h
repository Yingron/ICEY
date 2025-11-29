#pragma once
#ifndef __HITBOX_H__
#define __HITBOX_H__

#include "cocos2d.h"

class Hitbox
{
private:
    cocos2d::Rect m_rect;
    float m_damage;
    float m_duration;
    float m_elapsedTime;
    bool m_isActive;
    cocos2d::Node* m_owner;
    std::string m_type;

public:
    Hitbox(const cocos2d::Rect& rect, float damage, float duration, cocos2d::Node* owner, const std::string& type = "normal");

    void update(float delta);

    // »ñÈ¡Æ÷
    cocos2d::Rect getRect() const { return m_rect; }
    float getDamage() const { return m_damage; }
    bool isActive() const { return m_isActive; }
    cocos2d::Node* getOwner() const { return m_owner; }
    std::string getType() const { return m_type; }

    void setPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPosition() const;

    // Åö×²¼ì²â
    bool intersects(const cocos2d::Rect& otherRect) const;
};

#endif // __HITBOX_H__