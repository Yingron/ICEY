#pragma once
#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PauseScene : public cocos2d::Scene
{
private:
    cocos2d::ui::Button* m_resumeButton;
    cocos2d::ui::Button* m_restartButton;
    cocos2d::ui::Button* m_exitButton;
    cocos2d::LayerColor* m_background;
    cocos2d::EventListenerKeyboard* m_keyboardListener;

public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual ~PauseScene();

    CREATE_FUNC(PauseScene);

private:
    void onResumeClicked(cocos2d::Ref* sender);
    void onRestartClicked(cocos2d::Ref* sender);
    void onExitClicked(cocos2d::Ref* sender);
    void createUI();

    void initKeyboardListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // __PAUSE_SCENE_H__