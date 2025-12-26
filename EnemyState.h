// EnemyState.h
#pragma once
#ifndef ENEMYSTATE_H
#define ENEMYSTATE_H

enum class EnemyState {
    IDLE,        // Idle
    PATROLLING,  // Patrolling
    CHASING,     // Chasing
    ATTACKING,   // Attacking
    HURT,        // Hurt
    DEAD         // Dead
};

#endif // ENEMYSTATE_H