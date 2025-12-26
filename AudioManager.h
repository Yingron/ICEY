#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "AudioEngine.h"
#include <string>
#include <map>
#include <vector>

USING_NS_CC;
using namespace cocos2d::experimental;

// 音频类型枚举
enum class AudioType
{
    BGM,    // 背景音乐
    EFFECT, // 游戏音效
    UI      // UI音效
};

// 音频配置结构体
struct AudioConfig
{
    std::string id;         // 音频ID
    std::string filePath;   // 文件路径
    AudioType type;         // 音频类型
    float volume;           // 相对音量 (0.0-1.0)
    bool loop;              // 是否循环
    bool preload;           // 是否预加载

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

    // 获取单例实例
    static AudioManager* getInstance();

    // 销毁单例
    static void destroyInstance();

    // 初始化音频管理器
    bool init();

    // 音频配置管理
    void addAudioConfig(const AudioConfig& config);
    void addAudioConfig(const std::string& audioId, AudioType type, const std::string& filePath, bool loop, float volume);
    void removeAudioConfig(const std::string& audioId);
    void loadAudioConfigs(const std::vector<AudioConfig>& configs);

    // 背景音乐控制
    int playBGM(const std::string& audioId);  // 修改：返回int类型
    void stopBGM();
    void pauseBGM();
    void resumeBGM();
    void fadeInBGM(const std::string& audioId, float duration = 2.0f);
    void fadeOutBGM(float duration = 2.0f);
    bool isBGMPlaying() const;

    // 音效管理
    int playEffect(const std::string& audioId);
    void stopAllEffects();
    void unloadAllEffects();
    void stopEffect(int audioID);  // 修改：参数名统一
    void pauseAllEffects();
    void resumeAllEffects();
    void pauseEffect(int audioID);  // 添加：暂停单个音效
    void resumeEffect(int audioID); // 添加：恢复单个音效
    void preloadEffect(const std::string& audioId);
    void unloadEffect(const std::string& audioId);

    // UI音效管理
    void playUISound(const std::string& audioId);

    // 语音管理
    int playVoice(const std::string& audioId);
    void stopAllVoices();   // 添加：停止所有语音
    void pauseAllVoices();  // 添加：暂停所有语音
    void resumeAllVoices(); // 添加：恢复所有语音

    // 音量控制
    void setBGMVolume(float volume);
    void setEffectVolume(float volume);
    void setUIVolume(float volume);
    void setVoiceVolume(float volume);  // 添加：语音音量控制
    void setMuted(bool muted);

    // 获取音量
    float getBGMVolume() const;
    float getEffectVolume() const;
    float getUIVolume() const;
    float getVoiceVolume() const;  // 添加：获取语音音量
    bool isMuted() const;
    const std::string& getCurrentBGM() const;  // 添加：获取当前BGM

    // 应用状态处理
    void onEnterBackground();
    void onEnterForeground();

private:
    // 禁止拷贝和赋值
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 初始化默认配置
    void initDefaultConfigs();

    // 清理淡入淡出动作
    void cleanupFadeActions();

private:
    static AudioManager* s_instance;

    // 音频配置
    std::map<std::string, AudioConfig> m_audioConfigs;

    // 当前播放的BGM
    int m_currentBGMAudioID;
    std::string m_currentBGM;

    // 正在播放的音效
    std::map<int, std::string> m_playingEffects;

    // 正在播放的语音
    std::map<int, std::string> m_playingVoices;  // 添加：语音列表

    // 音量设置
    float m_bgmVolume;
    float m_effectVolume;
    float m_uiVolume;
    float m_voiceVolume;  // 添加：语音音量

    // 静音前音量存储
    float m_lastBGMVolume;
    float m_lastEffectVolume;
    float m_lastUIVolume;

    bool m_isMuted;
};

#endif // __AUDIO_MANAGER_H__