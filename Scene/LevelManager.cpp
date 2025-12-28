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

// 淇鍚庣殑鏋勯€犲嚱鏁?- 鍙繚鐣欎竴涓?
LevelManager::LevelManager() {
    log("Initializing LevelManager...");
    
    // 锟斤拷始锟斤拷锟截匡拷锟斤拷状态锟斤拷LEVEL1
    _currentLevel = LevelState::LEVEL1;
    _previousLevel = LevelState::LEVEL1; // 鍒濆鍖栦箣鍓嶇殑鍏冲崱涓哄綋鍓嶅叧鍗?
    
    // 锟斤拷始锟斤拷锟节碉拷锟斤拷式锟斤拷默锟斤拷模式
    _currentGameMode = GameMode::DEFAULT;
    
    // 锟斤拷始锟斤拷锟截匡拷锟斤拷贸锟斤拷莶锟斤拷锟?
    initLevelConfigs();
    
    // 锟斤拷始锟斤拷锟截匡拷锟捷诧拷锟斤拷
    log("LevelManager initialized with %zu level configs.", _levelConfigs.size());
    
    // 锟斤拷锟脚讹拷锟斤拷模式锟斤拷锟截猴拷锟斤拷
    AudioManager::getInstance()->playBGM("background_music");
    log("Background music started");
    
    // 鍒濆鍖栨柊澧炴垚鍛樺彉閲?
    _currentBackgroundIndex = 0;
    _triggeredLevel4Completion = false;
}

void LevelManager::initLevelConfigs() {
    // 娓呯悊鐜版湁閰嶇疆
    _levelConfigs.clear();

    // Level1: 5寮犺儗鏅浘
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

    // Level2-1 鍒?Level2-6
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

    // Hidden Level1
    LevelConfig hiddenLevel1;
    hiddenLevel1.level = LevelState::HIDDEN_LEVEL1;
    hiddenLevel1.name = "Hidden Level 1";
    hiddenLevel1.backgroundImages = {
        "background-hidden level1-1.png",
        "background-hidden level1-2.png",
        "background-hidden level1-3.png",
        "background-hidden level1-4.png",
        "background-hidden level1-5.png"
    };
    _levelConfigs.push_back(hiddenLevel1);
}

bool LevelManager::canSwitchToNextLevel(float playerWorldX) {
    float maxWorldX = getCurrentLevelMaxWorldX();

    // 鐜╁鍒拌揪褰撳墠鍏冲崱杈圭晫鏃讹紝鍙互鍒囨崲鍒颁笅涓€鍏冲崱
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
            // 鍦↙evel4-6鐨勬渶鍚庝竴寮犺儗鏅浘澶勶紝鎴戜滑闇€瑕佺壒娈婂鐞嗭紝涓嶇洿鎺ュ垏鎹㈠埌鏈€缁堝叧鍗?
            // 鑰屾槸绛夊緟鐜╁杈撳叆纭
            if (isAtLevel4FinalBackground() && !_triggeredLevel4Completion) {
                return false; // 鏆傛椂闃绘鑷姩鍒囨崲
            }
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

    // 鍒囨崲闊充箰
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager && _currentLevel != LevelState::COMPLETED) {
        musicManager->playBGMForLevel(_currentLevel);
    }

    log("Level switched from %d to %d", (int)oldLevel, (int)_currentLevel);

    return _currentLevel;
}

void LevelManager::switchToHiddenLevel1() {
    auto oldLevel = _currentLevel;
    _currentLevel = LevelState::HIDDEN_LEVEL1;
    
    // 淇濆瓨涔嬪墠鐨勫叧鍗★紝浠ヤ究杩斿洖
    _previousLevel = oldLevel;
    
    // 鍒囨崲闊充箰
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager) {
        musicManager->playBGMForLevel(_currentLevel);
    }
    
    log("Level switched from %d to HIDDEN_LEVEL1", (int)oldLevel);
}

void LevelManager::returnFromHiddenLevel() {
    auto oldLevel = _currentLevel;
    
    // 杩斿洖鍒颁箣鍓嶇殑鍏冲崱锛屽鏋滄病鏈夊垯榛樿杩斿洖LEVEL3_2
    if (_previousLevel != LevelState::COMPLETED) {
        _currentLevel = _previousLevel;
    } else {
        _currentLevel = LevelState::LEVEL3_2;
    }
    
    // 鍒囨崲闊充箰
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager) {
        musicManager->playBGMForLevel(_currentLevel);
    }
    
    log("Level switched from HIDDEN_LEVEL1 to %d", (int)_currentLevel);
}

