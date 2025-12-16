#pragma once
#ifndef __SCENE_TRIGGER_H__
#define __SCENE_TRIGGER_H__

#include "cocos2d.h"

class SceneTrigger : public cocos2d::Node
{

public:
    static SceneTrigger* create(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene = "");
    virtual bool init(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene);
    SceneTrigger* SceneTrigger::createWithRect(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene);
    void setup(const cocos2d::Rect& rect, const std::string& eventName);
    void checkTrigger(cocos2d::Node* player);
    void executeEvent();
    void reset();

    // 其它成员函数声明
    bool checkActivation(const cocos2d::Vec2& position);
    void activate();
    std::string getTriggerType() const { return m_triggerType; }
    std::string getTargetScene() const { return m_targetScene; }
    bool isActivated() const { return m_isActivated; }

    CREATE_FUNC(SceneTrigger);


protected:
    cocos2d::Rect m_triggerRect;
    std::string m_triggerType;
    std::string m_targetScene;
    std::string m_eventName;
    bool m_isActivated = false;
    bool m_isActive = true;
    bool m_isTriggered = false;
};

#endif // __SCENE_TRIGGER_H__