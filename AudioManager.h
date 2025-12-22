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
    float volume;           // 基础音量 (0.0-1.0)
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
    // BUG 15: 单例模式需要声明为私有构造函数，但这里是公有
    AudioManager();
    virtual ~AudioManager();  // BUG 16: 需要虚析构函数以支持继承

    // 获取单例实例
    static AudioManager* getInstance();

    // 销毁单例
    static void destroyInstance();

    // 初始化音频管理器
    bool init();

    // 音频配置管理
    void addAudioConfig(const AudioConfig& config);
    void removeAudioConfig(const std::string& audioId);
    void loadAudioConfigs(const std::vector<AudioConfig>& configs);

    // 背景音乐控制
    void playBGM(const std::string& audioId);
    void stopBGM();
    void pauseBGM();
    void resumeBGM();
    void fadeInBGM(const std::string& audioId, float duration = 2.0f);
    void fadeOutBGM(float duration = 2.0f);
    bool isBGMPlaying() const;

    // 音效控制
    int playEffect(const std::string& audioId);
    void stopAllEffects();
    void stopEffect(int effectId);
    void pauseAllEffects();
    void resumeAllEffects();
    void preloadEffect(const std::string& audioId);
    void unloadEffect(const std::string& audioId);

    // UI音效控制
    void playUISound(const std::string& audioId);

    // 音量控制
    void setBGMVolume(float volume);
    void setEffectVolume(float volume);
    void setUIVolume(float volume);
    void setMuted(bool muted);

    int playVoice(const std::string& audioId);

    // 获取音量
    float getBGMVolume() const { return m_bgmVolume; }
    float getEffectVolume() const { return m_effectVolume; }
    float getUIVolume() const { return m_uiVolume; }
    bool isMuted() const { return m_isMuted; }

    // 应用生命周期
    void onEnterBackground();
    void onEnterForeground();


private:
    // BUG 17: 需要禁用拷贝构造和赋值操作
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 初始化默认配置
    void initDefaultConfigs();

    // BUG 18: 添加清理调度器任务的方法声明
    void cleanupFadeActions();

private:
    static AudioManager* s_instance;

    // 音频配置
    std::map<std::string, AudioConfig> m_audioConfigs;

    // 正在播放的BGM
    int m_currentBGMAudioID;
    std::string m_currentBGM;

    // 正在播放的音效
    std::map<int, std::string> m_playingEffects;

    // 音量控制
    float m_bgmVolume;
    float m_effectVolume;
    float m_uiVolume;

    // BUG 19: 添加静音前保存的音量
    float m_lastBGMVolume;
    float m_lastEffectVolume;
    float m_lastUIVolume;

    bool m_isMuted;

    // 添加旁白音量成员
    float m_voiceVolume;
    // 正在播放的旁白列表
    std::map<int, std::string> m_playingVoices;
};

#endif // __AUDIO_MANAGER_H__