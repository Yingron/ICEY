#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "AudioEngine.h"
#include <string>
#include <map>
#include <vector>

USING_NS_CC;
using namespace cocos2d::experimental;

// 闊抽绫诲瀷鏋氫妇
enum class AudioType
{
    BGM,    // 鑳屾櫙闊充箰
    EFFECT, // 娓告垙闊虫晥
    UI,     // UI闊虫晥
    VOICE   // 鏃佺櫧/璇煶
};

// 闊抽閰嶇疆缁撴瀯浣?
struct AudioConfig
{
    std::string id;         // 闊抽ID
    std::string filePath;   // 鏂囦欢璺緞
    AudioType type;         // 闊抽绫诲瀷
    float volume;           // 鐩稿闊抽噺 (0.0-1.0)
    bool loop;              // 鏄惁寰幆
    bool preload;           // 鏄惁棰勫姞杞?

    AudioConfig()
        : volume(1.0f)
        , loop(false)
        , preload(false)
    {
    }

    AudioConfig(const std::string& _id, const std::string& _filePath,
        AudioType _type, float _volume, bool _loop, bool _preload)
        : id(_id)
        , filePath(_filePath)
        , type(_type)
        , volume(_volume)
        , loop(_loop)
        , preload(_preload)
    {
    }
};

class AudioManager
{
public:
    AudioManager();
    virtual ~AudioManager();

    // 鑾峰彇鍗曚緥瀹炰緥
    static AudioManager* getInstance();

    // 閿€姣佸崟渚?
    static void destroyInstance();

    // 鍒濆鍖栭煶棰戠鐞嗗櫒
    bool init();

    // 闊抽閰嶇疆绠＄悊
    void addAudioConfig(const AudioConfig& config);
    void addAudioConfig(const std::string& audioId, AudioType type, const std::string& filePath, bool loop, float volume);
    void removeAudioConfig(const std::string& audioId);
    void loadAudioConfigs(const std::vector<AudioConfig>& configs);

    // 鑳屾櫙闊充箰鎺у埗
    int playBGM(const std::string& audioId);  // 淇敼锛氳繑鍥瀒nt绫诲瀷
    void stopBGM();
    void pauseBGM();
    void resumeBGM();
    void fadeInBGM(const std::string& audioId, float duration = 2.0f);
    void fadeOutBGM(float duration = 2.0f);
    bool isBGMPlaying() const;

    // 闊虫晥绠＄悊
    int playEffect(const std::string& audioId);
    void stopAllEffects();
    void unloadAllEffects();
    void stopEffect(int audioID);  // 淇敼锛氬弬鏁板悕缁熶竴
    void pauseAllEffects();
    void resumeAllEffects();
    void pauseEffect(int audioID);  // 娣诲姞锛氭殏鍋滃崟涓煶鏁?
    void resumeEffect(int audioID); // 娣诲姞锛氭仮澶嶅崟涓煶鏁?
    void preloadEffect(const std::string& audioId);
    void unloadEffect(const std::string& audioId);

    // UI闊虫晥绠＄悊
    void playUISound(const std::string& audioId);

    // 璇煶绠＄悊
    int playVoice(const std::string& audioId);
    void stopAllVoices();   // 娣诲姞锛氬仠姝㈡墍鏈夎闊?
    void pauseAllVoices();  // 娣诲姞锛氭殏鍋滄墍鏈夎闊?
    void resumeAllVoices(); // 娣诲姞锛氭仮澶嶆墍鏈夎闊?

    // 闊抽噺鎺у埗
    void setBGMVolume(float volume);
    void setEffectVolume(float volume);
    void setUIVolume(float volume);
    void setVoiceVolume(float volume);  // 娣诲姞锛氳闊抽煶閲忔帶鍒?
    void setMuted(bool muted);

    // 鑾峰彇闊抽噺
    float getBGMVolume() const;
    float getEffectVolume() const;
    float getUIVolume() const;
    float getVoiceVolume() const;  // 娣诲姞锛氳幏鍙栬闊抽煶閲?
    bool isMuted() const;
    const std::string& getCurrentBGM() const;  // 娣诲姞锛氳幏鍙栧綋鍓岯GM

    // 搴旂敤鐘舵€佸鐞?
    void onEnterBackground();
    void onEnterForeground();

private:
    // 绂佹鎷疯礉鍜岃祴鍊?
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 鍒濆鍖栭粯璁ら厤缃?
    void initDefaultConfigs();

    // 娓呯悊娣″叆娣″嚭鍔ㄤ綔
    void cleanupFadeActions();

private:
    static AudioManager* s_instance;

    // 闊抽閰嶇疆
    std::map<std::string, AudioConfig> m_audioConfigs;

    // 褰撳墠鎾斁鐨凚GM
    int m_currentBGMAudioID;
    std::string m_currentBGM;

    // 姝ｅ湪鎾斁鐨勯煶鏁?
    std::map<int, std::string> m_playingEffects;

    // 姝ｅ湪鎾斁鐨勮闊?
    std::map<int, std::string> m_playingVoices;  // 娣诲姞锛氳闊冲垪琛?

    // 闊抽噺璁剧疆
    float m_bgmVolume;
    float m_effectVolume;
    float m_uiVolume;
    float m_voiceVolume;  // 娣诲姞锛氳闊抽煶閲?

    // 闈欓煶鍓嶉煶閲忓瓨鍌?
    float m_lastBGMVolume;
    float m_lastEffectVolume;
    float m_lastUIVolume;

    bool m_isMuted;
};

#endif // __AUDIO_MANAGER_H__