void LevelManager::resetLevels() {
    _currentLevel = LevelState::LEVEL1;

    // 閲嶇疆闊充箰
    auto musicManager = LevelMusicManager::getInstance();
    if (musicManager) {
        musicManager->playBGMForLevel(_currentLevel);
    }
    
    // 閲嶇疆鑳屾櫙绱㈠紩鍜岃Е鍙戠姸鎬?
    _currentBackgroundIndex = 0;
    _triggeredLevel4Completion = false;
}

std::vector<std::string> LevelManager::getCurrentLevelBackgrounds() const {
    for (const auto& config : _levelConfigs) {
        if (config.level == _currentLevel) {
            return config.backgroundImages;
        }
    }

    // 榛樿杩斿洖绗竴涓?
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

// 锟斤拷锟矫筹拷锟斤拷诘锟斤拷锟绞?
void LevelManager::setCurrentGameMode(GameMode mode) {
    _currentGameMode = mode;
    log("Game mode set to %s", (mode == GameMode::DEFAULT ? "DEFAULT" : "EASY"));
}

// 锟斤拷转锟斤拷锟节碉拷锟斤拷式
void LevelManager::toggleGameMode() {
    _currentGameMode = (_currentGameMode == GameMode::DEFAULT) ? GameMode::EASY : GameMode::DEFAULT;
    log("Game mode toggled to %s", (_currentGameMode == GameMode::DEFAULT ? "DEFAULT" : "EASY"));
}

// 璁剧疆褰撳墠鏄剧ず鐨勮儗鏅浘鐗囩储寮?
void LevelManager::setCurrentBackgroundIndex(int index) {
    _currentBackgroundIndex = index;
    log("Current background index set to %d", index);
}

// 鑾峰彇褰撳墠鏄剧ず鐨勮儗鏅浘鐗囩储寮?
int LevelManager::getCurrentBackgroundIndex() const {
    return _currentBackgroundIndex;
}

// 妫€鏌ユ槸鍚﹀埌杈綥evel4-6鐨勬渶鍚庝竴寮犺儗鏅浘(background-level4-6-3.png)
bool LevelManager::isAtLevel4FinalBackground() const {
    // 妫€鏌ユ槸鍚﹀湪Level4-6
    if (_currentLevel != LevelState::LEVEL4_6) {
        return false;
    }
    
    // 妫€鏌ユ槸鍚︽槸鏈€鍚庝竴寮犺儗鏅浘 (绱㈠紩涓?)
    return _currentBackgroundIndex == 2;
}

// 鏍囪鏄惁宸茶Е鍙慙evel4缁撴潫澶勭悊
bool LevelManager::hasTriggeredLevel4Completion() const {
    return _triggeredLevel4Completion;
}

void LevelManager::setTriggeredLevel4Completion(bool triggered) {
    _triggeredLevel4Completion = triggered;
    log("Level4 completion trigger set to %s", triggered ? "true" : "false");
}

// 鑾峰彇褰撳墠鍏冲崱鐨勫畬鏁撮厤缃?
LevelManager::LevelConfig LevelManager::getCurrentLevelConfig() const {
    LevelConfig result;
    
    for (const auto& config : _levelConfigs) {
        if (config.level == _currentLevel) {
            result = config;
            break;
        }
    }
    
    // 濡傛灉娌℃湁鎵惧埌閰嶇疆锛屼娇鐢ㄧ涓€涓厤缃垨鍒涘缓榛樿閰嶇疆
    if (result.backgroundImages.empty() && !_levelConfigs.empty()) {
        result = _levelConfigs[0];
    }
    
    // 璁剧疆姝ｇ‘鐨勪笘鐣屽搴?
    switch (_currentLevel) {
    case LevelState::LEVEL1:
        result.worldWidth = 5 * 1280.0f;
        break;
    case LevelState::FINAL_LEVEL:
        result.worldWidth = 4 * 1280.0f;
        break;
    case LevelState::LEVEL3_1:
    case LevelState::HIDDEN_LEVEL1:
        // 3-1鏈?寮犺儗鏅浘锛宧idden-level1涔熸湁5寮犺儗鏅浘
        result.worldWidth = 5 * 1280.0f;
        break;
    case LevelState::LEVEL3_2:
    case LevelState::LEVEL4_3:
    case LevelState::LEVEL4_5:
    case LevelState::LEVEL4_6:
        // 杩欎簺鍏冲崱鏈?寮犺儗鏅浘
        result.worldWidth = 3 * 1280.0f;
        break;
    default:
        // 榛樿鍗曞紶鑳屾櫙鍥?
        result.worldWidth = 1280.0f;
        break;
    }
    
    return result;
}