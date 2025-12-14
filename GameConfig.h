// GameConfig.h
#pragma once
#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// 游戏配置
namespace GameConfig {
    // 窗口设置
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // 角色设置
    const float PLAYER_SPEED = 400.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = 600.0f;      // 跳跃力量
    const float PLAYER_GROUND_Y = 0.0f;   // 地面高度
    // 玩家地面位置（在屏幕上的Y坐标百分比）
    const float PLAYER_GROUND_Y_PERCENT = 0.3f; // 从 0.2f 改为 0.3f

     // 动画设置
    namespace Animation {
        const float RUN_FRAME_DELAY = 0.05f;   // 跑动动画帧延迟
        const float IDLE_FRAME_DELAY = 0.06f;  // 待机动画帧延迟
        const float ATTACK_FRAME_DELAY = 0.06f; // 攻击动画帧延迟
        const float JUMP_FRAME_DELAY = 0.06f;  // 跳跃动画帧延迟 - 新增
    }

    // 输入设置
    namespace Input {
        const char MOVE_LEFT = 'A';
        const char MOVE_RIGHT = 'D';
        const char JUMP = 'W';           // 跳跃键 - 新增
        const char ATTACK = 'J';
        const char NORMAL_ATTACK = 'K';
        const char SKILL1 = 'K';
        const char SKILL2 = 'L';
        const char DASH = ' ';
    }
}

#endif // GAMECONFIG_H