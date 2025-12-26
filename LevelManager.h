// LevelManager.h - 修复编码并添加缺少的枚举
#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <vector>

class LevelManager {
public:
    // 关卡状态枚举
    enum class LevelState {
        START_SCENE,     // 开始场景
        LEVEL1,          // 关卡1
        LEVEL2_1,        // 关卡2-1
        LEVEL2_2,        // 关卡2-2
        LEVEL2_3,        // 关卡2-3
        LEVEL2_4,        // 关卡2-4
        LEVEL2_5,        // 关卡2-5
        LEVEL2_6,        // 关卡2-6
        LEVEL3_1,        // 关卡3-1
        LEVEL3_2,        // 关卡3-2
        LEVEL3_3,        // 关卡3-3
        LEVEL3_4,        // 关卡3-4
        LEVEL3_5,        // 关卡3-5
        LEVEL3_6,        // 关卡3-6
        LEVEL4_1,        // 关卡4-1
        LEVEL4_2,        // 关卡4-2
        LEVEL4_3,        // 关卡4-3
        LEVEL4_4,        // 关卡4-4
        LEVEL4_5,        // 关卡4-5
        LEVEL4_6,        // 关卡4-6
        PAUSE_SCENE,     // 暂停场景
        GAMEOVER_SCENE,  // 游戏结束场景
        FINAL_LEVEL,     // 最终关卡
        COMPLETED        // 游戏完成
    };

    static LevelManager* getInstance();

    // 获取当前关卡
    LevelState getCurrentLevel() const { return _currentLevel; }
    void setCurrentLevel(LevelState level);

    // 检查是否可以切换到下一关
    bool canSwitchToNextLevel(float playerWorldX);

    // 切换到下一关
    LevelState switchToNextLevel();

    // 重置关卡
    void resetLevels();

    // 获取当前关卡的背景图片
    std::vector<std::string> getCurrentLevelBackgrounds() const;

    // 获取当前关卡的世界宽度
    float getCurrentLevelWorldWidth() const;

    // 获取当前关卡的最大世界X坐标
    float getCurrentLevelMaxWorldX() const;

    // 检查玩家是否在关卡边界
    bool isPlayerAtLevelBoundary(float playerWorldX) const;

private:
    LevelManager();
    ~LevelManager() = default;

    static LevelManager* _instance;
    LevelState _currentLevel;

    // 关卡配置
    struct LevelConfig {
        LevelState level;
        std::string name;
        std::vector<std::string> backgroundImages;
        float worldWidth;  // 可选的：关卡世界宽度
    };

    std::vector<LevelConfig> _levelConfigs;

    // 初始化关卡配置
    void initLevelConfigs();
};

#endif // LEVELMANAGER_H