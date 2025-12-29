#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameOverScene : public cocos2d::Layer
{
public:
    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化
    virtual bool init() override;

    CREATE_FUNC(GameOverScene);

private:
    static int s_currentSessionDeathCount;  // 当前会话的死亡次数
    // UI 元素
    cocos2d::Label* m_gameOverLabel;
    cocos2d::ui::Button* m_restartButton;
    cocos2d::ui::Button* m_menuButton;
    cocos2d::LayerColor* m_background;

    // 键盘事件监听
    cocos2d::EventListenerKeyboard* m_keyboardListener;

    // 构建 UI
    void createUI();

    std::string getDeathDescription(int count);

    // 按钮回调
    void onRestartClicked(cocos2d::Ref* sender);
    void onMenuClicked(cocos2d::Ref* sender);

    // 键盘处理
    void initKeyboardListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 析构
    virtual ~GameOverScene();
};

#endif // __GAME_OVER_SCENE_H__
