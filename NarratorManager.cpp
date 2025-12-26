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
    
    // 只保留需要的11个旁白音频
    std::set<std::string> requiredAudioFiles = {
        "level_1_start.mp3",
        "level_1_tutorial.mp3",
        "level_1_first_enemy.mp3",
        "level_1_boss_approaching.mp3",
        "level_1_complete.mp3",
        "level_2_start.mp3",
        "level_3_start.mp3",
        "final_boss_approach.mp3",
        "final_boss_defeated.mp3",
        "game_complete.mp3",
        "player_health_critical.mp3"
    };
    
    // 硬编码添加必要的旁白条目
    NarrativeEntry entry;
    
    // level_1_start.mp3 - 游戏开场旁白
    entry.id = "level_1_start";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_start.mp3";
    entry.subtitleText = "欢迎来到第一关！";
    entry.duration = 3.0f;
    entry.isSkippable = true;
    entry.volume = 1.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_1_tutorial.mp3 - 基础操作指引
    entry.id = "level_1_tutorial";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_tutorial.mp3";
    entry.subtitleText = "使用方向键移动，空格键跳跃，Ctrl键攻击！";
    entry.duration = 4.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_1_first_enemy.mp3 - 首次遇到敌人提示
    entry.id = "level_1_first_enemy";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_first_enemy.mp3";
    entry.subtitleText = "小心！敌人出现了！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_1_boss_approaching.mp3 - Boss战即将开始提示
    entry.id = "level_1_boss_approaching";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_boss_approaching.mp3";
    entry.subtitleText = "注意！Boss即将出现！";
    entry.duration = 3.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_1_complete.mp3 - 完成第一关提示
    entry.id = "level_1_complete";
    entry.audioFile = "Resources\\audio\\narrator\\level_1_complete.mp3";
    entry.subtitleText = "恭喜完成第一关！";
    entry.duration = 3.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_2_start.mp3 - 第二关开始旁白
    entry.id = "level_2_start";
    entry.audioFile = "Resources\\audio\\narrator\\level_2_start.mp3";
    entry.subtitleText = "欢迎来到第二关！";
    entry.duration = 3.0f;
    _narrativeEntries[entry.id] = entry;
    
    // level_3_start.mp3 - 第三关开始旁白
    entry.id = "level_3_start";
    entry.audioFile = "Resources\\audio\\narrator\\level_3_start.mp3";
    entry.subtitleText = "欢迎来到最终关卡！";
    entry.duration = 3.0f;
    _narrativeEntries[entry.id] = entry;
    
    // final_boss_approach.mp3 - 接近最终Boss提示
    entry.id = "final_boss_approach";
    entry.audioFile = "Resources\\audio\\narrator\\final_boss_approach.mp3";
    entry.subtitleText = "最终Boss就在前方！准备战斗！";
    entry.duration = 4.0f;
    _narrativeEntries[entry.id] = entry;
    
    // final_boss_defeated.mp3 - 击败最终Boss提示
    entry.id = "final_boss_defeated";
    entry.audioFile = "Resources\\audio\\narrator\\final_boss_defeated.mp3";
    entry.subtitleText = "太棒了！你击败了最终Boss！";
    entry.duration = 3.0f;
    _narrativeEntries[entry.id] = entry;
    
    // game_complete.mp3 - 游戏通关旁白
    entry.id = "game_complete";
    entry.audioFile = "Resources\\audio\\narrator\\game_complete.mp3";
    entry.subtitleText = "恭喜你通关了游戏！";
    entry.duration = 4.0f;
    _narrativeEntries[entry.id] = entry;
    
    // player_health_critical.mp3 - 玩家生命值低提示
    entry.id = "player_health_critical";
    entry.audioFile = "Resources\\audio\\narrator\\player_health_critical.mp3";
    entry.subtitleText = "你的生命值很低！小心！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - caixukun
    entry.id = "boss1_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss1_ready.mp3";
    entry.subtitleText = "caixukun准备就绪！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    entry.id = "boss1_hit";
    entry.audioFile = "Resources\\audio\\narrator\\boss1_hit.mp3";
    entry.subtitleText = "caixukun被击败！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - nailong
    entry.id = "boss2_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss2_ready.mp3";
    entry.subtitleText = "nailong准备就绪！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    // Boss专属旁白条目 - maodie
    entry.id = "boss3_ready";
    entry.audioFile = "Resources\\audio\\narrator\\boss3_ready.mp3";
    entry.subtitleText = "maodie准备就绪！";
    entry.duration = 2.0f;
    _narrativeEntries[entry.id] = entry;
    
    // 添加默认的触发条件
    TriggerCondition condition;
    
    // 关卡开始触发器
    condition.type = TriggerType::LEVEL_START;
    condition.targetId = "level_1";
    _triggers.emplace_back(condition, "level_1_start");
    
    condition.targetId = "level_2";
    _triggers.emplace_back(condition, "level_2_start");
    
    condition.targetId = "level_3";
    _triggers.emplace_back(condition, "level_3_start");
    
    // 关卡完成触发器
    condition.type = TriggerType::LEVEL_END;
    condition.targetId = "level_1";
    _triggers.emplace_back(condition, "level_1_complete");
    
    // Boss相关触发器
    condition.type = TriggerType::BOSS_APPEAR;
    condition.targetId = "final_boss";
    _triggers.emplace_back(condition, "final_boss_approach");
    
    condition.type = TriggerType::BOSS_DEFEATED;
    condition.targetId = "final_boss";
    _triggers.emplace_back(condition, "final_boss_defeated");
    
    // 游戏完成触发器
    condition.type = TriggerType::CUSTOM_EVENT;
    condition.customEventName = "GAME_COMPLETE";
    _triggers.emplace_back(condition, "game_complete");
    
    // Boss模式变化触发器
    condition.type = TriggerType::CUSTOM_EVENT;
    condition.customEventName = "BOSS_READY_caixukun";
    _triggers.emplace_back(condition, "boss1_ready");
    
    condition.customEventName = "BOSS_DEFEATED_caixukun";
    _triggers.emplace_back(condition, "boss1_hit");
    
    condition.customEventName = "BOSS_READY_nailong";
    _triggers.emplace_back(condition, "boss2_ready");
    
    condition.customEventName = "BOSS_READY_maodie";
    _triggers.emplace_back(condition, "boss3_ready");
    
    CCLOG("NarratorManager: Loaded %zu required narratives", _narrativeEntries.size());
}

