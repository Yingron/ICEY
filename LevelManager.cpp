// LevelManager.cpp
#include "LevelManager.h"
#include "cocos2d.h"

USING_NS_CC;

LevelManager* LevelManager::_instance = nullptr;

LevelManager* LevelManager::getInstance() {
    if (!_instance) {
        _instance = new LevelManager();
    }
    return _instance;
}

LevelManager::LevelManager() {
    _currentLevel = LevelState::LEVEL1;
    initLevelConfigs();
}

// LevelManager.cpp - 修改 initLevelConfigs 函数
void LevelManager::initLevelConfigs() {
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

    // Level2-1 到 Level2-5（现有配置保持不变）
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

    // Level2-6: 第二关第6张图（新增）
    LevelConfig level2_6;
    level2_6.level = LevelState::LEVEL2_6;
    level2_6.name = "Level 2-6";
    level2_6.backgroundImages = { "background-level2-6.png" };
    _levelConfigs.push_back(level2_6);

    // Level3-1: 第三关第1张图（新增）
    LevelConfig level3_1;
    level3_1.level = LevelState::LEVEL3_1;
    level3_1.name = "Level 3-1";
    level3_1.backgroundImages = { "background-level3-1.png" };
    _levelConfigs.push_back(level3_1);

    // Level3-2: 第三关第2张图（新增）
    LevelConfig level3_2;
    level3_2.level = LevelState::LEVEL3_2;
    level3_2.name = "Level 3-2";
    level3_2.backgroundImages = { "background-level3-2.png" };
    _levelConfigs.push_back(level3_2);

    // Level3-3: 第三关第3张图（新增）
    LevelConfig level3_3;
    level3_3.level = LevelState::LEVEL3_3;
    level3_3.name = "Level 3-3";
    level3_3.backgroundImages = { "background-level3-3.png" };
    _levelConfigs.push_back(level3_3);

    // Level3-4: 第三关第4张图（新增）
    LevelConfig level3_4;
    level3_4.level = LevelState::LEVEL3_4;
    level3_4.name = "Level 3-4";
    level3_4.backgroundImages = { "background-level3-4.png" };
    _levelConfigs.push_back(level3_4);

    // Level3-5: 第三关第5张图（新增）
    LevelConfig level3_5;
    level3_5.level = LevelState::LEVEL3_5;
    level3_5.name = "Level 3-5";
    level3_5.backgroundImages = { "background-level3-5.png" };
    _levelConfigs.push_back(level3_5);

    // Level3-6: 第三关第6张图（新增）
    LevelConfig level3_6;
    level3_6.level = LevelState::LEVEL3_6;
    level3_6.name = "Level 3-6";
    level3_6.backgroundImages = { "background-level3-6.png" };
    _levelConfigs.push_back(level3_6);

    // Level4-1: 第四关第1张图（新增）
    LevelConfig level4_1;
    level4_1.level = LevelState::LEVEL4_1;
    level4_1.name = "Level 4-1";
    level4_1.backgroundImages = { "background-level4-1.png" };
    _levelConfigs.push_back(level4_1);

    // Level4-2: 第四关第2张图（新增）
    LevelConfig level4_2;
    level4_2.level = LevelState::LEVEL4_2;
    level4_2.name = "Level 4-2";
    level4_2.backgroundImages = { "background-level4-2.png" };
    _levelConfigs.push_back(level4_2);

    // Level4-3: 第四关第3张图（新增）
    LevelConfig level4_3;
    level4_3.level = LevelState::LEVEL4_3;
    level4_3.name = "Level 4-3";
    level4_3.backgroundImages = { "background-level4-3.png" };
    _levelConfigs.push_back(level4_3);

    // Level4-4: 第四关第4张图（新增）
    LevelConfig level4_4;
    level4_4.level = LevelState::LEVEL4_4;
    level4_4.name = "Level 4-4";
    level4_4.backgroundImages = { "background-level4-4.png" };
    _levelConfigs.push_back(level4_4);

    // Level4-5: 第四关第5张图（新增）
    LevelConfig level4_5;
    level4_5.level = LevelState::LEVEL4_5;
    level4_5.name = "Level 4-5";
    level4_5.backgroundImages = { "background-level4-5.png" };
    _levelConfigs.push_back(level4_5);

    // Level4-6: 第四关第6张图（新增）
    LevelConfig level4_6;
    level4_6.level = LevelState::LEVEL4_6;
    level4_6.name = "Level 4-6";
    level4_6.backgroundImages = { "background-level4-6.png" };
    _levelConfigs.push_back(level4_6);
}

