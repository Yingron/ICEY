#include "c:/test3/Classes/PhysicsConfig.h"
#include <iostream>

int main() {
    std::cout << "PhysicsConfig.h test:" << std::endl;
    std::cout << "COLLISION_CATEGORY_PLAYER: " << COLLISION_CATEGORY_PLAYER << std::endl;
    std::cout << "COLLISION_CATEGORY_ENEMY: " << COLLISION_CATEGORY_ENEMY << std::endl;
    std::cout << "COLLISION_CATEGORY_GROUND: " << COLLISION_CATEGORY_GROUND << std::endl;
    std::cout << "COLLISION_CATEGORY_BOUNDARY: " << COLLISION_CATEGORY_BOUNDARY << std::endl;
    std::cout << "COLLISION_MASK_PLAYER: " << COLLISION_MASK_PLAYER << std::endl;
    std::cout << "COLLISION_MASK_ENEMY: " << COLLISION_MASK_ENEMY << std::endl;
    std::cout << "All macros defined successfully!" << std::endl;
    return 0;
}