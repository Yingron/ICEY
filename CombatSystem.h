#pragma once
#ifndef __COMBAT_SYSTEM_H__
#define __COMBAT_SYSTEM_H__

#include "cocos2d.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include <vector>
#include <memory>

class CombatSystem
{
private:
    std::vector<std::shared_ptr<Hitbox>> m_activeHitboxes;
    std::vector<std::shared_ptr<Hurtbox>> m_activeHurtboxes;
    cocos2d::Node* m_parentNode;

public:
    static CombatSystem* getInstance();

    void setParentNode(cocos2d::Node* parent) { m_parentNode = parent; }

    void registerHitbox(std::shared_ptr<Hitbox> hitbox);
    void registerHurtbox(std::shared_ptr<Hurtbox> hurtbox);
    void unregisterHitbox(std::shared_ptr<Hitbox> hitbox);
    void unregisterHurtbox(std::shared_ptr<Hurtbox> hurtbox);

    void update(float delta);
    void checkCollisions();

    void spawnDamageText(const cocos2d::Vec2& position, int damage, bool isCritical = false);
    void spawnHitEffect(const cocos2d::Vec2& position);

    void clearAll();

private:
    CombatSystem() = default;
    ~CombatSystem() = default;
};

#endif // __COMBAT_SYSTEM_H__