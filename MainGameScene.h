#pragma once
#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "cocos2d.h"
#include "ICEY.h"
#include "InputHandler.h"
#include "UIManager.h"
#include "NarratorSystem.h"
#include "Level.h"
#include "CombatSystem.h"

class MainGameScene : public cocos2d::Scene
{
private:
    ICEY* m_player;
    InputHandler* m_inputHandler;
    UIManager* m_uiManager;
    NarratorSystem* m_narrator;
    Level* m_currentLevel;
    CombatSystem* m_combatSystem;
    bool m_isPaused;

public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void update(float delta) override;

    CREATE_FUNC(MainGameScene);

    // 游戏流程控制
    void pauseGame();
    void resumeGame();
    void gameOver(bool isWin);
    void restartGame();

    // 获取器
    ICEY* getPlayer() const { return m_player; }
    Level* getCurrentLevel() const { return m_currentLevel; }
};

#endif // __MAIN_GAME_SCENE_H__