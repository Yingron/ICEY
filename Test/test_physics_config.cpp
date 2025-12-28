#include "PhysicsConfig.h"
#include <iostream>

int main() {
    // 测试物理碰撞常量是否正确定义
    std::cout << "COLLISION_CATEGORY_PLAYER: " << COLLISION_CATEGORY_PLAYER << std::endl;
    std::cout << "COLLISION_CATEGORY_ENEMY: " << COLLISION_CATEGORY_ENEMY << std::endl;
    std::cout << "COLLISION_CATEGORY_GROUND: " << COLLISION_CATEGORY_GROUND << std::endl;
    std::cout << "COLLISION_CATEGORY_BOUNDARY: " << COLLISION_CATEGORY_BOUNDARY << std::endl;
    
    std::cout << "COLLISION_MASK_PLAYER: " << COLLISION_MASK_PLAYER << std::endl;
    std::cout << "COLLISION_MASK_ENEMY: " << COLLISION_MASK_ENEMY << std::endl;
    std::cout << "COLLISION_MASK_GROUND: " << COLLISION_MASK_GROUND << std::endl;
    std::cout << "COLLISION_MASK_BOUNDARY: " << COLLISION_MASK_BOUNDARY << std::endl;
    
    return 0;
}