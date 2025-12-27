#ifndef NARRATOR_MANAGER_H
#define NARRATOR_MANAGER_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <utility>

USING_NS_CC;

// 旁白系统管理器类 - 精简版
class NarratorManager : public Node {
private:
    // 单例模式
    static NarratorManager* _instance;
    
    // 构造函数
    NarratorManager();
    
    // 存储旁白条目
    struct NarrativeEntry {
        std::string id;                    // 唯一标识符
        std::string audioFile;             // 音频文件路径
        std::string subtitleText;          // 字幕文本
        float duration = 0.0f;             // 持续时间（秒）
        bool isSkippable = true;           // 是否可跳过
        float delay = 0.0f;                // 延迟播放时间
    };
    
    // 触发器条件
    struct TriggerCondition {
        // 触发器类型枚举
        enum class Type {
            LEVEL_START,   // 关卡开始时触发
            CUSTOM_EVENT   // 自定义事件触发
        };  
        
        Type type;                       // 触发器类型
        std::string targetId;            // 目标ID（如关卡ID）
        std::string customEventName;     // 自定义事件名称
        bool hasBeenTriggered = false;   // 是否已触发
    };
    
    // 成员变量
    std::unordered_map<std::string, NarrativeEntry> _narrativeEntries;  // 旁白条目映射
    std::vector<std::pair<TriggerCondition, std::string>> _triggers; // 触发器与旁白ID的映射
    std::set<std::string> _playedNarratives;       // 已播放过的旁白ID集合
    std::unordered_map<std::string, std::vector<std::string>> _sequences; // 序列映射
    
    std::string _currentPlayingNarrative;  // 当前播放的旁白ID
    std::string _currentSequence;          // 当前播放序列ID
    int _currentSequenceIndex;             // 当前序列索引
    bool _isMuted;                         // 是否静音
    float _volume;                         // 音量（0.0-1.0）
    bool _isSubtitlesEnabled;              // 是否启用字幕
    
    // 内部方法
    void playNarrativeInternal(const std::string& id, bool forcePlay);
    
public:
    // 获取单例实例
    static NarratorManager* getInstance();
    
    // 初始化方法
    virtual bool init() override;
    
    // 从JSON配置文件加载旁白数据
    void loadNarrativesFromJson(const std::string& jsonFile = "config/narratives.json");
    
    // 播放指定ID的旁白
    void playNarrative(const std::string& id, bool forcePlay = false);
    
    // 停止当前播放的旁白
    void stopCurrentNarrative();
    
    // 停止所有旁白
    void stopAllNarratives();
    
    // 检查是否正在播放
    bool isPlaying() const;
    
    // 检查旁白是否已经播放过
    bool hasPlayedNarrative(const std::string& id) const;
    
    // 设置静音状态
    void setMuted(bool muted);
    
    // 获取静音状态
    bool isMuted() const;
    
    // 设置音量
    void setVolume(float volume);
    
    // 获取音量
    float getVolume() const;
    
    // 设置字幕是否启用
    void setSubtitlesEnabled(bool enabled);
    
    // 获取字幕是否启用
    bool isSubtitlesEnabled() const;
    
    // 添加触发器
    void addTrigger(const TriggerCondition& condition, const std::string& narrativeId);
    
    // 检查触发器
    void checkTriggers();
    
    // 重置已播放的旁白状态
    void resetPlayedNarratives();
    
    // 处理游戏事件
    void onGameEvent(const std::string& eventName, const ValueMap& params = ValueMap());
    
    // 旁白完成回调
    void onNarrativeComplete(const std::string& id);
    
    // 更新方法
    virtual void update(float delta) override;
    
    // 析构函数
    virtual ~NarratorManager();
    
    // 测试功能：测试所有旁白触发
    bool testNarrationTriggers();
    
    // 重置所有触发器状态（用于测试）
    void resetAllTriggers();
};

#endif // NARRATOR_MANAGER_H
