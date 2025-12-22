/*添加bgm，音效和ui及旁白需要在下方的AudioManager::initDefaultConfigs()中配置，接着在需要的地方添加
AudioManager::getInstance()->playBGM("audioname");
AudioManager::getInstance()->playEffect("audioname");
AudioManager::getInstance()->playUISound("audioname");
AudioManager::getInstance()->playVoice("audioname");
*/


#include "AudioManager.h"
#include "GameConfig.h"
#include <string>
#include <algorithm>

USING_NS_CC;
using namespace cocos2d::experimental;

// 静态成员初始化
AudioManager* AudioManager::s_instance = nullptr;

AudioManager::AudioManager()
    : m_currentBGMAudioID(-1)
    , m_bgmVolume(1.0f)
    , m_effectVolume(1.0f)
    , m_uiVolume(1.0f)
    , m_isMuted(false)
    ,m_voiceVolume(1.0f)
    , m_lastBGMVolume(1.0f)     
    , m_lastEffectVolume(1.0f)   
    , m_lastUIVolume(1.0f)       
{
}

AudioManager::~AudioManager()
{
    cleanupFadeActions();

    // 停止所有音频
    AudioEngine::stopAll();

    // 清理预加载的音频
    for (const auto& pair : m_audioConfigs) {
        if (pair.second.preload) {
            AudioEngine::uncache(pair.second.filePath);
        }
    }
    m_playingVoices.clear();
}

//添加清理调度器任务的方法
void AudioManager::cleanupFadeActions()
{
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule("fade_in_bgm", this);
    scheduler->unschedule("fade_out_bgm", this);
    scheduler->unschedule("stop_bgm_after_fade", this);
}

// 获取单例实例
AudioManager* AudioManager::getInstance()
{
    if (!s_instance) {
        s_instance = new (std::nothrow) AudioManager();
        CCASSERT(s_instance, "Failed to create AudioManager instance");
    }
    return s_instance;
}

// 销毁单例
void AudioManager::destroyInstance()
{
    if (s_instance) {
        // 先清理调度器任务
        s_instance->cleanupFadeActions();
        delete s_instance;
        s_instance = nullptr;
    }
}

// 初始化音频管理器
bool AudioManager::init()
{
    log("AudioManager: Initializing with AudioEngine...");

    // 初始化AudioEngine
    if (!AudioEngine::lazyInit()) {
        log("AudioManager ERROR: Failed to initialize AudioEngine");
        return false;
    }

    // 初始化默认配置
    initDefaultConfigs();

    // 预加载配置为预加载的音频
    for (const auto& pair : m_audioConfigs) {
        if (pair.second.preload) {
            // 预加载音频文件
            std::string filePath = pair.second.filePath; // 复制到局部变量
            AudioEngine::preload(filePath, [filePath](bool isSuccess) {
                if (isSuccess) {
                    log("AudioManager: Preloaded: %s", filePath.c_str());
                }
                else {
                    log("AudioManager WARNING: Failed to preload: %s", filePath.c_str());
                }
                });
        }
    }

    log("AudioManager: Initialized successfully with AudioEngine");
    return true;
}

