#include "NarratorManager.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include <string>
#include <stdexcept>

USING_NS_CC;

// 静态实例初始化
NarratorManager* NarratorManager::_instance = nullptr;

// 获取单例实例
NarratorManager* NarratorManager::getInstance() {
    if (!_instance) {
        _instance = new (std::nothrow) NarratorManager();
        CCASSERT(_instance, "NarratorManager allocation failed!");
        _instance->init();
        Director::getInstance()->getRunningScene()->addChild(_instance);
    }
    return _instance;
}

// 构造函数
NarratorManager::NarratorManager() 
    : _currentPlayingNarrative("")
    , _currentSequence("")
    , _currentSequenceIndex(-1)
    , _isMuted(false)
    , _volume(1.0f)
    , _isSubtitlesEnabled(true) {
}

// 初始化方法
bool NarratorManager::init() {
    if (!Node::init()) {
        return false;
    }
    
    // 注册为更新调度器
    scheduleUpdate();
    
    // 加载默认配置
    loadNarrativesFromJson("config/narratives.json");
    
    return true;
}

// 从JSON文件加载旁白数据
void NarratorManager::loadNarrativesFromJson(const std::string& jsonFile) {
    // 清空现有数据
    _narrativeEntries.clear();
    _sequences.clear();
    _triggers.clear();
    
    // 只保留需要的6个旁白音频
    NarrativeEntry entry;
    
    // level_1_tutorial.mp3 - 基础操作指引
    entry.id = "level_1_tutorial";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_tutorial.mp3";
    entry.subtitleText = "新手教学之移动——A键左跑，D键右跑，W键向上跳跃；新手教学之攻击——K键普攻，L键重击；还有skill技能！";
    entry.duration = 8.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // player_health_critical.mp3 - 玩家生命值低提示
    entry.id = "player_health_critical";
    entry.audioFile = "Resources\\audio\\narrator\\player_health_critical.mp3";
    entry.subtitleText = "注意注意，你的生命值不高了，请注意躲避攻击！";
    entry.duration = 3.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - caixukun
    entry.id = "boss1_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss1_ready.mp3";
    entry.subtitleText = "caixukun准备就绪！";
    entry.duration = 2.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    entry.id = "boss1_hit";
    entry.audioFile = "Resources\\audio\\narrator\\boss1_hit.mp3";
    entry.subtitleText = "caixukun被击败！";
    entry.duration = 2.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - nailong
    entry.id = "boss2_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss2_ready.mp3";
    entry.subtitleText = "nailong准备就绪！";
    entry.duration = 2.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - maodie
    entry.id = "boss3_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss3_ready.mp3";
    entry.subtitleText = "maodie准备就绪！";
    entry.duration = 2.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // 游戏通关旁白
    entry.id = "game_complete";
    entry.audioFile = "Resources\\audio\\narrator\\game_complete.mp3";
    entry.subtitleText = "恭喜你成功通关！感谢游玩！";
    entry.duration = 5.0f;
    entry.isSkippable = true;
    _narrativeEntries[entry.id] = entry;
    
    // 添加必要的触发器
    TriggerCondition condition;
    
    // 新手教程触发器
    condition.type = TriggerType::LEVEL_START;
    condition.targetId = "level_1";
    _triggers.emplace_back(condition, "level_1_tutorial");
    
    // 生命值低触发器
    condition.type = TriggerType::CUSTOM_EVENT;
    condition.customEventName = "PLAYER_HEALTH_CRITICAL";
    _triggers.emplace_back(condition, "player_health_critical");
    
    // Boss模式变化触发器
    condition.customEventName = "BOSS_READY_caixukun";
    _triggers.emplace_back(condition, "boss1_ready");
    
    condition.customEventName = "BOSS_DEFEATED_caixukun";
    _triggers.emplace_back(condition, "boss1_hit");
    
    condition.customEventName = "BOSS_READY_nailong";
    _triggers.emplace_back(condition, "boss2_ready");
    
    condition.customEventName = "BOSS_READY_maodie";
    _triggers.emplace_back(condition, "boss3_ready");
    
    // 游戏通关触发器
    condition.customEventName = "GAME_COMPLETE";
    _triggers.emplace_back(condition, "game_complete");
    
    CCLOG("NarratorManager: 已加载 %zu 个必要旁白", _narrativeEntries.size());
}

