// LevelManager.h
#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <vector>

class LevelManager {
public:
    // 关卡状态
   // LevelManager.h
// 修改枚举类 LevelState
   // LevelManager.h
// 修改枚举类 LevelState
    enum class LevelState {
        LEVEL1,      // 第一关
        LEVEL2_1,    // 第二关第1张图
        LEVEL2_2,    // 第二关第2张图
        LEVEL2_3,    // 第二关第3张图
        LEVEL2_4,    // 第二关第4张图
        LEVEL2_5,    // 第二关第5张图
        LEVEL2_6,    // 第二关第6张图
        LEVEL3_1,    // 第三关第1张图
        LEVEL3_2,    // 第三关第2张图
        LEVEL3_3,    // 第三关第3张图
        LEVEL3_4,    // 第三关第4张图
        LEVEL3_5,    // 第三关第5张图
        LEVEL3_6,    // 第三关第6张图
        LEVEL4_1,    // 第四关第1张图（新增）
        LEVEL4_2,    // 第四关第2张图（新增）
        LEVEL4_3,    // 第四关第3张图（新增）
        LEVEL4_4,    // 第四关第4张图（新增）
        LEVEL4_5,    // 第四关第5张图（新增）
        LEVEL4_6,    // 第四关第6张图（新增）
        COMPLETED    // 所有关卡完成
    };

    static LevelManager* getInstance();

    // 关卡管理
    LevelState getCurrentLevel() const { return _currentLevel; }
    void setCurrentLevel(LevelState level);

    // 检查是否可以切换到下一关卡
    bool canSwitchToNextLevel(float playerWorldX);

    // 切换到下一关卡
    LevelState switchToNextLevel();

    // 重置关卡
    void resetLevels();

    // 获取当前关卡的背景图片列表
    std::vector<std::string> getCurrentLevelBackgrounds() const;

    // 获取当前关卡的世界宽度（所有背景图总宽度）
    float getCurrentLevelWorldWidth() const;

    // 获取当前关卡的最大世界位置（玩家不能超过的位置）
    float getCurrentLevelMaxWorldX() const;

    // 检查玩家是否到达当前关卡边界
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
        float worldWidth;  // 实际宽度会在加载后计算
    };

    std::vector<LevelConfig> _levelConfigs;

    // 初始化关卡配置
    void initLevelConfigs();
};

#endif // LEVELMANAGER_H