// 初始化默认音频配置
void AudioManager::initDefaultConfigs()
{
    // ============ 背景音乐 ============
    // 开始场景背景音乐
    addAudioConfig(AudioConfig(
        "bgm_title",
        "audio/bgm/title_bgm.mp3",
        AudioType::BGM,
        0.1f,    // 音量
        true,    // 循环
        false    // 不预加载
    ));

    // 关卡1背景音乐
    addAudioConfig(AudioConfig(
        "bgm_level1",
        "audio/bgm/level1_bgm.mp3",
        AudioType::BGM,
        0.3f,
        true,
        false
    ));

    // 关卡2背景音乐
    addAudioConfig(AudioConfig(
        "bgm_level2",
        "audio/bgm/level2_bgm.mp3",
        AudioType::BGM,
        0.7f,
        true,
        false
    ));

    // 关卡3背景音乐
    addAudioConfig(AudioConfig(
        "bgm_level3",
        "audio/bgm/level3_bgm.mp3",
        AudioType::BGM,
        0.7f,
        true,
        false
    ));

    // 关卡4背景音乐
    addAudioConfig(AudioConfig(
        "bgm_level4",
        "audio/bgm/level4_bgm.mp3",
        AudioType::BGM,
        0.7f,
        true,
        false
    ));

    //暂停音乐
    addAudioConfig(AudioConfig(
        "bgm_pause",
        "audio/bgm/pause_bgm.mp3",
        AudioType::BGM,
        0.1f,    // 音量
        true,    // 循环
        false    // 不预加载
    ));

    //gameover音乐
    addAudioConfig(AudioConfig(
        "bgm_gameover",
        "audio/bgm/gameover_bgm.mp3",
        AudioType::BGM,
        0.1f,    // 音量
        true,    // 循环
        false    // 不预加载
    ));
    // ============ 音效 ============
    // 玩家音效
    addAudioConfig(AudioConfig(
        "sfx_jump",
        "audio/sfx/jump.mp3",
        AudioType::EFFECT,
        1.0f,
        false,
        true    // 预加载常用音效
    ));

    addAudioConfig(AudioConfig(
        "sfx_attack",
        "audio/sfx/attack.mp3",
        AudioType::EFFECT,
        0.2f,
        false,
        true
    ));

    addAudioConfig(AudioConfig(
        "sfx_dash",
        "audio/sfx/dash.mp3",
        AudioType::EFFECT,
        0.6f,
        false,
        true
    ));

    addAudioConfig(AudioConfig(
        "sfx_hurt",
        "audio/sfx/hurt.mp3",
        AudioType::EFFECT,
        0.8f,
        false,
        true
    ));

    // 物品收集音效
    addAudioConfig(AudioConfig(
        "sfx_collect_item",
        "audio/sfx/collect_item.mp3",
        AudioType::EFFECT,
        0.5f,
        false,
        true
    ));

    // ============ UI音效 ============
    addAudioConfig(AudioConfig(
        "ui_button_click",
        "audio/ui/button_click.mp3",
        AudioType::UI,
        1.0f,
        false,
        true
    ));

    //==============旁白音效==============
    
    // 开场旁白
    addAudioConfig(AudioConfig(
        "voice_intro",
        "audio/voice/intro.mp3",
        AudioType::EFFECT,  // 使用 EFFECT 类型
        0.8f,    // 音量较高
        false,   // 不循环
        true    // 预加载
    ));

    // 教程旁白1
    addAudioConfig(AudioConfig(
        "voice_tutorial1",
        "audio/voice/tutorial1.mp3",
        AudioType::EFFECT,
        0.8f,
        false,
        true     
    ));


    // 剧情旁白
    addAudioConfig(AudioConfig(
        "voice_story1",
        "audio/voice/story1.mp3",
        AudioType::EFFECT,
        0.8f,
        false,
        false
    ));

    // 胜利旁白
    addAudioConfig(AudioConfig(
        "voice_victory",
        "audio/voice/victory.mp3",
        AudioType::EFFECT,
        0.9f,    
        false,
        false
    ));

    // 失败旁白
    addAudioConfig(AudioConfig(
        "voice_defeat",
        "audio/voice/defeat.mp3",
        AudioType::EFFECT,
        0.7f,    
        false,
        false
    ));

    log("AudioManager: Default audio configs initialized");
}

// 添加音频配置
void AudioManager::addAudioConfig(const AudioConfig& config)
{
    m_audioConfigs[config.id] = config;
    log("AudioManager: Added audio config: %s -> %s", config.id.c_str(), config.filePath.c_str());
}

// 移除音频配置
void AudioManager::removeAudioConfig(const std::string& audioId)
{
    auto it = m_audioConfigs.find(audioId);
    if (it != m_audioConfigs.end()) {
        // 如果是预加载的音效，卸载缓存
        if (it->second.preload) {
            AudioEngine::uncache(it->second.filePath);
        }
        m_audioConfigs.erase(it);
        log("AudioManager: Removed audio config: %s", audioId.c_str());
    }
}

// 批量加载音频配置
void AudioManager::loadAudioConfigs(const std::vector<AudioConfig>& configs)
{
    for (const auto& config : configs) {
        addAudioConfig(config);
    }
}

// 播放背景音乐
void AudioManager::playBGM(const std::string& audioId)
{
    log("AudioManager: playBGM called for %s", audioId.c_str());

    if (m_isMuted) {
        log("  Audio is muted, skipping");
        return;
    }

    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("  ERROR: Audio config not found: %s", audioId.c_str());
        return;
    }

    const AudioConfig& config = it->second;
    if (config.type != AudioType::BGM) {
        log("  ERROR: Audio %s is not a BGM", audioId.c_str());
        return;
    }

    // 如果已经在播放相同的BGM，不重复播放
    if (m_currentBGM == audioId && AudioEngine::getState(m_currentBGMAudioID) == AudioEngine::AudioState::PLAYING) {
        log("  BGM already playing: %s", audioId.c_str());
        return;
    }

    // 停止当前BGM
    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        m_currentBGMAudioID = -1;
    }

    // 播放新的BGM
    m_currentBGMAudioID = AudioEngine::play2d(config.filePath, config.loop, m_bgmVolume * config.volume);
    m_currentBGM = audioId;

    if (m_currentBGMAudioID != AudioEngine::INVALID_AUDIO_ID) {
        log("  Playing BGM: %s, AudioID: %d", audioId.c_str(), m_currentBGMAudioID);
    }
    else {
        log("  ERROR: Failed to play BGM: %s", audioId.c_str());
        m_currentBGM.clear();
    }
}

