// LevelManager.cpp
#include "LevelManager.h"
#include "AudioManager.h"
#include "cocos2d.h"
#include "LevelMusicManager.h"

USING_NS_CC;

LevelManager* LevelManager::_instance = nullptr;

LevelManager* LevelManager::getInstance() {
    if (!_instance) {
        _instance = new LevelManager();
    }
    return _instance;
}

// 修正后的构造函数 - 只保留一个
LevelManager::LevelManager() {
    _currentLevel = LevelState::LEVEL1;

    // 初始化关卡配置
    initLevelConfigs();

    // 播放初始关卡的背景音乐
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager) {
        musicManager->playBGMForLevel(_currentLevel);
    }
    else {
        // 备用方案：如果 LevelMusicManager 不可用，使用 AudioManager
        AudioManager::getInstance()->playBGM("bgm_level1");
    }
}

void LevelManager::initLevelConfigs() {
    // 清理现有配置
    _levelConfigs.clear();

    // Level1: 5张背景图
    LevelConfig level1;
    level1.level = LevelState::LEVEL1;
    level1.name = "Level 1";
    level1.backgroundImages = {
        "background-level1-1.png",
        "background-level1-2.png",
        "background-level1-3.png",
        "background-level1-4.png",
        "background-level1-5.png"
    };
    _levelConfigs.push_back(level1);

    // Level2-1 到 Level2-6
    LevelConfig level2_1;
    level2_1.level = LevelState::LEVEL2_1;
    level2_1.name = "Level 2-1";
    level2_1.backgroundImages = { "background-level2-1.png" };
    _levelConfigs.push_back(level2_1);

    LevelConfig level2_2;
    level2_2.level = LevelState::LEVEL2_2;
    level2_2.name = "Level 2-2";
    level2_2.backgroundImages = { "background-level2-2.png" };
    _levelConfigs.push_back(level2_2);

    LevelConfig level2_3;
    level2_3.level = LevelState::LEVEL2_3;
    level2_3.name = "Level 2-3";
    level2_3.backgroundImages = { "background-level2-3.png" };
    _levelConfigs.push_back(level2_3);

    LevelConfig level2_4;
    level2_4.level = LevelState::LEVEL2_4;
    level2_4.name = "Level 2-4";
    level2_4.backgroundImages = { "background-level2-4.png" };
    _levelConfigs.push_back(level2_4);

    LevelConfig level2_5;
    level2_5.level = LevelState::LEVEL2_5;
    level2_5.name = "Level 2-5";
    level2_5.backgroundImages = { "background-level2-5.png" };
    _levelConfigs.push_back(level2_5);

    // Level2-6
    LevelConfig level2_6;
    level2_6.level = LevelState::LEVEL2_6;
    level2_6.name = "Level 2-6";
    level2_6.backgroundImages = { "background-level2-6.png" };
    _levelConfigs.push_back(level2_6);

    // Level3-1
    LevelConfig level3_1;
    level3_1.level = LevelState::LEVEL3_1;
    level3_1.name = "Level 3-1";
    level3_1.backgroundImages = {
        "background-level3-1-1.png",
        "background-level3-1-2.png",
        "background-level3-1-3.png",
        "background-level3-1-4.png",
        "background-level3-1-5.png"
    };
    _levelConfigs.push_back(level3_1);

    // Level3-2
    LevelConfig level3_2;
    level3_2.level = LevelState::LEVEL3_2;
    level3_2.name = "Level 3-2";
    level3_2.backgroundImages = {
        "background-level3-2-1.png",
        "background-level3-2-2.png",
        "background-level3-2-3.png"
    };
    _levelConfigs.push_back(level3_2);

    // Level3-3
    LevelConfig level3_3;
    level3_3.level = LevelState::LEVEL3_3;
    level3_3.name = "Level 3-3";
    level3_3.backgroundImages = { "background-level3-3.png" };
    _levelConfigs.push_back(level3_3);

    // Level3-4
    LevelConfig level3_4;
    level3_4.level = LevelState::LEVEL3_4;
    level3_4.name = "Level 3-4";
    level3_4.backgroundImages = { "background-level3-4.png" };
    _levelConfigs.push_back(level3_4);

    // Level3-5
    LevelConfig level3_5;
    level3_5.level = LevelState::LEVEL3_5;
    level3_5.name = "Level 3-5";
    level3_5.backgroundImages = { "background-level3-5.png" };
    _levelConfigs.push_back(level3_5);

    // Level3-6
    LevelConfig level3_6;
    level3_6.level = LevelState::LEVEL3_6;
    level3_6.name = "Level 3-6";
    level3_6.backgroundImages = { "background-level3-6.png" };
    _levelConfigs.push_back(level3_6);

    // Level4-1
    LevelConfig level4_1;
    level4_1.level = LevelState::LEVEL4_1;
    level4_1.name = "Level 4-1";
    level4_1.backgroundImages = { "background-level4-1.png" };
    _levelConfigs.push_back(level4_1);

    // Level4-2
    LevelConfig level4_2;
    level4_2.level = LevelState::LEVEL4_2;
    level4_2.name = "Level 4-2";
    level4_2.backgroundImages = { "background-level4-2.png" };
    _levelConfigs.push_back(level4_2);

    // Level4-3
    LevelConfig level4_3;
    level4_3.level = LevelState::LEVEL4_3;
    level4_3.name = "Level 4-3";
    level4_3.backgroundImages = {
        "background-level4-3-1.png",
        "background-level4-3-2.png",
        "background-level4-3-3.png"
    };
    _levelConfigs.push_back(level4_3);

    // Level4-4
    LevelConfig level4_4;
    level4_4.level = LevelState::LEVEL4_4;
    level4_4.name = "Level 4-4";
    level4_4.backgroundImages = { "background-level4-4.png" };
    _levelConfigs.push_back(level4_4);

    // Level4-5
    LevelConfig level4_5;
    level4_5.level = LevelState::LEVEL4_5;
    level4_5.name = "Level 4-5";
    level4_5.backgroundImages = {
        "background-level4-5-1.png",
        "background-level4-5-2.png",
        "background-level4-5-3.png"
    };
    _levelConfigs.push_back(level4_5);

    // Level4-6
    LevelConfig level4_6;
    level4_6.level = LevelState::LEVEL4_6;
    level4_6.name = "Level 4-6";
    level4_6.backgroundImages = {
        "background-level4-6-1.png",
        "background-level4-6-2.png",
        "background-level4-6-3.png"
    };
    _levelConfigs.push_back(level4_6);

    // Final Level
    LevelConfig finalLevel;
    finalLevel.level = LevelState::FINAL_LEVEL;
    finalLevel.name = "Final Level";
    finalLevel.backgroundImages = {
        "images/environment/background/final-level-4.png",
        "images/environment/background/final-level-4.png",
        "images/environment/background/final-level-4.png",
        "images/environment/background/final-level-4.png"
    };
    _levelConfigs.push_back(finalLevel);
}

