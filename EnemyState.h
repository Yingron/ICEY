#pragma once
#ifndef __ENEMY_STATE_H__
#define __ENEMY_STATE_H__

enum class EnemyState
{
    IDLE,
    PATROL,
    CHASE,
    ATTACK,
    HURT,
    DEAD
};

enum class EnemyType
{
    MELEE,
    RANGED,
    ELITE
};

#endif // __ENEMY_STATE_H__