// 停止背景音乐
void AudioManager::stopBGM()
{
    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        log("AudioManager: Stopped BGM (AudioID: %d)", m_currentBGMAudioID);
    }
    m_currentBGMAudioID = -1;
    m_currentBGM.clear();
}

// 暂停背景音乐
void AudioManager::pauseBGM()
{
    if (m_currentBGMAudioID != -1) {
        AudioEngine::pause(m_currentBGMAudioID);
        log("AudioManager: Paused BGM (AudioID: %d)", m_currentBGMAudioID);
    }
}

// 恢复背景音乐
void AudioManager::resumeBGM()
{
    if (m_isMuted) return;

    if (m_currentBGMAudioID != -1) {
        AudioEngine::resume(m_currentBGMAudioID);
        log("AudioManager: Resumed BGM (AudioID: %d)", m_currentBGMAudioID);
    }
}

// 淡入播放背景音乐
void AudioManager::fadeInBGM(const std::string& audioId, float duration)
{
    // BUG 6: 先清理之前的淡入任务
    cleanupFadeActions();

    playBGM(audioId);

    if (m_currentBGMAudioID != -1) {
        // 设置初始音量为0
        AudioEngine::setVolume(m_currentBGMAudioID, 0);

        // 创建淡入动作
        float currentTime = 0;
        float interval = 0.1f;
        int steps = static_cast<int>(duration / interval);

        for (int i = 1; i <= steps; i++) {
            float delay = i * interval;
            float volume = (m_bgmVolume * (i / static_cast<float>(steps)));

            Director::getInstance()->getScheduler()->schedule([this, volume](float dt) {
                if (m_currentBGMAudioID != -1) {
                    AudioEngine::setVolume(m_currentBGMAudioID, volume);
                }
                }, this, delay, 0, 0, false, "fade_in_bgm");
        }

        log("AudioManager: Fade in BGM started, duration: %.1fs", duration);
    }
}

// 淡出停止背景音乐
void AudioManager::fadeOutBGM(float duration)
{
    if (m_currentBGMAudioID == -1) return;

    // BUG 7: 先清理之前的淡出任务
    cleanupFadeActions();

    // 创建淡出动作
    float currentVolume = AudioEngine::getVolume(m_currentBGMAudioID);
    float interval = 0.1f;
    int steps = static_cast<int>(duration / interval);

    for (int i = steps; i >= 0; i--) {
        float delay = (steps - i) * interval;
        float volume = currentVolume * (i / static_cast<float>(steps));

        Director::getInstance()->getScheduler()->schedule([this, volume](float dt) {
            if (m_currentBGMAudioID != -1) {
                AudioEngine::setVolume(m_currentBGMAudioID, volume);
            }
            }, this, delay, 0, 0, false, "fade_out_bgm");
    }

    // 最后停止
    Director::getInstance()->getScheduler()->schedule([this](float dt) {
        stopBGM();
        }, this, duration, 0, 0, false, "stop_bgm_after_fade");

    log("AudioManager: Fade out BGM started, duration: %.1fs", duration);
}

// 播放音效
int AudioManager::playEffect(const std::string& audioId)
{
    if (m_isMuted) {
        log("AudioManager: Effect %s not played (muted)", audioId.c_str());
        return -1;
    }

    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("AudioManager: Effect config not found: %s", audioId.c_str());
        return -1;
    }

    const AudioConfig& config = it->second;
    if (config.type != AudioType::EFFECT) {
        log("AudioManager: Audio %s is not an effect", audioId.c_str());
        return -1;
    }

    int audioID = AudioEngine::play2d(config.filePath, config.loop, m_effectVolume * config.volume);

    if (audioID != AudioEngine::INVALID_AUDIO_ID) {
        m_playingEffects[audioID] = audioId;
        log("AudioManager: Playing effect: %s, AudioID: %d", audioId.c_str(), audioID);
    }
    else {
        log("AudioManager: Failed to play effect: %s", audioId.c_str());
    }

    return audioID;
}