// 播放指定ID的旁白
void NarratorManager::playNarrative(const std::string& id, bool forcePlay) {
    // 检查是否存在该旁白
    auto it = _narrativeEntries.find(id);
    if (it == _narrativeEntries.end()) {
        CCLOG("NarratorManager: Narrative with ID '%s' not found", id.c_str());
        return;
    }
    
    // 如果当前正在播放且不强制播放，则不执行
    if (isPlaying() && !forcePlay) {
        CCLOG("NarratorManager: Already playing a narrative, skipping request");
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
                CCLOG("NarratorManager: Playing audio: %s", entry.audioFile.c_str());
                
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
        CCLOG("NarratorManager: Playing audio: %s", entry.audioFile.c_str());
        
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

// 播放指定ID的旁白序列
void NarratorManager::playSequence(const std::string& sequenceId, bool forcePlay) {
    // 检查序列是否存在
    auto it = _sequences.find(sequenceId);
    if (it == _sequences.end()) {
        CCLOG("NarratorManager: Sequence with ID '%s' not found", sequenceId.c_str());
        return;
    }
    
    // 如果当前正在播放且不强制播放，则不执行
    if (isPlaying() && !forcePlay) {
        CCLOG("NarratorManager: Already playing a narrative, skipping sequence request");
        return;
    }
    
    // 停止当前播放
    stopCurrentNarrative();
    
    // 设置序列播放状态
    _currentSequence = sequenceId;
    _currentSequenceIndex = 0;
    
    // 开始播放序列中的第一个旁白
    const auto& entryIds = it->second.entryIds;
    if (!entryIds.empty()) {
        playNarrativeInternal(entryIds[0], forcePlay);
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

// 预加载旁白
void NarratorManager::preloadNarratives(const std::vector<std::string>& narrativeIds) {
    // 预加载音频资源
    for (const auto& id : narrativeIds) {
        auto it = _narrativeEntries.find(id);
        if (it != _narrativeEntries.end()) {
            CCLOG("NarratorManager: Preloading narrative: %s", it->second.audioFile.c_str());
        }
    }
}

// 卸载未使用的旁白
void NarratorManager::unloadUnusedNarratives() {
    // 可以在这里实现资源卸载逻辑
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
        else if (eventName == "LEVEL_END" && condition.type == TriggerType::LEVEL_END) {
            if (params.find("levelId") != params.end() && 
                params.at("levelId").asString() == condition.targetId) {
                condition.hasBeenTriggered = true;
                playNarrative(narrativeId);
            }
        }
        else if (eventName == "BOSS_APPEAR" && condition.type == TriggerType::BOSS_APPEAR) {
            if (params.find("bossId") != params.end() && 
                params.at("bossId").asString() == condition.targetId) {
                condition.hasBeenTriggered = true;
                playNarrative(narrativeId);
            }
        }
        else if (eventName == "BOSS_DEFEATED" && condition.type == TriggerType::BOSS_DEFEATED) {
            if (params.find("bossId") != params.end() && 
                params.at("bossId").asString() == condition.targetId) {
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
    // 如果是序列的一部分，播放下一个
    if (!_currentSequence.empty()) {
        onSequenceEntryComplete();
    } else {
        // 清除当前播放状态
        _currentPlayingNarrative = "";
        
        // 隐藏字幕
        NotificationCenter::getInstance()->postNotification("HIDE_SUBTITLE");
    }
}

// 序列条目完成回调
void NarratorManager::onSequenceEntryComplete() {
    if (_currentSequence.empty() || _currentSequenceIndex < 0) {
        return;
    }
    
    // 查找当前序列
    auto it = _sequences.find(_currentSequence);
    if (it == _sequences.end()) {
        _currentSequence = "";
        _currentSequenceIndex = -1;
        _currentPlayingNarrative = "";
        return;
    }
    
    const auto& sequence = it->second;
    const auto& entryIds = sequence.entryIds;
    
    // 更新索引
    _currentSequenceIndex++;
    
    // 检查是否还有更多条目
    if (_currentSequenceIndex < entryIds.size()) {
        // 如果是顺序播放，添加延迟
        if (sequence.isSequential && sequence.betweenDelay > 0.0f) {
            runAction(Sequence::create(
                DelayTime::create(sequence.betweenDelay),
                CallFunc::create([this, entryIds]() {
                    playNarrativeInternal(entryIds[_currentSequenceIndex]);
                }),
                nullptr
            ));
        } else {
            playNarrativeInternal(entryIds[_currentSequenceIndex]);
        }
    } else {
        // 序列播放完成
        _currentSequence = "";
        _currentSequenceIndex = -1;
        _currentPlayingNarrative = "";
        
        // 隐藏字幕
        NotificationCenter::getInstance()->postNotification("HIDE_SUBTITLE");
    }
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
    
    // 测试Boss旁白配置
    std::vector<std::string> testBossEvents = {
        "BOSS_READY_caixukun",  // caixukun出现
        "BOSS_DEFEATED_caixukun", // caixukun被击败
        "BOSS_READY_nailong",    // nailong出现
        "BOSS_READY_maodie"      // maodie出现
    };
    
    for (const auto& eventName : testBossEvents) {
        CCLOG("测试事件: %s", eventName.c_str());
        ValueMap params;
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