bool LevelManager::canSwitchToNextLevel(float playerWorldX) {
    float maxWorldX = getCurrentLevelMaxWorldX();

    // 玩家到达当前关卡边界时，可以切换到下一关卡
    if (playerWorldX >= maxWorldX - 10.0f) {
        LevelState nextLevel = _currentLevel;
        switch (_currentLevel) {
        case LevelState::LEVEL1:
            nextLevel = LevelState::LEVEL2_1;
            break;
        case LevelState::LEVEL2_1:
            nextLevel = LevelState::LEVEL2_2;
            break;
        case LevelState::LEVEL2_2:
            nextLevel = LevelState::LEVEL2_3;
            break;
        case LevelState::LEVEL2_3:
            nextLevel = LevelState::LEVEL2_4;
            break;
        case LevelState::LEVEL2_4:
            nextLevel = LevelState::LEVEL2_5;
            break;
        case LevelState::LEVEL2_5:
            nextLevel = LevelState::LEVEL2_6;
            break;
        case LevelState::LEVEL2_6:
            nextLevel = LevelState::LEVEL3_1;
            break;
        case LevelState::LEVEL3_1:
            nextLevel = LevelState::LEVEL3_2;
            break;
        case LevelState::LEVEL3_2:
            nextLevel = LevelState::LEVEL3_3;
            break;
        case LevelState::LEVEL3_3:
            nextLevel = LevelState::LEVEL3_4;
            break;
        case LevelState::LEVEL3_4:
            nextLevel = LevelState::LEVEL3_5;
            break;
        case LevelState::LEVEL3_5:
            nextLevel = LevelState::LEVEL3_6;
            break;
        case LevelState::LEVEL3_6:
            nextLevel = LevelState::LEVEL4_1;
            break;
        case LevelState::LEVEL4_1:
            nextLevel = LevelState::LEVEL4_2;
            break;
        case LevelState::LEVEL4_2:
            nextLevel = LevelState::LEVEL4_3;
            break;
        case LevelState::LEVEL4_3:
            nextLevel = LevelState::LEVEL4_4;
            break;
        case LevelState::LEVEL4_4:
            nextLevel = LevelState::LEVEL4_5;
            break;
        case LevelState::LEVEL4_5:
            nextLevel = LevelState::LEVEL4_6;
            break;
        case LevelState::LEVEL4_6:
            nextLevel = LevelState::FINAL_LEVEL;
            break;
        case LevelState::FINAL_LEVEL:
            nextLevel = LevelState::COMPLETED;
            break;
        default:
            return false;
        }

        return (nextLevel != LevelState::COMPLETED);
    }

    return false;
}

