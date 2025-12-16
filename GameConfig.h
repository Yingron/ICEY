// GameConfig.h
#pragma once
#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// 游戏配置
namespace GameConfig {
    // 窗口设置
    const int WINDOW_WIDTH = 2560;
    const int WINDOW_HEIGHT = 1440;

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
        const float ATTACK_FRAME_DELAY = 0.01f; // 攻击动画帧延迟
        const float JUMP_FRAME_DELAY = 0.06f;  // 跳跃动画帧延迟 - 新增
    }

    // 输入设置
    namespace Input {
        const char MOVE_LEFT = 'A';
        const char MOVE_RIGHT = 'D';
        const char JUMP = 'W';           // 跳跃键 - 新增
        const char ATTACK = 'J';
        const char NORMAL_ATTACK = 'K';
        const char SKILL1 = 'Q';
        const char SKILL2 = 'E';
        const char DASH = ' ';
    }

    // 冲刺设置
    namespace Dash {
        const float DASH_SPEED = 1500.0f;      // 冲刺速度
        const float DASH_DISTANCE = 300.0f;    // 冲刺距离
        const float DASH_DURATION = 0.2f;      // 冲刺持续时间
        const float DASH_COOLDOWN = 0.5f;      // 冲刺冷却时间
        const float DASH_FRAME_DELAY = 0.03f;  // 冲刺动画帧延迟
    }

    // 连击设置
    namespace Combo {
        const float COMBO_WINDOW = 0.3f;       // 连击时间窗口
        const int MAX_COMBO = 3;               // 最大连击数
    }
}

#endif // GAMECONFIG_H