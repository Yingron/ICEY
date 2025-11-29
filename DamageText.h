#pragma once
#ifndef __DAMAGE_TEXT_H__
#define __DAMAGE_TEXT_H__

#include "cocos2d.h"

class DamageText : public cocos2d::Label
{
private:
    float m_lifetime;
    float m_elapsedTime;
    cocos2d::Vec2 m_velocity;

public:
    static DamageText* create(int damage, const cocos2d::Vec2& position, bool isCritical = false);
    virtual bool init(int damage, const cocos2d::Vec2& position, bool isCritical);

    virtual void update(float delta);

private:
    void setupAppearance(int damage, bool isCritical);
    void animateText();

    CREATE_FUNC(DamageText);
};

#endif // __DAMAGE_TEXT_H__