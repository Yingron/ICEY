// LevelMusicManager.h
#pragma once
#ifndef LEVELMUSICMANAGER_H
#define LEVELMUSICMANAGER_H

#include "LevelManager.h"
#include "AudioManager.h"
#include <string>

class LevelMusicManager {
public:
    static LevelMusicManager* getInstance();

    // 根据关卡状态播放对应的BGM
    void playBGMForLevel(LevelManager::LevelState levelState);

    // 停止当前BGM
    void stopCurrentBGM();

    // 暂停当前BGM
    void pauseCurrentBGM();

    // 恢复当前BGM
    void resumeCurrentBGM();

    // 淡入淡出切换音乐
    void fadeToNextLevelBGM(LevelManager::LevelState levelState, float duration = 1.0f);

private:
    LevelMusicManager();
    ~LevelMusicManager() = default;

    static LevelMusicManager* _instance;

    // 获取关卡对应的BGM ID
    std::string getBGMIdForLevel(LevelManager::LevelState levelState);

    // 当前正在播放的BGM ID
    std::string _currentBGMId;
};

#endif // LEVELMUSICMANAGER_H
