#pragma once
#ifndef __SCENE_TRIGGER_H__
#define __SCENE_TRIGGER_H__

#include "cocos2d.h"

class SceneTrigger : public cocos2d::Node
{
private:
    cocos2d::Rect m_triggerRect;
    std::string m_triggerType;
    std::string m_targetScene;
    bool m_isActivated;

public:
    static SceneTrigger* create(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene = "");
    virtual bool init(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene);

    bool checkActivation(const cocos2d::Vec2& position);
    void activate();

    // »ñÈ¡Æ÷
    std::string getTriggerType() const { return m_triggerType; }
    std::string getTargetScene() const { return m_targetScene; }
    bool isActivated() const { return m_isActivated; }

    CREATE_FUNC(SceneTrigger);
};

#endif // __SCENE_TRIGGER_H__