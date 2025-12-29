/*封装 BGM、音效、UI 音效、配音的统一调用，需要在 AudioManager::initDefaultConfigs 里添加配置。
调用示例：
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
{
    log("AudioManager::AudioManager()");
    // 初始化默认音量
    m_bgmVolume = 0.8f;
    m_effectVolume = 0.8f;
    m_uiVolume = 0.8f;
    m_voiceVolume = 1.0f;
    m_isMuted = false;
    m_currentBGMAudioID = -1;
    m_lastBGMVolume = m_bgmVolume;
    m_lastEffectVolume = m_effectVolume;
    m_lastUIVolume = m_uiVolume;
    
    // 初始化音频配置
    initDefaultConfigs();
    
    // 初始化音频引擎
    init();
    
    // 可在此注册音频完成回调
    // AudioEngine::setFinishCallback用于音频播放完毕时的回调
}

AudioManager::~AudioManager()
{
    log("AudioManager::~AudioManager()");

    // 清理淡入淡出动作
    cleanupFadeActions();

    // 释放音频资源
    unloadAllEffects();
    stopAllEffects();

    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        m_currentBGMAudioID = -1;
    }

    // 清空配置与状态
    m_audioConfigs.clear();
    m_playingEffects.clear();
    m_playingVoices.clear();
}

AudioManager* AudioManager::getInstance()
{
    if (s_instance == nullptr) {
        s_instance = new (std::nothrow) AudioManager();
    }
    return s_instance;
}

void AudioManager::destroyInstance()
{
    if (s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
    }
}

void AudioManager::initDefaultConfigs()
{
    log("AudioManager::initDefaultConfigs()");

    // === 背景音乐 ===
    // 标题场景
    addAudioConfig("bgm_title", AudioType::BGM, "audio/bgm/title_bgm.mp3", true, 0.8f);

    // 关卡 1
    addAudioConfig("bgm_level1", AudioType::BGM, "audio/bgm/level1_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_1", AudioType::BGM, "audio/bgm/level2-1_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_2", AudioType::BGM, "audio/bgm/level2-2_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_3", AudioType::BGM, "audio/bgm/level2-3_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_4", AudioType::BGM, "audio/bgm/level2-4_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_5", AudioType::BGM, "audio/bgm/level2-5_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level2_6", AudioType::BGM, "audio/bgm/level2-6_bgm.mp3", true, 0.8f);

    addAudioConfig("bgm_level3_1", AudioType::BGM, "audio/bgm/level3-1_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level3_2", AudioType::BGM, "audio/bgm/level3-2_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level3_3", AudioType::BGM, "audio/bgm/level3-3_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level3_4", AudioType::BGM, "audio/bgm/level3-4_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level3_5", AudioType::BGM, "audio/bgm/level3-5_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level3_6", AudioType::BGM, "audio/bgm/level3-6_bgm.mp3", true, 0.8f);

    addAudioConfig("bgm_level4_1", AudioType::BGM, "audio/bgm/level4-1_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level4_2", AudioType::BGM, "audio/bgm/level4-2_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level4_3", AudioType::BGM, "audio/bgm/level4-3_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level4_4", AudioType::BGM, "audio/bgm/level4-4_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level4_5", AudioType::BGM, "audio/bgm/level4-5_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_level4_6", AudioType::BGM, "audio/bgm/level4-6_bgm.mp3", true, 0.8f);

    addAudioConfig("bgm_final_level", AudioType::BGM, "audio/bgm/final_level_bgm.mp3", true, 0.8f);
    addAudioConfig("bgm_pause", AudioType::BGM, "audio/bgm/pause_bgm.mp3", true, 0.6f);

    // === 音效 ===
    // 角色动作音效
    addAudioConfig("sfx_attack", AudioType::EFFECT, "audio/sfx/attack.mp3", false, 0.7f);
    addAudioConfig("sfx_dash", AudioType::EFFECT, "audio/sfx/dash.mp3", false, 0.7f);
    addAudioConfig("sfx_jump", AudioType::EFFECT, "audio/sfx/jump.mp3", false, 0.6f);
    addAudioConfig("sfx_skill1", AudioType::EFFECT, "audio/sfx/skill1.mp3", false, 0.8f);
    addAudioConfig("sfx_skill2", AudioType::EFFECT, "audio/sfx/skill2.mp3", false, 0.8f);
    addAudioConfig("achievement", AudioType::EFFECT, "audio/sfx/achievement.mp3", false, 0.8f);//12/28/15.19
    addAudioConfig("sfx_hurt", AudioType::EFFECT, "audio/sfx/icey-be-hurted.mp3", false, 0.8f);

    // 原测试用音效（保留）
    addAudioConfig("effect_jump", AudioType::EFFECT, "effects/jump.mp3", false, 0.8f);
    addAudioConfig("effect_attack", AudioType::EFFECT, "effects/attack.mp3", false, 1.0f);
    addAudioConfig("effect_hit", AudioType::EFFECT, "effects/hit.mp3", false, 0.8f);
    addAudioConfig("effect_die", AudioType::EFFECT, "effects/die.mp3", false, 1.0f);
    addAudioConfig("effect_shield", AudioType::EFFECT, "effects/shield.mp3", false, 0.8f);
    addAudioConfig("effect_pickup", AudioType::EFFECT, "effects/pickup.mp3", false, 0.7f);



    // UI音效
    addAudioConfig("ui_button_click", AudioType::EFFECT, "ui/button_click.mp3", false, 0.5f);
    addAudioConfig("ui_level_complete", AudioType::EFFECT, "ui/level_complete.mp3", false, 0.8f);
    addAudioConfig("ui_error", AudioType::EFFECT, "ui/error.mp3", false, 0.6f);

    // 语音
    addAudioConfig("voice_start", AudioType::EFFECT, "voices/start.mp3", false, 1.0f);
    addAudioConfig("voice_gameover", AudioType::EFFECT, "voices/gameover.mp3", false, 1.0f);
    
    // 旁白（标为 VOICE，可在播放时压低 BGM）
    addAudioConfig("narrator_prologue", AudioType::VOICE, "audio/narrator/narrator-prologue.mp3", false, 1.0f);
    addAudioConfig("narrator_prologue2", AudioType::VOICE, "audio/narrator/narrator-prologue2.mp3", false, 1.0f);
    addAudioConfig("narrator_die", AudioType::VOICE, "audio/narrator/narrator-die.mp3", false, 1.0f);

    log("AudioManager: %d audio configs initialized", m_audioConfigs.size());
}

void AudioManager::addAudioConfig(const std::string& audioId, AudioType type, const std::string& filePath, bool loop, float volume)
{
    AudioConfig config;
    config.type = type;
    config.filePath = filePath;
    config.loop = loop;
    config.volume = volume;
    
    m_audioConfigs[audioId] = config;
    log("AudioManager: Added config for %s (type: %d, path: %s)", audioId.c_str(), (int)type, filePath.c_str());
}

int AudioManager::playBGM(const std::string& audioId)
{
    if (m_isMuted) {
        log("AudioManager: BGM %s not played (muted)", audioId.c_str());
        return -1;
    }
    
    // 如果当前 BGM 一样且正在播放，则直接返回
    if (!m_currentBGM.empty() && m_currentBGM == audioId && m_currentBGMAudioID != -1) {
        AudioEngine::AudioState state = AudioEngine::getState(m_currentBGMAudioID);
        if (state == AudioEngine::AudioState::PLAYING) {
            log("AudioManager: BGM %s is already playing", audioId.c_str());
            return m_currentBGMAudioID;
        }
    }
    
    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("AudioManager: BGM config not found: %s", audioId.c_str());
        return -1;
    }
    
    const AudioConfig& config = it->second;
    
    // 锟斤拷锟斤拷锟狡碉拷锟斤拷锟斤拷欠锟轿狟GM
    if (config.type != AudioType::BGM) {
        log("AudioManager: Audio %s is not a BGM", audioId.c_str());
        return -1;
    }
    
    // 停止当前正在播放的 BGM
    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        m_currentBGMAudioID = -1;
    }
    
    // 播放新的 BGM
    m_currentBGM = audioId;
    m_currentBGMAudioID = AudioEngine::play2d(config.filePath, config.loop, m_bgmVolume * config.volume);
    
    if (m_currentBGMAudioID != AudioEngine::INVALID_AUDIO_ID) {
        log("AudioManager: Playing BGM: %s, AudioID: %d", audioId.c_str(), m_currentBGMAudioID);
        
        // 注册 BGM 播放完成回调
        AudioEngine::setFinishCallback(m_currentBGMAudioID, [this](int id, const std::string&) {
            if (id == m_currentBGMAudioID) {
                m_currentBGMAudioID = -1;
                m_currentBGM.clear();
                log("AudioManager: BGM finished playing");
            }
        });
    } else {
        log("AudioManager: Failed to play BGM: %s", audioId.c_str());
        m_currentBGM.clear();
    }
    
    return m_currentBGMAudioID;
}

void AudioManager::stopBGM()
{
    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        log("AudioManager: Stopped BGM, AudioID: %d", m_currentBGMAudioID);
        m_currentBGMAudioID = -1;
        m_currentBGM.clear();
    } else {
        log("AudioManager: No BGM is currently playing");
    }
}

void AudioManager::pauseBGM()
{
    if (m_currentBGMAudioID != -1) {
        AudioEngine::pause(m_currentBGMAudioID);
        log("AudioManager: Paused BGM, AudioID: %d", m_currentBGMAudioID);
    } else {
        log("AudioManager: No BGM is currently playing");
    }
}

void AudioManager::resumeBGM()
{
    if (m_isMuted) {
        log("AudioManager: BGM not resumed (muted)");
        return;
    }
    
    if (m_currentBGMAudioID != -1) {
        AudioEngine::resume(m_currentBGMAudioID);
        log("AudioManager: Resumed BGM, AudioID: %d", m_currentBGMAudioID);
    } else {
        log("AudioManager: No BGM is currently playing");
    }
}

// 判断 BGM 是否正在播放
bool AudioManager::isBGMPlaying() const
{
    if (m_currentBGMAudioID == -1) {
        return false;
    }
    
    AudioEngine::AudioState state = AudioEngine::getState(m_currentBGMAudioID);
    bool isPlaying = (state == AudioEngine::AudioState::PLAYING);
    log("AudioManager: isBGMPlaying() -> %d (currentBGMAudioID: %d, state: %d)", 
        isPlaying, m_currentBGMAudioID, (int)state);
    return isPlaying;
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

// 预加载单个音效
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

// 卸载单个音效
void AudioManager::unloadEffect(const std::string& audioId)
{
    auto it = m_audioConfigs.find(audioId);
    if (it != m_audioConfigs.end()) {
        AudioEngine::uncache(it->second.filePath);
        log("AudioManager: Unloaded effect: %s", audioId.c_str());
    }
}

// 播放 UI 音效
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

// 设置 BGM 音量
void AudioManager::setBGMVolume(float volume)
{
    m_bgmVolume = std::max(0.0f, std::min(1.0f, volume));
    
    if (m_currentBGMAudioID != -1 && !m_isMuted) {
        auto it = m_audioConfigs.find(m_currentBGM);
        if (it != m_audioConfigs.end()) {
            const AudioConfig& config = it->second;
            float actualVolume = m_bgmVolume * config.volume;
            AudioEngine::setVolume(m_currentBGMAudioID, actualVolume);
            log("AudioManager: BGM volume set to %.2f (actual: %.2f)", volume, actualVolume);
        }
    } else {
        log("AudioManager: BGM volume set to %.2f (not applied - BGM not playing or muted)", volume);
    }
}

// 设置音效音量
void AudioManager::setEffectVolume(float volume)
{
    m_effectVolume = std::max(0.0f, std::min(1.0f, volume));
    
    if (!m_isMuted) {
        // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟节诧拷锟脚碉拷锟斤拷效锟斤拷锟斤拷
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

// 设置 UI 音效音量
void AudioManager::setUIVolume(float volume)
{
    m_uiVolume = std::max(0.0f, std::min(1.0f, volume));
    log("AudioManager: UI volume set to %.2f", m_uiVolume);
}

// 静音/取消静音 
void AudioManager::setMuted(bool muted)
{
    if (m_isMuted == muted) return;  // 状态未变化则直接返回
    m_isMuted = muted;
    
    if (muted) {
        // 记录当前音量
        m_lastBGMVolume = m_bgmVolume;
        m_lastEffectVolume = m_effectVolume;
        m_lastUIVolume = m_uiVolume;
        
        // 设为 0 实现静音
        setBGMVolume(0);
        setEffectVolume(0);
        setUIVolume(0);
        
        log("AudioManager: Audio muted (volumes saved: BGM=%.2f, Effect=%.2f, UI=%.2f)",
            m_lastBGMVolume, m_lastEffectVolume, m_lastUIVolume);
    }
    else {
        // 恢复之前的音量
        setBGMVolume(m_lastBGMVolume);
        setEffectVolume(m_lastEffectVolume);
        setUIVolume(m_lastUIVolume);
        
        log("AudioManager: Audio unmuted (volumes restored: BGM=%.2f, Effect=%.2f, UI=%.2f)",
            m_bgmVolume, m_effectVolume, m_uiVolume);
    }
}

// 进入后台时处理
void AudioManager::onEnterBackground()
{
    // 暂停当前所有音频
    if (m_currentBGMAudioID != -1) {
        AudioEngine::pause(m_currentBGMAudioID);
    }
    
    std::map<int, std::string> effectsCopy = m_playingEffects;
    
    for (const auto& pair : effectsCopy) {
        AudioEngine::pause(pair.first);
    }
    
    log("AudioManager: Audio paused for background");
}

// 回到前台时处理
void AudioManager::onEnterForeground()
{
    if (!m_isMuted) {
        // 恢复当前音频
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

int AudioManager::playVoice(const std::string& audioId)//锟斤拷锟斤拷锟斤拷锟脚接匡拷
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
    
    
    if (config.type != AudioType::EFFECT && config.type != AudioType::VOICE) {
        log("  ERROR: Audio %s is not a valid voice type (should be EFFECT or VOICE)", audioId.c_str());
        return -1;
    }
    
    // ===== Ducking 逻辑开始 =====
    // 1. 记录当前 BGM 音量
    float originalBGMVolume = m_bgmVolume;
    
    // 2. 压低 BGM 音量以突出语音
    float duckedBGMVolume = originalBGMVolume * 0.3f;  // 降低到 30%
    if (m_currentBGMAudioID != -1) {
        AudioEngine::setVolume(m_currentBGMAudioID, duckedBGMVolume);
    }
    log("  Ducking BGM volume from %.2f to %.2f", originalBGMVolume, duckedBGMVolume);
    
    // 3. 播放语音
    int audioID = AudioEngine::play2d(config.filePath, config.loop, m_voiceVolume * config.volume);
    
    if (audioID != AudioEngine::INVALID_AUDIO_ID) {
        // 记录语音句柄
        m_playingVoices[audioID] = audioId;
        
        // 4. 注册结束回调
        AudioEngine::setFinishCallback(audioID, [this, originalBGMVolume, audioId](int id, const std::string&) {
            // 移除语音记录
            m_playingVoices.erase(id);
            
            // 恢复 BGM 音量
            if (m_currentBGMAudioID != -1) {
                AudioEngine::setVolume(m_currentBGMAudioID, originalBGMVolume);
            }
            log("  Voice finished, restoring BGM volume to %.2f", originalBGMVolume);
            
            // 如果是第一段旁白，播放第二段
            if (audioId == "narrator_prologue") {
                log("  First narrator finished, playing second narrator");
                this->playVoice("narrator_prologue2");
            }
            });
        
        log("  Playing voice: %s, AudioID: %d", audioId.c_str(), audioID);
    } else {
        // 播放失败则恢复 BGM
        if (m_currentBGMAudioID != -1) {
            AudioEngine::setVolume(m_currentBGMAudioID, originalBGMVolume);
        }
        log("  ERROR: Failed to play voice: %s", audioId.c_str());
    }
    
    return audioID;
}

// 设置语音音量
void AudioManager::setVoiceVolume(float volume)
{
    m_voiceVolume = std::max(0.0f, std::min(1.0f, volume));
    
    if (!m_isMuted) {
        // 更新正在播放的语音音量
        std::map<int, std::string> voicesCopy = m_playingVoices;
        
        for (const auto& pair : voicesCopy) {
            int audioID = pair.first;
            const std::string& audioId = pair.second;
            
            auto configIt = m_audioConfigs.find(audioId);
            if (configIt != m_audioConfigs.end()) {
                float actualVolume = m_voiceVolume * configIt->second.volume;
                AudioEngine::setVolume(audioID, actualVolume);
            }
        }
    }
    
    log("AudioManager: Voice volume set to %.2f", m_voiceVolume);
}

// 停止所有语音
void AudioManager::stopAllVoices()
{
    std::map<int, std::string> voicesCopy = m_playingVoices;
    
    for (const auto& pair : voicesCopy) {
        int audioID = pair.first;
        AudioEngine::stop(audioID);
        m_playingVoices.erase(audioID);
    }
    
    log("AudioManager: All voices stopped");
}

// 暂停所有语音
void AudioManager::pauseAllVoices()
{
    std::map<int, std::string> voicesCopy = m_playingVoices;
    
    for (const auto& pair : voicesCopy) {
        int audioID = pair.first;
        AudioEngine::pause(audioID);
    }
    
    log("AudioManager: All voices paused");
}

// 恢复所有语音
void AudioManager::resumeAllVoices()
{
    if (m_isMuted) return;
    
    std::map<int, std::string> voicesCopy = m_playingVoices;
    
    for (const auto& pair : voicesCopy) {
        int audioID = pair.first;
        AudioEngine::resume(audioID);
    }
    
    log("AudioManager: All voices resumed");
}

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
        
        // 注册音效完成回调
        AudioEngine::setFinishCallback(audioID, [this](int id, const std::string&) {
            m_playingEffects.erase(id);
        });
    } else {
        log("AudioManager: Failed to play effect: %s", audioId.c_str());
    }
    
    return audioID;
}

void AudioManager::stopEffect(int audioID)
{
    if (audioID == -1) return;
    
    AudioEngine::stop(audioID);
    m_playingEffects.erase(audioID);
    log("AudioManager: Stopped effect, AudioID: %d", audioID);
}

void AudioManager::stopAllEffects()
{
    std::map<int, std::string> effectsCopy = m_playingEffects;
    
    for (const auto& pair : effectsCopy) {
        AudioEngine::stop(pair.first);
    }
    
    m_playingEffects.clear();
    log("AudioManager: All effects stopped");
}

void AudioManager::pauseEffect(int audioID)
{
    if (audioID == -1) return;
    
    AudioEngine::pause(audioID);
    log("AudioManager: Paused effect, AudioID: %d", audioID);
}

void AudioManager::resumeEffect(int audioID)
{
    if (audioID == -1) return;
    
    if (m_isMuted) {
        log("AudioManager: Effect not resumed (muted)");
        return;
    }
    
    AudioEngine::resume(audioID);
    log("AudioManager: Resumed effect, AudioID: %d", audioID);
}

void AudioManager::unloadAllEffects()
{
    // 停止所有音效
    stopAllEffects();
    
    // 卸载已加载的音效文件
    std::vector<std::string> loadedFiles;
    
    for (const auto& pair : m_audioConfigs) {
        const AudioConfig& config = pair.second;
        
        // 只卸载音效文件
        if (config.type == AudioType::EFFECT) {
            // 去重后再执行 uncache
            if (std::find(loadedFiles.begin(), loadedFiles.end(), config.filePath) == loadedFiles.end()) {
                AudioEngine::uncache(config.filePath);
                loadedFiles.push_back(config.filePath);
                log("AudioManager: Uncached effect file: %s", config.filePath.c_str());
            }
        }
    }
    
    log("AudioManager: Unloaded all effects");
}

// 初始化音频引擎
bool AudioManager::init()
{
    log("AudioManager::init()");
    // Lazy init，确保底层资源初始化一次
    AudioEngine::lazyInit();

    // 预加载需要预加载的音频（如有）
    for (const auto& pair : m_audioConfigs) {
        const AudioConfig& config = pair.second;
        if (config.preload) {
            AudioEngine::preload(config.filePath);
            log("AudioManager: Preloaded audio: %s", config.id.c_str());
        }
    }

    return true;
}

// 删除某个音频配置
void AudioManager::removeAudioConfig(const std::string& audioId)
{
    auto it = m_audioConfigs.find(audioId);
    if (it != m_audioConfigs.end()) {
        m_audioConfigs.erase(it);
        log("AudioManager: Removed audio config: %s", audioId.c_str());
    }
}

// 批量加载音频配置
void AudioManager::loadAudioConfigs(const std::vector<AudioConfig>& configs)
{
    for (const auto& config : configs) {
        m_audioConfigs[config.id] = config;
        log("AudioManager: Loaded audio config: %s", config.id.c_str());
    }
}

// 淡入播放 BGM
void AudioManager::fadeInBGM(const std::string& audioId, float duration)
{
    log("AudioManager::fadeInBGM(%s, %.2f)", audioId.c_str(), duration);

    if (m_isMuted) {
        log("AudioManager: BGM %s not faded in (muted)", audioId.c_str());
        return;
    }

    // 停止当前 BGM（如果有），避免叠加
    if (m_currentBGMAudioID != -1) {
        AudioEngine::stop(m_currentBGMAudioID);
        m_currentBGMAudioID = -1;
    }

    // 查找配置
    auto it = m_audioConfigs.find(audioId);
    if (it == m_audioConfigs.end()) {
        log("AudioManager: BGM config not found: %s", audioId.c_str());
        return;
    }

    const AudioConfig& config = it->second;

    // 校验类型
    if (config.type != AudioType::BGM) {
        log("AudioManager: Audio %s is not a BGM", audioId.c_str());
        return;
    }

    // 以 0 音量开始播放
    m_currentBGM = audioId;
    m_currentBGMAudioID = AudioEngine::play2d(config.filePath, config.loop, 0.0f);

    if (m_currentBGMAudioID != AudioEngine::INVALID_AUDIO_ID) {
        log("AudioManager: Started fading in BGM: %s, AudioID: %d", audioId.c_str(), m_currentBGMAudioID);

        // 目标音量
        float targetVolume = m_bgmVolume * config.volume;

        // 计算每帧步进（假设 60FPS）
        float step = targetVolume / (duration * 60.0f); // 假设60FPS

        // AudioEngine 没有内置淡入，这里用 Scheduler 实现
        Director::getInstance()->getScheduler()->schedule(
            [this, targetVolume, step, config](float dt) {
                if (m_currentBGMAudioID == -1) return;

                float currentVolume = AudioEngine::getVolume(m_currentBGMAudioID);
                float newVolume = currentVolume + step;

                if (newVolume >= targetVolume) {
                    newVolume = targetVolume;
                    AudioEngine::setVolume(m_currentBGMAudioID, newVolume);
                    Director::getInstance()->getScheduler()->unschedule("fade_in_bgm", this);
                    log("AudioManager: BGM fade in completed");
                }
                else {
                    AudioEngine::setVolume(m_currentBGMAudioID, newVolume);
                }
            },
            this, 1.0f / 60.0f, CC_REPEAT_FOREVER, 0.0f, false, "fade_in_bgm");

        // 注册播放完成回调
        AudioEngine::setFinishCallback(m_currentBGMAudioID, [this](int id, const std::string&) {
            if (id == m_currentBGMAudioID) {
                Director::getInstance()->getScheduler()->unschedule("fade_in_bgm", this);
                m_currentBGMAudioID = -1;
                m_currentBGM.clear();
                log("AudioManager: BGM finished playing");
            }
            });
    }
    else {
        log("AudioManager: Failed to play BGM: %s", audioId.c_str());
        m_currentBGM.clear();
    }
}

// 淡出当前 BGM
void AudioManager::fadeOutBGM(float duration)
{
    log("AudioManager::fadeOutBGM(%.2f)", duration);

    if (m_currentBGMAudioID == -1) {
        log("AudioManager: No BGM is currently playing");
        return;
    }

    float currentVolume = AudioEngine::getVolume(m_currentBGMAudioID);
    float step = currentVolume / (duration * 60.0f); // 60FPS

    Director::getInstance()->getScheduler()->schedule(
        [this, step](float dt) {
            if (m_currentBGMAudioID == -1) return;

            float currentVolume = AudioEngine::getVolume(m_currentBGMAudioID);
            float newVolume = currentVolume - step;

            if (newVolume <= 0.0f) {
                AudioEngine::stop(m_currentBGMAudioID);
                m_currentBGMAudioID = -1;
                m_currentBGM.clear();
                Director::getInstance()->getScheduler()->unschedule("fade_out_bgm", this);
                log("AudioManager: BGM fade out completed");
            }
            else {
                AudioEngine::setVolume(m_currentBGMAudioID, newVolume);
            }
        },
        this, 1.0f / 60.0f, CC_REPEAT_FOREVER, 0.0f, false, "fade_out_bgm");
}

// 清理淡入淡出调度
void AudioManager::cleanupFadeActions()
{
    log("AudioManager::cleanupFadeActions()");
    // 锟斤拷锟斤拷锟斤拷锟叫碉拷锟诫淡锟斤拷锟斤拷schedule
    Director::getInstance()->getScheduler()->unschedule("fade_in_bgm", this);
    Director::getInstance()->getScheduler()->unschedule("fade_out_bgm", this);

}
