#ifndef NARRATOR_MANAGER_H
#define NARRATOR_MANAGER_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>
#include <set>

USING_NS_CC;

// 旁白条目结构体
struct NarrativeEntry {
    std::string id;                 // 唯一标识符
    std::string audioFile;          // 音频文件路径
    std::string subtitleText;       // 字幕文本
    float duration;                 // 持续时间(秒)
    std::vector<std::string> tags;  // 标签列表(用于过滤)
    bool isSkippable;               // 是否可跳过
    float volume;                   // 音量(0.0-1.0)
    float delay;                    // 延迟播放时间(秒)
    
    NarrativeEntry() : duration(0.0f), isSkippable(true), volume(1.0f), delay(0.0f) {}
};

// 旁白序列结构体
struct NarrativeSequence {
    std::string sequenceId;         // 序列ID
    std::vector<std::string> entryIds; // 条目ID列表
    bool isSequential;              // 是否顺序播放
    float betweenDelay;             // 条目间延迟(秒)
    
    NarrativeSequence() : isSequential(true), betweenDelay(0.0f) {}
};

// 触发器条件枚举
enum class TriggerType {
    LEVEL_START,
    LEVEL_END,
    BOSS_APPEAR,
    BOSS_DEFEATED,
    PLAYER_POSITION,
    TIME_ELAPSED,
    CUSTOM_EVENT
};

// 触发器条件结构体
struct TriggerCondition {
    TriggerType type;               // 触发类型
    std::string targetId;           // 目标ID(关卡/Boss等)
    Vec2 position;                  // 玩家位置条件
    float distance;                 // 距离阈值
    float timeInSeconds;            // 时间阈值
    std::string customEventName;    // 自定义事件名称
    bool hasBeenTriggered;          // 是否已触发
    
    TriggerCondition() : type(TriggerType::CUSTOM_EVENT), distance(100.0f), 
                        timeInSeconds(0.0f), hasBeenTriggered(false) {}
};

// 旁白系统核心管理类
class NarratorManager : public Node {
private:
    static NarratorManager* _instance;
    
    // 旁白数据存储
    std::map<std::string, NarrativeEntry> _narrativeEntries;
    std::map<std::string, NarrativeSequence> _sequences;
    std::vector<pair<TriggerCondition, std::string>> _triggers;
    
    // 状态管理
    std::set<std::string> _playedNarratives;  // 已播放的旁白ID集合
    std::string _currentPlayingNarrative;         // 当前播放中的旁白ID
    std::string _currentSequence;                 // 当前播放中的序列ID
    int _currentSequenceIndex;                    // 当前序列中的索引
    
    // 配置选项
    bool _isMuted;
    float _volume;
    bool _isSubtitlesEnabled;
    
    // 私有构造函数
    NarratorManager();
    
    // 内部方法
    void loadNarrativesFromJson(const std::string& jsonFile);
    void playNarrativeInternal(const std::string& id, bool forcePlay = false);
    void onNarrativeComplete(const std::string& id);
    void onSequenceEntryComplete();
    
public:
    // 获取单例实例
    static NarratorManager* getInstance();
    
    // 初始化方法
    bool init();
    
    // 旁白播放控制
    void playNarrative(const std::string& id, bool forcePlay = false);
    void playSequence(const std::string& sequenceId, bool forcePlay = false);
    void stopCurrentNarrative();
    void stopAllNarratives();
    
    // 状态查询
    bool isPlaying() const;
    bool hasPlayedNarrative(const std::string& id) const;
    
    // 配置方法
    void setMuted(bool muted);
    bool isMuted() const;
    void setVolume(float volume);
    float getVolume() const;
    void setSubtitlesEnabled(bool enabled);
    bool isSubtitlesEnabled() const;
    
    // 触发器管理
    void addTrigger(const TriggerCondition& condition, const std::string& narrativeId);
    void checkTriggers();  // 检查所有触发器条件
    
    // 资源管理
    void preloadNarratives(const std::vector<std::string>& narrativeIds);
    void unloadUnusedNarratives();
    
    // 重置方法
    void resetPlayedNarratives();  // 重置已播放状态
    
    // 事件回调
    void onGameEvent(const std::string& eventName, const ValueMap& params = ValueMap());
    
    // 生命周期管理
    void update(float delta) override;
    virtual ~NarratorManager();
};

#endif // NARRATOR_MANAGER_H