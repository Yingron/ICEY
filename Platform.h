#pragma once
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "cocos2d.h"

class Platform : public cocos2d::Node
{
private:
    cocos2d::Rect m_collisionRect;
    bool m_isPassThrough; // 可穿透平台（从下方跳上来）

public:
    static Platform* create(const cocos2d::Rect& rect, bool isPassThrough = false);
    virtual bool init(const cocos2d::Rect& rect, bool isPassThrough);

    cocos2d::Rect getCollisionRect() const { return m_collisionRect; }
    bool isPassThrough() const { return m_isPassThrough; }

    bool checkCollision(const cocos2d::Rect& otherRect, bool isFalling = false) const;

    CREATE_FUNC(Platform);
};

#endif // __PLATFORM_H__