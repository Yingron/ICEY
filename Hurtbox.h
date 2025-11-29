#pragma once
#ifndef __HURTBOX_H__
#define __HURTBOX_H__

#include "cocos2d.h"

class Hurtbox
{
private:
    cocos2d::Rect m_rect;
    cocos2d::Node* m_owner;
    bool m_isActive;

public:
    Hurtbox(cocos2d::Node* owner);

    void setRect(const cocos2d::Rect& rect) { m_rect = rect; }
    cocos2d::Rect getRect() const { return m_rect; }

    void setActive(bool active) { m_isActive = active; }
    bool isActive() const { return m_isActive; }

    cocos2d::Node* getOwner() const { return m_owner; }

    void setPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPosition() const;

    bool intersects(const cocos2d::Rect& otherRect) const;
};

#endif // __HURTBOX_H__