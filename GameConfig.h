#pragma once
#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include <string>

namespace GameConfig
{
    // 窗口设置
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const std::string WINDOW_TITLE = "ICEY Clone";

    // 物理参数
    const float GRAVITY = -980.0f;
    const float PLAYER_JUMP_FORCE = 450.0f;
    const float PLAYER_MOVE_SPEED = 300.0f;

    // 游戏参数
    const float DODGE_INVINCIBLE_TIME = 0.5f;
    const float COMBO_TIMEOUT = 2.0f;

    // 资源路径
    const std::string TEXTURE_PATH = "Textures/";
    const std::string SOUND_PATH = "Sounds/";
    const std::string FONT_PATH = "Fonts/";
}

#endif // __GAME_CONFIG_H__