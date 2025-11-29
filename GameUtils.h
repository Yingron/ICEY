#pragma once
#ifndef __GAME_UTILS_H__
#define __GAME_UTILS_H__

#include "cocos2d.h"
#include <random>

class GameUtils
{
public:
    // 随机数生成
    static int randomInt(int min, int max);
    static float randomFloat(float min, float max);
    static bool randomBool();

    // 数学计算
    static float lerp(float a, float b, float t);
    static cocos2d::Vec2 lerp(const cocos2d::Vec2& a, const cocos2d::Vec2& b, float t);
    static float clamp(float value, float min, float max);
    static int clamp(int value, int min, int max);

    // 字符串处理
    static std::string formatTime(float seconds);
    static std::string formatCombo(int combo);

    // 坐标转换
    static cocos2d::Vec2 screenToWorld(const cocos2d::Vec2& screenPos);
    static cocos2d::Vec2 worldToScreen(const cocos2d::Vec2& worldPos);

    // 调试工具
    static void drawDebugRect(cocos2d::Node* parent, const cocos2d::Rect& rect, const cocos2d::Color4F& color);
    static void log(const std::string& message);

private:
    static std::random_device m_randomDevice;
    static std::mt19937 m_randomGenerator;
};

#endif // __GAME_UTILS_H__