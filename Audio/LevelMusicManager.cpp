// LevelMusicManager.cpp
#include "LevelMusicManager.h"

USING_NS_CC;

LevelMusicManager* LevelMusicManager::_instance = nullptr;

LevelMusicManager* LevelMusicManager::getInstance() {
    if (!_instance) {
        _instance = new LevelMusicManager();
    }
    return _instance;
}

LevelMusicManager::LevelMusicManager() : _currentBGMId("") {
}

std::string LevelMusicManager::getBGMIdForLevel(LevelManager::LevelState levelState) {
    switch (levelState) {
        // 开始场景
    case LevelManager::LevelState::START_SCENE:
        return "bgm_title";

        // 关卡1
    case LevelManager::LevelState::LEVEL1:
        return "bgm_level1";

        // 关卡2的6个子关卡
    case LevelManager::LevelState::LEVEL2_1:
        return "bgm_level2_1";
    case LevelManager::LevelState::LEVEL2_2:
        return "bgm_level2_2";
    case LevelManager::LevelState::LEVEL2_3:
        return "bgm_level2_3";
    case LevelManager::LevelState::LEVEL2_4:
        return "bgm_level2_4";
    case LevelManager::LevelState::LEVEL2_5:
        return "bgm_level2_5";
    case LevelManager::LevelState::LEVEL2_6:
        return "bgm_level2_6";

        // 关卡3的6个子关卡
    case LevelManager::LevelState::LEVEL3_1:
        return "bgm_level3_1";
    case LevelManager::LevelState::LEVEL3_2:
        return "bgm_level3_2";
    case LevelManager::LevelState::LEVEL3_3:
        return "bgm_level3_3";
    case LevelManager::LevelState::LEVEL3_4:
        return "bgm_level3_4";
    case LevelManager::LevelState::LEVEL3_5:
        return "bgm_level3_5";
    case LevelManager::LevelState::LEVEL3_6:
        return "bgm_level3_6";

        // 关卡4的6个子关卡
    case LevelManager::LevelState::LEVEL4_1:
        return "bgm_level4_1";
    case LevelManager::LevelState::LEVEL4_2:
        return "bgm_level4_2";
    case LevelManager::LevelState::LEVEL4_3:
        return "bgm_level4_3";
    case LevelManager::LevelState::LEVEL4_4:
        return "bgm_level4_4";
    case LevelManager::LevelState::LEVEL4_5:
        return "bgm_level4_5";
    case LevelManager::LevelState::LEVEL4_6:
        return "bgm_level4_6";

        // 最终关卡
    case LevelManager::LevelState::FINAL_LEVEL:
        return "bgm_final_level";

        // 暂停场景
    case LevelManager::LevelState::PAUSE_SCENE:
        return "bgm_pause";

        // 游戏结束场景
    case LevelManager::LevelState::GAMEOVER_SCENE:
        return "bgm_gameover";

    default:
        return "bgm_level1"; // 默认返回关卡1音乐
    }
}

void LevelMusicManager::playBGMForLevel(LevelManager::LevelState levelState) {
    std::string bgmId = getBGMIdForLevel(levelState);

    // 如果已经在播放相同的BGM，则不需要重新播放
    if (_currentBGMId == bgmId && AudioManager::getInstance()->isBGMPlaying()) {
        return;
    }

    // 停止当前的BGM
    if (!_currentBGMId.empty()) {
        AudioManager::getInstance()->stopBGM();
    }

    // 播放新的BGM
    AudioManager::getInstance()->playBGM(bgmId);
    _currentBGMId = bgmId;

    log("LevelMusicManager: Playing BGM for level %d -> %s",
        (int)levelState, bgmId.c_str());
}

void LevelMusicManager::stopCurrentBGM() {
    if (!_currentBGMId.empty()) {
        AudioManager::getInstance()->stopBGM();
        _currentBGMId = "";
    }
}

void LevelMusicManager::pauseCurrentBGM() {
    AudioManager::getInstance()->pauseBGM();
}

void LevelMusicManager::resumeCurrentBGM() {
    AudioManager::getInstance()->resumeBGM();
}

void LevelMusicManager::fadeToNextLevelBGM(LevelManager::LevelState levelState, float duration) {
    std::string bgmId = getBGMIdForLevel(levelState);

    // 如果已经在播放相同的BGM，则不需要切换
    if (_currentBGMId == bgmId) {
        return;
    }

    // 淡出当前BGM
    if (!_currentBGMId.empty()) {
        AudioManager::getInstance()->fadeOutBGM(duration / 2);

        // 延迟播放新的BGM
        Director::getInstance()->getScheduler()->schedule([this, bgmId, duration](float dt) {
            // 淡入新的BGM
            AudioManager::getInstance()->fadeInBGM(bgmId, duration / 2);
            _currentBGMId = bgmId;

            log("LevelMusicManager: Faded to BGM -> %s", bgmId.c_str());
            }, this, duration / 2, 0, 0, false, "fade_to_bgm");
    }
    else {
        // 直接淡入新的BGM
        AudioManager::getInstance()->fadeInBGM(bgmId, duration);
        _currentBGMId = bgmId;
    }
}