// 停止所有音效
void AudioManager::stopAllEffects()
{
    // 先复制再遍历，避免遍历时容器被修改
    std::map<int, std::string> effectsCopy = m_playingEffects;

    for (const auto& pair : effectsCopy) {
        AudioEngine::stop(pair.first);
    }
    m_playingEffects.clear();
    log("AudioManager: All effects stopped");
}

// 停止指定音效
void AudioManager::stopEffect(int effectId)
{
    auto it = m_playingEffects.find(effectId);
    if (it != m_playingEffects.end()) {
        AudioEngine::stop(effectId);
        m_playingEffects.erase(it);
        log("AudioManager: Stopped effect AudioID: %d", effectId);
    }
}

// 暂停所有音效
void AudioManager::pauseAllEffects()
{
    std::map<int, std::string> effectsCopy = m_playingEffects;

    for (const auto& pair : effectsCopy) {
        AudioEngine::pause(pair.first);
    }
    log("AudioManager: All effects paused");
}

// 恢复所有音效
void AudioManager::resumeAllEffects()
{
    if (m_isMuted) return;

    std::map<int, std::string> effectsCopy = m_playingEffects;

    for (const auto& pair : effectsCopy) {
        AudioEngine::resume(pair.first);
    }
    log("AudioManager: All effects resumed");
}

// 预加载音效
void AudioManager::preloadEffect(const std::string& audioId)
{
    auto it = m_audioConfigs.find(audioId);
    if (it != m_audioConfigs.end()) {
        AudioEngine::preload(it->second.filePath, [audioId](bool isSuccess) {
            if (isSuccess) {
                log("AudioManager: Preloaded effect: %s", audioId.c_str());
            }
            else {
                log("AudioManager: Failed to preload effect: %s", audioId.c_str());
            }
            });
    }
}

// 卸载音效
void AudioManager::unloadEffect(const std::string& audioId)
{
    auto it = m_audioConfigs.find(audioId);
    if (it != m_audioConfigs.end()) {
        AudioEngine::uncache(it->second.filePath);
        log("AudioManager: Unloaded effect: %s", audioId.c_str());
    }
}

// 播放UI音效
void AudioManager::playUISound(const std::string& audioId)
{
    if (m_isMuted) {
        log("AudioManager: UI sound %s not played (muted)", audioId.c_str());
        return;
    }

    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("AudioManager: UI sound config not found: %s", audioId.c_str());
        return;
    }

    const AudioConfig& config = it->second;
    int audioID = AudioEngine::play2d(config.filePath, false, m_uiVolume * config.volume);

    if (audioID != AudioEngine::INVALID_AUDIO_ID) {
        log("AudioManager: Playing UI sound: %s, AudioID: %d", audioId.c_str(), audioID);
    }
    else {
        log("AudioManager: Failed to play UI sound: %s", audioId.c_str());
    }
}

// 设置背景音乐音量
void AudioManager::setBGMVolume(float volume)
{
    m_bgmVolume = std::max(0.0f, std::min(1.0f, volume));

    if (m_currentBGMAudioID != -1 && !m_isMuted) {
        auto it = m_audioConfigs.find(m_currentBGM);
        if (it != m_audioConfigs.end()) {
            float actualVolume = m_bgmVolume * it->second.volume;
            AudioEngine::setVolume(m_currentBGMAudioID, actualVolume);
        }
    }

    log("AudioManager: BGM volume set to %.2f", m_bgmVolume);
}

// 设置音效音量
void AudioManager::setEffectVolume(float volume)
{
    m_effectVolume = std::max(0.0f, std::min(1.0f, volume));

    if (!m_isMuted) {
        // 更新所有正在播放的音效音量
        std::map<int, std::string> effectsCopy = m_playingEffects;

        for (const auto& pair : effectsCopy) {
            int audioID = pair.first;
            const std::string& audioId = pair.second;

            auto configIt = m_audioConfigs.find(audioId);
            if (configIt != m_audioConfigs.end()) {
                float actualVolume = m_effectVolume * configIt->second.volume;
                AudioEngine::setVolume(audioID, actualVolume);
            }
        }
    }

    log("AudioManager: Effect volume set to %.2f", m_effectVolume);
}

// 设置UI音效音量
void AudioManager::setUIVolume(float volume)
{
    m_uiVolume = std::max(0.0f, std::min(1.0f, volume));
    log("AudioManager: UI volume set to %.2f", m_uiVolume);
}

