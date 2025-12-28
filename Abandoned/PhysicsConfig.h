#ifndef PHYSICSCONFIG_H
#define PHYSICSCONFIG_H

#include "cocos2d.h"

USING_NS_CC;

// 鐗╃悊纰版挒绫诲埆鏋氫妇 - 浣跨敤浣嶆帺鐮?#define COLLISION_CATEGORY_PLAYER  0x01    // 鐜╁绫诲埆
#define COLLISION_CATEGORY_ENEMY   0x02    // 鏁屼汉绫诲埆
#define COLLISION_CATEGORY_ITEM    0x04    // 鐗╁搧绫诲埆
#define COLLISION_CATEGORY_GROUND  0x08    // 鍦伴潰绫诲埆
#define COLLISION_CATEGORY_BOUNDARY 0x10   // 灞忓箷杈圭晫绫诲埆
#define COLLISION_CATEGORY_PROJECTILE 0x20 // 鎶曞皠鐗╃被鍒?
// 鐗╃悊纰版挒鎺╃爜
#define COLLISION_MASK_PLAYER      (COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_GROUND | COLLISION_CATEGORY_ITEM | COLLISION_CATEGORY_PROJECTILE)
#define COLLISION_MASK_ENEMY       (COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_GROUND | COLLISION_CATEGORY_BOUNDARY)
#define COLLISION_MASK_ITEM        (COLLISION_CATEGORY_PLAYER)
#define COLLISION_MASK_GROUND      (COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY)
#define COLLISION_MASK_BOUNDARY    (COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_PROJECTILE)
#define COLLISION_MASK_PROJECTILE  (COLLISION_CATEGORY_PLAYER | COLLISION_CATEGORY_ENEMY | COLLISION_CATEGORY_BOUNDARY)

// 鐗╃悊鏉愯川鍙傛暟
namespace PhysicsMaterialParams {
    // 鐜╁鐗╃悊鏉愯川
    const float PLAYER_DENSITY = 1.0f;
    const float PLAYER_RESTITUTION = 0.1f; // 浣庡脊鎬э紝鍑忓皯寮硅烦
    const float PLAYER_FRICTION = 0.8f;     // 閫備腑鎽╂摝鍔?    
    // 鏁屼汉鐗╃悊鏉愯川
    const float ENEMY_DENSITY = 1.0f;
    const float ENEMY_RESTITUTION = 0.1f;
    const float ENEMY_FRICTION = 0.5f;
    
    // 鍦伴潰鐗╃悊鏉愯川
    const float GROUND_DENSITY = 0.0f;      // 闈欐€佺墿浣撳瘑搴︿负0
    const float GROUND_RESTITUTION = 0.0f;
    const float GROUND_FRICTION = 1.0f;
    
    // 灞忓箷杈圭晫鐗╃悊鏉愯川
    const float BOUNDARY_DENSITY = 0.0f;
    const float BOUNDARY_RESTITUTION = 0.5f;
    const float BOUNDARY_FRICTION = 0.0f;
}

// 纰版挒鍝嶅簲鍙傛暟
namespace CollisionResponseParams {
    // 鍑婚€€鍔涘害
    const float PLAYER_KNOCKBACK_POWER = 500.0f;
    const float ENEMY_KNOCKBACK_POWER = 300.0f;
    
    // 鏃犳晫鐘舵€佸弬鏁?    const float INVINCIBLE_DURATION = 1.0f;   // 鍙椾激鍚庢棤鏁屾椂闂达紙绉掞級
    const float INVINCIBLE_FLASH_INTERVAL = 0.1f; // 鏃犳晫鐘舵€侀棯鐑侀棿闅?    
    // 鏁屼汉鍒嗙鍙傛暟
    const float ENEMY_SEPARATION_FORCE = 100.0f;
    
    // 纰版挒妫€娴嬬伒鏁忓害
    const float COLLISION_TOLERANCE = 0.1f;
}

// 鐗╃悊涓栫晫鍙傛暟
namespace PhysicsWorldParams {
    const float GRAVITY_Y = -500.0f;
    const float VELOCITY_ITERATIONS = 8;
    const float POSITION_ITERATIONS = 3;
}

#endif // PHYSICSCONFIG_H