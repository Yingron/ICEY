#pragma once
#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "cocos2d.h"
#include <unordered_map>

class SoundManager
{
private:
    static SoundManager* m_instance;

    std::unordered_map<std::string, cocos2d::Experimental::AudioProfile> m_audioProfiles;
    float m_masterVolume;
    float m_bgmVolume;
    float m_sfxVolume;

    std::string m_currentBGM;

public:
    static SoundManager* getInstance();
    static void destroyInstance();

    void preloadSounds();

    // ±≥æ∞“Ù¿÷
    void playBGM(const std::string& bgmName, bool loop = true);
    void stopBGM();
    void pauseBGM();
    void resumeBGM();

    // “Ù–ß
    unsigned int playSFX(const std::string& sfxName, bool loop = false);
    void stopSFX(unsigned int soundId);

    // “Ù¡øøÿ÷∆
    void setMasterVolume(float volume);
    void setBGMVolume(float volume);
    void setSFXVolume(float volume);

    float getMasterVolume() const { return m_masterVolume; }
    float getBGMVolume() const { return m_bgmVolume; }
    float getSFXVolume() const { return m_sfxVolume; }

private:
    SoundManager();
    ~SoundManager();

    void createAudioProfiles();
};

#endif // __SOUND_MANAGER_H__