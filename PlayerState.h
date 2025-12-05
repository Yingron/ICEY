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
    HURT,
    DEAD
};

#endif // PLAYERSTATE_H