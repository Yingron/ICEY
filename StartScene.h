#pragma once
#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class StartScene : public cocos2d::Scene
{
private:
    cocos2d::ui::Button* m_startButton;
    cocos2d::ui::Button* m_exitButton;
    cocos2d::Label* m_titleLabel;

public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(StartScene);

private:
    void onStartClicked(cocos2d::Ref* sender);
    void onExitClicked(cocos2d::Ref* sender);
    void createUI();
    //void loadSceneResources();
};

#endif // __START_SCENE_H__