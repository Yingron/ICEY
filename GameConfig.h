#pragma once
// GameConfig.h
#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// 游戏常量配置
namespace GameConfig {
    // 窗口尺寸
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // 玩家配置
    const float PLAYER_SPEED = 400.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = 600.0f;

    // 动画配置
    namespace Animation {
        const float RUN_FRAME_DELAY = 0.05f;  // 跑动动画每帧时间（秒）
        const float IDLE_FRAME_DELAY = 0.1f;  // 待机动画每帧时间
        const float ATTACK_FRAME_DELAY = 0.06f; // 攻击动画每帧时间
    }

    // 输入键位
    namespace Input {
        const char MOVE_LEFT = 'A';
        const char MOVE_RIGHT = 'D';
        const char JUMP = 'W';
        const char ATTACK = 'J';
        const char SKILL1 = 'K';
        const char SKILL2 = 'L';
        const char DASH = ' ';
    }
}

#endif // GAMECONFIG_H