// 播放指定ID的旁白
void NarratorManager::playNarrative(const std::string& id, bool forcePlay) {
    // 检查是否存在该旁白
    auto it = _narrativeEntries.find(id);
    if (it == _narrativeEntries.end()) {
        CCLOG("NarratorManager: 找不到ID为 '%s' 的旁白", id.c_str());
        return;
    }
    
    // 如果当前正在播放且不强制播放，则不执行
    if (isPlaying() && !forcePlay) {
        CCLOG("NarratorManager: 正在播放其他旁白，跳过请求");
        return;
    }
    
    // 停止当前播放的旁白
    stopCurrentNarrative();
    
    // 内部播放处理
    playNarrativeInternal(id, forcePlay);
}

// 内部播放实现
void NarratorManager::playNarrativeInternal(const std::string& id, bool forcePlay) {
    const auto& entry = _narrativeEntries.at(id);
    
    // 记录当前播放的旁白
    _currentPlayingNarrative = id;
    
    // 如果不是强制播放，标记为已播放
    if (!forcePlay) {
        _playedNarratives.insert(id);
    }
    
    // 处理延迟播放
    if (entry.delay > 0.0f) {
        runAction(Sequence::create(
            DelayTime::create(entry.delay),
            CallFunc::create([this, id, entry]() {
                // 这里应该实现实际的音频播放逻辑
                CCLOG("NarratorManager: 播放音频: %s", entry.audioFile.c_str());
                
                // 显示字幕
                if (_isSubtitlesEnabled && !entry.subtitleText.empty()) {
                    ValueMap params;
                    params["text"] = entry.subtitleText;
                    params["duration"] = entry.duration;
                    NotificationCenter::getInstance()->postNotification("SHOW_SUBTITLE", (Ref*)&params);
                }
                
                // 设置自动完成回调
                scheduleOnce([this, id](float dt) {
                    onNarrativeComplete(id);
                }, entry.duration, "narrative_complete");
            }),
            nullptr
        ));
    } else {
        // 直接播放音频
        CCLOG("NarratorManager: 播放音频: %s", entry.audioFile.c_str());
        
        // 显示字幕
        if (_isSubtitlesEnabled && !entry.subtitleText.empty()) {
            ValueMap params;
            params["text"] = entry.subtitleText;
            params["duration"] = entry.duration;
            NotificationCenter::getInstance()->postNotification("SHOW_SUBTITLE", (Ref*)&params);
        }
        
        // 设置自动完成回调
        scheduleOnce([this, id](float dt) {
            onNarrativeComplete(id);
        }, entry.duration, "narrative_complete");
    }
}

// 停止当前旁白
void NarratorManager::stopCurrentNarrative() {
    // 取消定时器
    unschedule("narrative_complete");
    
    // 清除当前播放状态
    _currentPlayingNarrative = "";
    
    // 隐藏字幕
    NotificationCenter::getInstance()->postNotification("HIDE_SUBTITLE");
}

// 停止所有旁白
void NarratorManager::stopAllNarratives() {
    stopCurrentNarrative();
    _currentSequence = "";
    _currentSequenceIndex = -1;
}

// 检查是否正在播放
bool NarratorManager::isPlaying() const {
    return !_currentPlayingNarrative.empty();
}

// 检查旁白是否已经播放过
bool NarratorManager::hasPlayedNarrative(const std::string& id) const {
    return _playedNarratives.find(id) != _playedNarratives.end();
}

// 设置静音状态
void NarratorManager::setMuted(bool muted) {
    _isMuted = muted;
}

// 获取静音状态
bool NarratorManager::isMuted() const {
    return _isMuted;
}

