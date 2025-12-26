#ifndef NARRATOR_TRIGGER_H
#define NARRATOR_TRIGGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

// 触发器类型枚举
enum class TriggerType {
    NONE,             // 无触发类型
    LEVEL_START,      // 关卡开始
    LEVEL_END,        // 关卡结束
    BOSS_APPEAR,      // Boss出现
    BOSS_DEFEATED,    // Boss被击败
    CUSTOM_EVENT      // 自定义事件触发
};

// 触发器条件结构体
struct TriggerCondition {
    TriggerType type;             // 触发器类型
    std::string targetId;         // 目标ID(关卡ID、BossID等)
    std::string customEventName;  // 自定义事件名称
    
    // 构造函数
    TriggerCondition(TriggerType t = TriggerType::NONE)
        : type(t), targetId(""), customEventName("") {
    }
};

/**
 * NarratorTrigger 类
 * 负责监听游戏事件并触发相应的旁白
 */
class NarratorTrigger : public Node {
public:
    // 创建NarratorTrigger实例
    static NarratorTrigger* create();
    
    // 析构函数
    virtual ~NarratorTrigger();
    
    // 设置触发器类型
    void setTriggerType(TriggerType type);
    
    // 获取触发器类型
    TriggerType getTriggerType() const;
    
    // 设置目标ID
    void setTargetId(const std::string& id);
    
    // 获取目标ID
    const std::string& getTargetId() const;
    
    // 设置触发条件
    void setCondition(const TriggerCondition& condition);
    
    // 获取触发条件
    const TriggerCondition& getCondition() const;
    
    // 设置旁白ID
    void setNarrativeId(const std::string& id);
    
    // 获取旁白ID
    const std::string& getNarrativeId() const;
    
    // 设置是否可重复触发
    void setRepeats(bool repeats);
    
    // 获取是否可重复触发
    bool getRepeats() const;
    
    // 设置延迟触发时间
    void setDelay(float delay);
    
    // 获取延迟触发时间
    float getDelay() const;
    
    // 检查是否已经触发过
    bool hasBeenTriggered() const;
    
    // 触发旁白
    void trigger();
    
    // 重置触发器状态
    void reset();
    
    // 添加游戏事件监听
    void addGameEventListener();
    
    // 移除游戏事件监听
    void removeGameEventListener();
    
    // 事件回调函数
    void onLevelStartEvent(cocos2d::Ref* sender);
    void onLevelEndEvent(cocos2d::Ref* sender);
    void onBossAppearEvent(cocos2d::Ref* sender);
    void onBossDefeatedEvent(cocos2d::Ref* sender);
    void onBossModeChangeEvent(cocos2d::Ref* sender); // 新增：Boss模式变化事件回调
    void onCustomEvent(cocos2d::Ref* sender);
    
protected:
    // 初始化方法
    virtual bool init() override;
    
private:
    TriggerType _triggerType;        // 触发器类型
    std::string _targetId;           // 目标ID
    std::string _narrativeId;        // 旁白ID
    std::string _customEventName;    // 自定义事件名称
    TriggerCondition _condition;     // 触发条件
    bool _hasBeenTriggered;          // 是否已触发
    bool _repeats;                   // 是否可重复触发
    float _delay;                    // 延迟触发时间(秒)
    
    // 构造函数
    NarratorTrigger();
};

#endif // NARRATOR_TRIGGER_H