bool LevelManager::canSwitchToNextLevel(float playerWorldX) {
    float maxWorldX = getCurrentLevelMaxWorldX();

    // 玩家到达当前关卡边界时，可以切换到下一关卡
    // 使用更宽松的条件：距离边界100像素时就可以切换
    if (playerWorldX >= maxWorldX - 200.0f) {
        // 检查是否可以切换到下一关卡（不是最后一个关卡）
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
            nextLevel = LevelState::LEVEL4_1;  // 新增：从Level3-6切换到Level4-1
            break;
        case LevelState::LEVEL4_1:
            nextLevel = LevelState::LEVEL4_2;  // 新增
            break;
        case LevelState::LEVEL4_2:
            nextLevel = LevelState::LEVEL4_3;  // 新增
            break;
        case LevelState::LEVEL4_3:
            nextLevel = LevelState::LEVEL4_4;  // 新增
            break;
        case LevelState::LEVEL4_4:
            nextLevel = LevelState::LEVEL4_5;  // 新增
            break;
        case LevelState::LEVEL4_5:
            nextLevel = LevelState::LEVEL4_6;  // 新增
            break;
        case LevelState::LEVEL4_6:
            nextLevel = LevelState::COMPLETED;  // Level4-6是最后一关
            break;
        default:
            return false; // 已经是最后一个关卡
        }

        // 如果不是COMPLETED状态，就可以切换
        return (nextLevel != LevelState::COMPLETED);
    }

    return false;
}

// LevelManager.cpp - 修改 switchToNextLevel 函数
LevelManager::LevelState LevelManager::switchToNextLevel() {
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
        _currentLevel = LevelState::LEVEL4_1;  // 新增
        break;
    case LevelState::LEVEL4_1:
        _currentLevel = LevelState::LEVEL4_2;  // 新增
        break;
    case LevelState::LEVEL4_2:
        _currentLevel = LevelState::LEVEL4_3;  // 新增
        break;
    case LevelState::LEVEL4_3:
        _currentLevel = LevelState::LEVEL4_4;  // 新增
        break;
    case LevelState::LEVEL4_4:
        _currentLevel = LevelState::LEVEL4_5;  // 新增
        break;
    case LevelState::LEVEL4_5:
        _currentLevel = LevelState::LEVEL4_6;  // 新增
        break;
    case LevelState::LEVEL4_6:
        _currentLevel = LevelState::COMPLETED;  // 新增
        break;
    default:
        break;
    }

    return _currentLevel;
}

void LevelManager::resetLevels() {
    _currentLevel = LevelState::LEVEL1;
}

std::vector<std::string> LevelManager::getCurrentLevelBackgrounds() const {
    for (const auto& config : _levelConfigs) {
        if (config.level == _currentLevel) {
            return config.backgroundImages;
        }
    }

    // 默认返回第一关
    return _levelConfigs[0].backgroundImages;
}

float LevelManager::getCurrentLevelWorldWidth() const {
    // 这个值需要在MainGameScene中根据实际背景图计算
    // 这里返回一个估计值，实际值在加载背景后确定
    switch (_currentLevel) {
    case LevelState::LEVEL1:
        return 5 * 1280.0f; // 假设每张背景图宽度等于屏幕宽度
    case LevelState::LEVEL2_1:
    case LevelState::LEVEL2_2:
    case LevelState::LEVEL2_3:
    case LevelState::LEVEL2_4:
    case LevelState::LEVEL2_5:
        return 1280.0f; // Level2每张图宽度等于屏幕宽度
    default:
        return 1280.0f;
    }
}

float LevelManager::getCurrentLevelMaxWorldX() const {
    return getCurrentLevelWorldWidth();
}

bool LevelManager::isPlayerAtLevelBoundary(float playerWorldX) const {
    float maxWorldX = getCurrentLevelMaxWorldX();
    return playerWorldX >= maxWorldX - 200.0f; // 从100增加到200像素
}