#pragma once
#ifndef __DIALOGUE_TRIGGER_H__
#define __DIALOGUE_TRIGGER_H__

#include "cocos2d.h"

class DialogueTrigger : public cocos2d::Node
{
private:
    cocos2d::Rect m_triggerArea;
    std::string m_dialogueKey;
    bool m_hasTriggered;
    bool m_isOneTime;

public:
    static DialogueTrigger* create(const cocos2d::Rect& area, const std::string& dialogueKey, bool oneTime = true);
    virtual bool init(const cocos2d::Rect& area, const std::string& dialogueKey, bool oneTime);

    bool checkTrigger(const cocos2d::Vec2& position);
    void trigger();
    void reset();

    std::string getDialogueKey() const { return m_dialogueKey; }
    bool hasTriggered() const { return m_hasTriggered; }

    CREATE_FUNC(DialogueTrigger);
};

#endif // __DIALOGUE_TRIGGER_H__