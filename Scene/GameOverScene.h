#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameOverScene : public cocos2d::Layer
{
public:
    // 锟斤拷锟斤拷锟斤拷锟斤拷
    static cocos2d::Scene* createScene();

    // 锟斤拷始锟斤拷
    virtual bool init() override;

    CREATE_FUNC(GameOverScene);

private:
    static int s_currentSessionDeathCount;  // 锟斤拷锟轿会话锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    // UI元锟斤拷
    cocos2d::Label* m_gameOverLabel;
    cocos2d::ui::Button* m_restartButton;
    cocos2d::ui::Button* m_menuButton;
    cocos2d::LayerColor* m_background;

    // 锟斤拷锟教硷拷锟斤拷锟斤拷
    cocos2d::EventListenerKeyboard* m_keyboardListener;

    // 锟斤拷锟斤拷UI
    void createUI();

    std::string GameOverScene::getDeathDescription(int count);

    // 锟斤拷钮锟截碉拷
    void onRestartClicked(cocos2d::Ref* sender);
    void onMenuClicked(cocos2d::Ref* sender);

    // 锟斤拷锟斤拷锟铰硷拷
    void initKeyboardListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 锟斤拷锟斤拷锟斤拷锟斤拷
    virtual ~GameOverScene();
};

#endif // __GAME_OVER_SCENE_H__