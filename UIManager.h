#pragma once
#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ICEY.h"

class UIManager : public cocos2d::Node
{
private:
    ICEY* m_player;

    // HUD ÔªËØ
    cocos2d::ui::LoadingBar* m_healthBar;
    cocos2d::ui::LoadingBar* m_skill1Cooldown;
    cocos2d::ui::LoadingBar* m_skill2Cooldown;
    cocos2d::Label* m_comboLabel;
    cocos2d::Label* m_scoreLabel;

public:
    static UIManager* create(ICEY* player);
    virtual bool init(ICEY* player);

    virtual void update(float delta) override;

    void updateHealthBar();
    void updateSkillCooldowns();
    void updateComboDisplay();
    void updateScore();

    void showGameOver(bool isWin);
    void showPauseMenu();

    void setPlayer(ICEY* player) { m_player = player; }

private:
    void createHUD();
    void createHealthBar();
    void createSkillButtons();
    void createComboDisplay();
    void createScoreDisplay();

    CREATE_FUNC(UIManager);
};

#endif // __UI_MANAGER_H__