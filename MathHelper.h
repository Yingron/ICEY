#pragma once
#ifndef __MATH_HELPER_H__
#define __MATH_HELPER_H__

#include "cocos2d.h"
#include <cmath>

class MathHelper
{
public:
    static const float PI;
    static const float DEG_TO_RAD;
    static const float RAD_TO_DEG;

    // 角度计算
    static float toDegrees(float radians);
    static float toRadians(float degrees);
    static float angleBetween(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

    // 向量操作
    static cocos2d::Vec2 normalize(const cocos2d::Vec2& vector);
    static float distance(const cocos2d::Vec2& a, const cocos2d::Vec2& b);
    static float distanceSquared(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

    // 插值
    static cocos2d::Vec2 moveTowards(const cocos2d::Vec2& current, const cocos2d::Vec2& target, float maxDistanceDelta);

    // 碰撞检测
    static bool rectContains(const cocos2d::Rect& rect, const cocos2d::Vec2& point);
    static bool rectIntersects(const cocos2d::Rect& a, const cocos2d::Rect& b);

    // 缓动函数
    static float easeIn(float t);
    static float easeOut(float t);
    static float easeInOut(float t);
};

#endif // __MATH_HELPER_H__