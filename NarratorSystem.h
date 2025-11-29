#pragma once
#ifndef __NARRATOR_SYSTEM_H__
#define __NARRATOR_SYSTEM_H__

#include "cocos2d.h"
#include "PlayerAction.h"
#include <queue>
#include <unordered_map>

class NarratorSystem : public cocos2d::Node
{
private:
    cocos2d::Label* m_subtitleLabel;
    std::queue<std::string> m_dialogueQueue;
    std::unordered_map<std::string, std::vector<std::string>> m_dialogueMap;

    float m_displayDuration;
    float m_currentDisplayTime;
    bool m_isShowingDialogue;

public:
    static NarratorSystem* create();
    virtual bool init() override;
    virtual void update(float delta) override;

    void loadDialogueData(const std::string& filename);

    // ´¥·¢¶Ô»°
    void showDialogue(const std::string& text, float duration = 3.0f);
    void triggerSceneDialogue(const std::string& sceneId);
    void triggerCombatDialogue(int enemyCount);
    void triggerPlayerActionDialogue(PlayerAction action);
    void triggerRandomDialogue(const std::string& category);

    void clearQueue();
    void skipCurrentDialogue();

private:
    void createSubtitleLabel();
    void processQueue(float delta);
    void showNextDialogue();

    std::string getRandomDialogue(const std::string& category);

    CREATE_FUNC(NarratorSystem);
};

#endif // __NARRATOR_SYSTEM_H__