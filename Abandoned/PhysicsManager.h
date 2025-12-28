#pragma once
#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "cocos2d.h"

// 物理碰撞类别定义
enum class PhysicsCategory {
    NONE = 0,
    PLAYER = 1 << 0,   // 1
    PLAYER_ATK = 1 << 1,   // 2
    ENEMY = 1 << 2,   // 4
    ENEMY_ATK = 1 << 3,   // 8
    ITEM = 1 << 4,   // 16
    WORLD = 1 << 5,   // 32
    ALL = 0xFFFFFFFF
};

class PhysicsManager : public cocos2d::Ref {
public:
    static PhysicsManager* getInstance();
    static void destroyInstance();

    void init(cocos2d::Scene* scene);
    void update(float delta);

    // 碰撞检测辅助函数
    bool checkAttackHit(cocos2d::Node* attacker, cocos2d::Node* target,
        float attackRange, bool usePhysics = false);

    // 生成攻击检测区域
    cocos2d::Rect getAttackRange(cocos2d::Node* attacker, float range,
        bool facingRight, float yOffset = 0);

private:
    PhysicsManager();
    ~PhysicsManager();
    static PhysicsManager* _instance;

    cocos2d::Scene* _scene;
    cocos2d::EventListenerPhysicsContact* _contactListener;

    // 碰撞回调
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void onContactSeparate(cocos2d::PhysicsContact& contact);
};

#endif // PHYSICS_MANAGER_H