// 静音/取消静音 
void AudioManager::setMuted(bool muted)
{
    if (m_isMuted == muted) return;  // 避免重复设置
    m_isMuted = muted;

    if (muted) {
        // 保存当前音量
        m_lastBGMVolume = m_bgmVolume;
        m_lastEffectVolume = m_effectVolume;
        m_lastUIVolume = m_uiVolume;

        // 将所有音量设置为0而不是暂停
        setBGMVolume(0);
        setEffectVolume(0);
        setUIVolume(0);

        log("AudioManager: Audio muted (volumes saved: BGM=%.2f, Effect=%.2f, UI=%.2f)",
            m_lastBGMVolume, m_lastEffectVolume, m_lastUIVolume);
    }
    else {
        // 恢复之前保存的音量
        setBGMVolume(m_lastBGMVolume);
        setEffectVolume(m_lastEffectVolume);
        setUIVolume(m_lastUIVolume);

        log("AudioManager: Audio unmuted (volumes restored: BGM=%.2f, Effect=%.2f, UI=%.2f)",
            m_bgmVolume, m_effectVolume, m_uiVolume);
    }
}

// 背景音乐是否正在播放
bool AudioManager::isBGMPlaying() const
{
    if (m_currentBGMAudioID == -1) return false;
    return AudioEngine::getState(m_currentBGMAudioID) == AudioEngine::AudioState::PLAYING;
}

// 处理应用进入后台
void AudioManager::onEnterBackground()
{
    // 暂停所有音频
    if (m_currentBGMAudioID != -1) {
        AudioEngine::pause(m_currentBGMAudioID);
    }

    std::map<int, std::string> effectsCopy = m_playingEffects;

    for (const auto& pair : effectsCopy) {
        AudioEngine::pause(pair.first);
    }

    log("AudioManager: Audio paused for background");
}

// 处理应用进入前台
void AudioManager::onEnterForeground()
{
    if (!m_isMuted) {
        // 恢复所有音频
        if (m_currentBGMAudioID != -1) {
            AudioEngine::resume(m_currentBGMAudioID);
        }

        std::map<int, std::string> effectsCopy = m_playingEffects;

        for (const auto& pair : effectsCopy) {
            AudioEngine::resume(pair.first);
        }

        log("AudioManager: Audio resumed for foreground");
    }
}

int AudioManager::playVoice(const std::string& audioId)//旁白用的播放方式
{
    log("AudioManager::playVoice called for %s", audioId.c_str());

    if (m_isMuted) {
        log("  Audio is muted, skipping voice");
        return -1;
    }

    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("  ERROR: Voice config not found: %s", audioId.c_str());
        return -1;
    }

    const AudioConfig& config = it->second;

    
    if (config.type != AudioType::EFFECT) {
        log("  ERROR: Audio %s is not an effect (voice should be EFFECT type)", audioId.c_str());
        return -1;
    }

    // ===== 旁白特殊处理开始 =====
    // 1. 保存当前BGM音量
    float originalBGMVolume = m_bgmVolume;

    // 2. 降低BGM音量（让旁白更清晰）
    float duckedBGMVolume = originalBGMVolume * 0.3f;  // 降到30%
    if (m_currentBGMAudioID != -1) {
        AudioEngine::setVolume(m_currentBGMAudioID, duckedBGMVolume);
    }
    log("  Ducking BGM volume from %.2f to %.2f", originalBGMVolume, duckedBGMVolume);

    // 3. 播放旁白
    int audioID = AudioEngine::play2d(config.filePath, config.loop, m_voiceVolume * config.volume);

    if (audioID != AudioEngine::INVALID_AUDIO_ID) {
        // 添加到旁白列表
        m_playingVoices[audioID] = audioId;

        // 4. 设置播放完成回调
        AudioEngine::setFinishCallback(audioID, [this, originalBGMVolume](int id, const std::string&) {
            // 从列表中移除
            m_playingVoices.erase(id);

            // 恢复BGM音量
            if (m_currentBGMAudioID != -1) {
                AudioEngine::setVolume(m_currentBGMAudioID, originalBGMVolume);
            }
            log("  Voice finished, restoring BGM volume to %.2f", originalBGMVolume);
            });

        log("  Playing voice: %s, AudioID: %d", audioId.c_str(), audioID);
    }
    else {
        log("  ERROR: Failed to play voice: %s", audioId.c_str());
        // 播放失败时也恢复BGM音量
        if (m_currentBGMAudioID != -1) {
            AudioEngine::setVolume(m_currentBGMAudioID, originalBGMVolume);
        }
    }

    return audioID;
}