LevelManager::LevelState LevelManager::switchToNextLevel() {
    auto oldLevel = _currentLevel;

    switch (_currentLevel) {
    case LevelState::LEVEL1:
        _currentLevel = LevelState::LEVEL2_1;
        break;
    case LevelState::LEVEL2_1:
        _currentLevel = LevelState::LEVEL2_2;
        break;
    case LevelState::LEVEL2_2:
        _currentLevel = LevelState::LEVEL2_3;
        break;
    case LevelState::LEVEL2_3:
        _currentLevel = LevelState::LEVEL2_4;
        break;
    case LevelState::LEVEL2_4:
        _currentLevel = LevelState::LEVEL2_5;
        break;
    case LevelState::LEVEL2_5:
        _currentLevel = LevelState::LEVEL2_6;
        break;
    case LevelState::LEVEL2_6:
        _currentLevel = LevelState::LEVEL3_1;
        break;
    case LevelState::LEVEL3_1:
        _currentLevel = LevelState::LEVEL3_2;
        break;
    case LevelState::LEVEL3_2:
        _currentLevel = LevelState::LEVEL3_3;
        break;
    case LevelState::LEVEL3_3:
        _currentLevel = LevelState::LEVEL3_4;
        break;
    case LevelState::LEVEL3_4:
        _currentLevel = LevelState::LEVEL3_5;
        break;
    case LevelState::LEVEL3_5:
        _currentLevel = LevelState::LEVEL3_6;
        break;
    case LevelState::LEVEL3_6:
        _currentLevel = LevelState::LEVEL4_1;
        break;
    case LevelState::LEVEL4_1:
        _currentLevel = LevelState::LEVEL4_2;
        break;
    case LevelState::LEVEL4_2:
        _currentLevel = LevelState::LEVEL4_3;
        break;
    case LevelState::LEVEL4_3:
        _currentLevel = LevelState::LEVEL4_4;
        break;
    case LevelState::LEVEL4_4:
        _currentLevel = LevelState::LEVEL4_5;
        break;
    case LevelState::LEVEL4_5:
        _currentLevel = LevelState::LEVEL4_6;
        break;
    case LevelState::LEVEL4_6:
        _currentLevel = LevelState::FINAL_LEVEL;
        break;
    case LevelState::FINAL_LEVEL:
        _currentLevel = LevelState::COMPLETED;
        break;
    default:
        break;
    }

    // 切换音乐
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager && _currentLevel != LevelState::COMPLETED) {
        musicManager->playBGMForLevel(_currentLevel);
    }

    log("Level switched from %d to %d", (int)oldLevel, (int)_currentLevel);

    return _currentLevel;
}

void LevelManager::resetLevels() {
    _currentLevel = LevelState::LEVEL1;

    // 重置音乐
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager) {
        musicManager->playBGMForLevel(_currentLevel);
    }
}

std::vector<std::string> LevelManager::getCurrentLevelBackgrounds() const {
    for (const auto& config : _levelConfigs) {
        if (config.level == _currentLevel) {
            return config.backgroundImages;
        }
    }

    // 默认返回第一个
    return _levelConfigs.empty() ? std::vector<std::string>() : _levelConfigs[0].backgroundImages;
}

float LevelManager::getCurrentLevelWorldWidth() const {
    switch (_currentLevel) {
    case LevelState::LEVEL1:
        return 5 * 1280.0f;
    case LevelState::FINAL_LEVEL:
        return 4 * 1280.0f;
    default:
        return 1280.0f;
    }
}

float LevelManager::getCurrentLevelMaxWorldX() const {
    return getCurrentLevelWorldWidth();
}

bool LevelManager::isPlayerAtLevelBoundary(float playerWorldX) const {
    float maxWorldX = getCurrentLevelMaxWorldX();
    return playerWorldX >= maxWorldX - 200.0f;
}