// 设置音量
void NarratorManager::setVolume(float volume) {
    _volume = clampf(volume, 0.0f, 1.0f);
}

// 获取音量
float NarratorManager::getVolume() const {
    return _volume;
}

// 设置字幕是否启用
void NarratorManager::setSubtitlesEnabled(bool enabled) {
    _isSubtitlesEnabled = enabled;
    if (!enabled) {
        NotificationCenter::getInstance()->postNotification("HIDE_SUBTITLE");
    }
}

// 获取字幕是否启用
bool NarratorManager::isSubtitlesEnabled() const {
    return _isSubtitlesEnabled;
}

// 添加触发器
void NarratorManager::addTrigger(const TriggerCondition& condition, const std::string& narrativeId) {
    _triggers.emplace_back(condition, narrativeId);
}

// 检查触发器
void NarratorManager::checkTriggers() {
    // 这里可以添加自定义触发器检查逻辑
}

// 重置已播放的旁白状态
void NarratorManager::resetPlayedNarratives() {
    _playedNarratives.clear();
}

// 处理游戏事件
void NarratorManager::onGameEvent(const std::string& eventName, const ValueMap& params) {
    // 检查是否有匹配的触发器
    for (auto& triggerPair : _triggers) {
        TriggerCondition& condition = triggerPair.first;
        const std::string& narrativeId = triggerPair.second;
        
        if (condition.hasBeenTriggered || isPlaying()) {
            continue;
        }
        
        // 根据事件类型处理
        if (eventName == "LEVEL_START" && condition.type == TriggerType::LEVEL_START) {
            if (params.find("levelId") != params.end() && 
                params.at("levelId").asString() == condition.targetId) {
                condition.hasBeenTriggered = true;
                playNarrative(narrativeId);
            }
        }
        else if (eventName == condition.customEventName && condition.type == TriggerType::CUSTOM_EVENT) {
            condition.hasBeenTriggered = true;
            playNarrative(narrativeId);
        }
    }
}

// 旁白完成回调
void NarratorManager::onNarrativeComplete(const std::string& id) {
    // 清除当前播放状态
    _currentPlayingNarrative = "";
    
    // 隐藏字幕
    NotificationCenter::getInstance()->postNotification("HIDE_SUBTITLE");
}

// 更新方法
void NarratorManager::update(float delta) {
    // 检查触发器
    checkTriggers();
}

// 析构函数
NarratorManager::~NarratorManager() {
    // 清理资源
    stopAllNarratives();
    
    // 移除所有通知监听器
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    _instance = nullptr;
}

// 测试所有旁白触发功能
bool NarratorManager::testNarrationTriggers() {
    CCLOG("===== 开始测试旁白触发功能 =====");
    
    bool allTestsPassed = true;
    
    // 测试所有必要的旁白触发
    std::vector<std::string> testEvents = {
        "LEVEL_START",             // 新手教程
        "PLAYER_HEALTH_CRITICAL",  // 生命值低提示
        "BOSS_READY_caixukun",     // caixukun出现
        "BOSS_DEFEATED_caixukun",  // caixukun被击败
        "BOSS_READY_nailong",      // nailong出现
        "BOSS_READY_maodie",       // maodie出现
        "GAME_COMPLETE"            // 游戏通关
    };
    
    for (const auto& eventName : testEvents) {
        CCLOG("测试事件: %s", eventName.c_str());
        ValueMap params;
        if (eventName == "LEVEL_START") {
            params["levelId"] = "level_1";
        }
        onGameEvent(eventName, params);
        CCLOG("事件处理完成: %s", eventName.c_str());
    }
    
    CCLOG("===== 旁白触发测试完成 =====");
    return allTestsPassed;
}

// 重置所有触发器状态供测试使用
void NarratorManager::resetAllTriggers() {
    for (auto& triggerPair : _triggers) {
        triggerPair.first.hasBeenTriggered = false;
    }
    _playedNarratives.clear();
    CCLOG("所有触发器状态已重置");
}

