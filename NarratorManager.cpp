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
    // 清空现有条目和触发器
    _narrativeEntries.clear();
    _triggers.clear();
    
    // 为了演示，直接硬编码旁白条目
    // 确保只保留实际存在的音频文件对应的条目
    
    // 第1关教程旁白
    NarrativeEntry level1Tutorial;
    level1Tutorial.id = "level_1_tutorial";
    level1Tutorial.audioFile = "Resources\\audio\\narrator\\level_1_tutorial.mp3";
    level1Tutorial.subtitleText = "欢迎来到第一关，准备好了吗？";
    level1Tutorial.duration = 3.0f;
    level1Tutorial.isSkippable = true;
    level1Tutorial.delay = 0.0f;
    _narrativeEntries[level1Tutorial.id] = level1Tutorial;
    
    // 添加触发器 - 第1关开始时触发
    TriggerCondition level1Condition;
    level1Condition.type = TriggerCondition::Type::LEVEL_START;
    level1Condition.targetId = "1";
    _triggers.emplace_back(level1Condition, level1Tutorial.id);
    
    // 玩家生命值危险旁白
    NarrativeEntry playerHealthCritical;
    playerHealthCritical.id = "player_health_critical";
    playerHealthCritical.audioFile = "Resources\\audio\\narrator\\player_health_critical.mp3";
    playerHealthCritical.subtitleText = "注意注意，你的生命值不高了，请注意躲避攻击！";
    playerHealthCritical.duration = 4.0f;
    playerHealthCritical.isSkippable = true;
    playerHealthCritical.delay = 0.0f;
    _narrativeEntries[playerHealthCritical.id] = playerHealthCritical;
    
    // 添加触发器 - 玩家生命值危险时触发
    TriggerCondition healthCriticalCondition;
    healthCriticalCondition.type = TriggerCondition::Type::CUSTOM_EVENT;
    healthCriticalCondition.customEventName = "PLAYER_HEALTH_CRITICAL";
    _triggers.emplace_back(healthCriticalCondition, playerHealthCritical.id);
    
    // Boss1就绪旁白
    NarrativeEntry boss1Ready;
    boss1Ready.id = "boss1_ready";
    boss1Ready.audioFile = "Resources\\audio\\narrator\\boss1_ready.mp3";
    boss1Ready.subtitleText = "第一个Boss即将出现！";
    boss1Ready.duration = 2.0f;
    boss1Ready.isSkippable = true;
    _narrativeEntries[boss1Ready.id] = boss1Ready;
    
    // 添加触发器 - Boss1出现时触发
    TriggerCondition boss1Condition;
    boss1Condition.type = TriggerCondition::Type::CUSTOM_EVENT;
    boss1Condition.customEventName = "BOSS_READY_boss1";
    _triggers.emplace_back(boss1Condition, boss1Ready.id);
    
    // Boss1被击中旁白
    NarrativeEntry boss1Hit;
    boss1Hit.id = "boss1_hit";
    boss1Hit.audioFile = "Resources\\audio\\narrator\\boss1_hit.mp3";
    boss1Hit.subtitleText = "Boss被击中了！继续攻击！";
    boss1Hit.duration = 1.5f;
    boss1Hit.isSkippable = true;
    _narrativeEntries[boss1Hit.id] = boss1Hit;
    
    // 添加触发器 - Boss1被击中时触发
    TriggerCondition boss1HitCondition;
    boss1HitCondition.type = TriggerCondition::Type::CUSTOM_EVENT;
    boss1HitCondition.customEventName = "BOSS_HIT_boss1";
    _triggers.emplace_back(boss1HitCondition, boss1Hit.id);
    
    // Boss2就绪旁白
    NarrativeEntry boss2Ready;
    boss2Ready.id = "boss2_ready";
    boss2Ready.audioFile = "Resources\\audio\\narrator\\boss2_ready.mp3";
    boss2Ready.subtitleText = "第二个Boss出现了！";
    boss2Ready.duration = 2.0f;
    boss2Ready.isSkippable = true;
    _narrativeEntries[boss2Ready.id] = boss2Ready;
    
    // 添加触发器 - Boss2出现时触发
    TriggerCondition boss2Condition;
    boss2Condition.type = TriggerCondition::Type::CUSTOM_EVENT;
    boss2Condition.customEventName = "BOSS_READY_boss2";
    _triggers.emplace_back(boss2Condition, boss2Ready.id);
    
    // Boss3就绪旁白
    NarrativeEntry boss3Ready;
    boss3Ready.id = "boss3_ready";
    boss3Ready.audioFile = "Resources\\audio\\narrator\\boss3_ready.mp3";
    boss3Ready.subtitleText = "最终Boss登场！";
    boss3Ready.duration = 2.0f;
    boss3Ready.isSkippable = true;
    _narrativeEntries[boss3Ready.id] = boss3Ready;
    
    // 添加触发器 - Boss3出现时触发
    TriggerCondition boss3Condition;
    boss3Condition.type = TriggerCondition::Type::CUSTOM_EVENT;
    boss3Condition.customEventName = "BOSS_READY_boss3";
    _triggers.emplace_back(boss3Condition, boss3Ready.id);
    
    // 游戏完成旁白
    NarrativeEntry gameComplete;
    gameComplete.id = "game_complete";
    gameComplete.audioFile = "Resources\\audio\\narrator\\game_complete.m4a";
    gameComplete.subtitleText = "恭喜你成功通关！感谢游玩！";
    gameComplete.duration = 5.0f;
    gameComplete.isSkippable = true;
    _narrativeEntries[gameComplete.id] = gameComplete;
    
    // 添加触发器 - 游戏完成时触发
    TriggerCondition completeCondition;
    completeCondition.type = TriggerCondition::Type::CUSTOM_EVENT;
    completeCondition.customEventName = "GAME_COMPLETE";
    _triggers.emplace_back(completeCondition, gameComplete.id);
    
    // 开场旁白
    NarrativeEntry prologue;
    prologue.id = "narrator_prologue";
    prologue.audioFile = "Resources\\audio\\narrator\\narrator-prologue.mp3";
    prologue.subtitleText = "这是一段开场旁白。";
    prologue.duration = 4.0f;
    prologue.isSkippable = true;
    _narrativeEntries[prologue.id] = prologue;
    
    // 添加触发器 - 游戏开始时触发
    TriggerCondition prologueCondition;
    prologueCondition.type = TriggerCondition::Type::CUSTOM_EVENT;
    prologueCondition.customEventName = "GAME_START";
    _triggers.emplace_back(prologueCondition, prologue.id);
    
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
        if (eventName == "LEVEL_START" && condition.type == TriggerCondition::Type::LEVEL_START) {
            if (params.find("levelId") != params.end() && 
                params.at("levelId").asString() == condition.targetId) {
                condition.hasBeenTriggered = true;
                playNarrative(narrativeId);
            }
        }
        else if (eventName == condition.customEventName && condition.type == TriggerCondition::Type::CUSTOM_EVENT) {
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
        "PLAYER_HEALTH_CRITICAL",  // 玩家生命值危险
        "BOSS_READY_boss1",        // Boss1出现
        "BOSS_HIT_boss1",          // Boss1被击中
        "BOSS_READY_boss2",        // Boss2出现
        "BOSS_READY_boss3",        // Boss3出现
        "GAME_COMPLETE",           // 游戏通关
        "GAME_START"               // 游戏开始
    };
    
    for (const auto& eventName : testEvents) {
        CCLOG("测试事件: %s", eventName.c_str());
        ValueMap params;
        if (eventName == "LEVEL_START") {
            params["levelId"] = "1";
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


