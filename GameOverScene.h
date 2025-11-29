#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameOverScene : public cocos2d::Scene
{
private:
    bool m_isWin;
    cocos2d::Label* m_resultLabel;
    cocos2d::ui::Button* m_restartButton;
    cocos2d::ui::Button* m_menuButton;

public:
    static cocos2d::Scene* createScene(bool isWin);
    virtual bool init() override;
    void setResult(bool isWin) { m_isWin = isWin; }

    CREATE_FUNC(GameOverScene);

private:
    void onRestartClicked(cocos2d::Ref* sender);
    void onMenuClicked(cocos2d::Ref* sender);
    void createUI();
};

#endif // __GAME_OVER_SCENE_H__