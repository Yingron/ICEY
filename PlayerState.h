#pragma once
// PlayerState.h
#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

enum class PlayerState {
    IDLE,
    WALKING,
    RUNNING,
    JUMPING,
    ATTACKING,
    DASHING,
    SKILL1,
    SKILL2,
    HURT,
    DEAD
};

#endif // PLAYERSTATE_H