#pragma once
#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

enum class PlayerState
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    ATTACK,
    DODGE,
    HURT,
    DEAD
};

enum class PlayerAction
{
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP,
    ATTACK_LIGHT,
    ATTACK_HEAVY,
    DODGE,
    SKILL_1,
    SKILL_2
};

#endif // __PLAYER_STATE_H__