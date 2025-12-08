#include "MathHelper.h"
#include <algorithm>
#include <cmath>

USING_NS_CC;

// 定义数学常量
const float MathHelper::PI = 3.1415926535f;
const float MathHelper::DEG_TO_RAD = MathHelper::PI / 180.0f;
const float MathHelper::RAD_TO_DEG = 180.0f / MathHelper::PI;

float MathHelper::toDegrees(float radians) {
    return radians * RAD_TO_DEG;
}

float MathHelper::toRadians(float degrees) {
    return degrees * DEG_TO_RAD;
}

float MathHelper::angleBetween(const cocos2d::Vec2& a, const cocos2d::Vec2& b) {
    // 使用点积公式计算夹角：a · b = |a| |b| cos(theta)
    // theta = acos((a · b) / (|a| |b|))
    // 但在2D游戏中，通常关注相对于X轴的夹角或两向量的相对方向
    // 这里实现为计算向量(b - a)的角度
    Vec2 diff = b - a;
    return diff.getAngle(); // 返回弧度
}

cocos2d::Vec2 MathHelper::normalize(const cocos2d::Vec2& vector) {
    return vector.getNormalized();
}

float MathHelper::distance(const cocos2d::Vec2& a, const cocos2d::Vec2& b) {
    return a.distance(b);
}

float MathHelper::distanceSquared(const cocos2d::Vec2& a, const cocos2d::Vec2& b) {
    // 性能优化关键点：避免开平方根运算 (sqrt)
    // 用于比较距离时，直接比较平方值即可
    return a.distanceSquared(b);
}

cocos2d::Vec2 MathHelper::moveTowards(const cocos2d::Vec2& current, const cocos2d::Vec2& target, float maxDistanceDelta) {
    Vec2 a = target - current;
    float magnitude = a.getLength();

    // 如果当前位置已经足够接近目标，或者一步即可到达
    if (magnitude <= maxDistanceDelta || magnitude == 0.f) {
        return target;
    }

    // 否则，向目标方向移动 maxDistanceDelta 的距离
    return current + (a / magnitude) * maxDistanceDelta;
}

bool MathHelper::rectContains(const cocos2d::Rect& rect, const cocos2d::Vec2& point) {
    return rect.containsPoint(point);
}

bool MathHelper::rectIntersects(const cocos2d::Rect& a, const cocos2d::Rect& b) {
    return a.intersectsRect(b);
}

float MathHelper::easeIn(float t) {
    // 二次缓动函数 (Quadratic Ease-In)
    // t 取值范围 
    return t * t;
}

float MathHelper::easeOut(float t) {
    // 二次缓动函数 (Quadratic Ease-Out)
    return t * (2 - t);
}

float MathHelper::easeInOut(float t) {
    // 二次缓动函数 (Quadratic Ease-In-Out)
    if (t < 0.5f) return 2 * t * t;
    return -1 + (4 - 2 * t) * t;
}