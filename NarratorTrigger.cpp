#include "NarratorTrigger.h"
#include "NarratorManager.h"
#include "cocos2d.h"
#include <string>

USING_NS_CC;

// 创建NarratorTrigger实例
NarratorTrigger* NarratorTrigger::create() {
    auto trigger = new (std::nothrow) NarratorTrigger();
    if (trigger && trigger->init()) {
        trigger->autorelease();
        return trigger;
    }
    CC_SAFE_DELETE(trigger);
    return nullptr;
}

// 构造函数
NarratorTrigger::NarratorTrigger()
    : _triggerType(TriggerType::NONE)
    , _targetId("")
    , _narrativeId("")
    , _customEventName("")
    , _condition(TriggerType::NONE)
    , _hasBeenTriggered(false)
    , _repeats(false)
    , _delay(0.0f) {
}

// 析构函数
NarratorTrigger::~NarratorTrigger() {
    // 移除所有事件监听
    NotificationCenter::getInstance()->removeAllObservers(this);
}

// 初始化方法
bool NarratorTrigger::init() {
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

// 设置触发器类型
void NarratorTrigger::setTriggerType(TriggerType type) {
    _triggerType = type;
}

// 获取触发器类型
TriggerType NarratorTrigger::getTriggerType() const {
    return _triggerType;
}

// 设置目标ID
void NarratorTrigger::setTargetId(const std::string& id) {
    _targetId = id;
}

// 获取目标ID
const std::string& NarratorTrigger::getTargetId() const {
    return _targetId;
}

// 设置触发条件
void NarratorTrigger::setCondition(const TriggerCondition& condition) {
    _condition = condition;
    _triggerType = condition.type;
    _targetId = condition.targetId;
    _customEventName = condition.customEventName;
}

// 获取触发条件
const TriggerCondition& NarratorTrigger::getCondition() const {
    return _condition;
}

// 设置旁白ID
void NarratorTrigger::setNarrativeId(const std::string& id) {
    _narrativeId = id;
}

// 获取旁白ID
const std::string& NarratorTrigger::getNarrativeId() const {
    return _narrativeId;
}

// 设置是否可重复触发
void NarratorTrigger::setRepeats(bool repeats) {
    _repeats = repeats;
}

// 获取是否可重复触发
bool NarratorTrigger::getRepeats() const {
    return _repeats;
}

// 设置延迟触发时间
void NarratorTrigger::setDelay(float delay) {
    _delay = delay;
}

// 获取延迟触发时间
float NarratorTrigger::getDelay() const {
    return _delay;
}

// 检查是否已经触发过
bool NarratorTrigger::hasBeenTriggered() const {
    return _hasBeenTriggered;
}

// 触发旁白
void NarratorTrigger::trigger() {
    // 如果已经触发过且不重复触发，则跳过
    if (_hasBeenTriggered && !_repeats) {
        return;
    }
    
    // 标记为已触发
    _hasBeenTriggered = true;
    
    // 如果有延迟，则延迟触发
    if (_delay > 0.0f) {
        runAction(Sequence::create(
            DelayTime::create(_delay),
            CallFunc::create([this]() {
                // 调用NarratorManager播放旁白
                if (!_narrativeId.empty()) {
                    NarratorManager::getInstance()->playNarrative(_narrativeId);
                }
            }),
            nullptr
        ));
    } else {
        // 直接触发
        if (!_narrativeId.empty()) {
            NarratorManager::getInstance()->playNarrative(_narrativeId);
        }
    }
}

// 重置触发器状态
void NarratorTrigger::reset() {
    _hasBeenTriggered = false;
    stopAllActions();
}

// 添加游戏事件监听
void NarratorTrigger::addGameEventListener() {
    // 根据触发器类型添加相应的事件监听
    switch (_triggerType) {
        case TriggerType::LEVEL_START:
            NotificationCenter::getInstance()->addObserver(
                this, 
                callfuncO_selector(NarratorTrigger::onLevelStartEvent), 
                "LEVEL_START", 
                nullptr
            );
            break;
            
        case TriggerType::LEVEL_END:
            NotificationCenter::getInstance()->addObserver(
                this, 
                callfuncO_selector(NarratorTrigger::onLevelEndEvent), 
                "LEVEL_END", 
                nullptr
            );
            break;
            
        case TriggerType::BOSS_APPEAR:
            NotificationCenter::getInstance()->addObserver(
                this, 
                callfuncO_selector(NarratorTrigger::onBossAppearEvent), 
                "BOSS_APPEAR", 
                nullptr
            );
            break;
            
        case TriggerType::BOSS_DEFEATED:
            NotificationCenter::getInstance()->addObserver(
                this, 
                callfuncO_selector(NarratorTrigger::onBossDefeatedEvent), 
                "BOSS_DEFEATED", 
                nullptr
            );
            break;
            
        case TriggerType::CUSTOM_EVENT:
            if (!_customEventName.empty()) {
                NotificationCenter::getInstance()->addObserver(
                    this, 
                    callfuncO_selector(NarratorTrigger::onCustomEvent), 
                    _customEventName.c_str(), 
                    nullptr
                );
            }
            // 为BOSS模式变化添加通用监听
            NotificationCenter::getInstance()->addObserver(
                this, 
                callfuncO_selector(NarratorTrigger::onBossModeChangeEvent), 
                "BOSS_MODE_CHANGE", 
                nullptr
            );
            break;
            
        default:
            break;
    }
}

// 移除游戏事件监听
void NarratorTrigger::removeGameEventListener() {
    // 移除所有事件监听
    NotificationCenter::getInstance()->removeAllObservers(this);
}

// 关卡开始事件回调
void NarratorTrigger::onLevelStartEvent(cocos2d::Ref* sender) {
    // 检查参数
    auto params = static_cast<ValueMap*>(sender);
    if (!params || params->find("levelId") == params->end()) {
        return;
    }
    
    // 检查目标ID是否匹配
    if (params->at("levelId").asString() == _targetId) {
        trigger();
    }
}

// 关卡结束事件回调
void NarratorTrigger::onLevelEndEvent(cocos2d::Ref* sender) {
    // 检查参数
    auto params = static_cast<ValueMap*>(sender);
    if (!params || params->find("levelId") == params->end()) {
        return;
    }
    
    // 检查目标ID是否匹配
    if (params->at("levelId").asString() == _targetId) {
        trigger();
    }
}

// Boss出现事件回调
void NarratorTrigger::onBossAppearEvent(cocos2d::Ref* sender) {
    // 检查参数
    auto params = static_cast<ValueMap*>(sender);
    if (!params || params->find("bossId") == params->end()) {
        return;
    }
    
    // 检查目标ID是否匹配
    if (params->at("bossId").asString() == _targetId) {
        trigger();
    }
}

// Boss被击败事件回调
void NarratorTrigger::onBossDefeatedEvent(cocos2d::Ref* sender) {
    // 检查参数
    auto params = static_cast<ValueMap*>(sender);
    if (!params || params->find("bossId") == params->end()) {
        return;
    }
    
    // 检查目标ID是否匹配
    if (params->at("bossId").asString() == _targetId) {
        trigger();
    }
}

// Boss模式变化事件回调
void NarratorTrigger::onBossModeChangeEvent(cocos2d::Ref* sender) {
    // 检查参数
    auto params = static_cast<ValueMap*>(sender);
    if (!params || params->find("bossId") == params->end() || 
        params->find("mode") == params->end()) {
        return;
    }
    
    std::string bossId = params->at("bossId").asString();
    std::string mode = params->at("mode").asString();
    
    // 检查是否是当前关注的boss和模式
    if (bossId == _targetId && !_customEventName.empty()) {
        // 构建期望的事件名称格式：BOSS_[MODE]_[BOSSID]
        std::string expectedEventName = "BOSS_" + mode + "_" + bossId;
        
        // 如果自定义事件名称匹配，则触发
        if (_customEventName == expectedEventName) {
            trigger();
        }
    }
}

// 自定义事件回调
void NarratorTrigger::onCustomEvent(cocos2d::Ref* sender) {
    // 直接触发
    trigger();
}