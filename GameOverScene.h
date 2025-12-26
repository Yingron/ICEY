#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameOverScene : public cocos2d::Layer
{
public:
    // ��������
    static cocos2d::Scene* createScene();

    // ��ʼ��
    virtual bool init() override;

    CREATE_FUNC(GameOverScene);

private:
    static int s_currentSessionDeathCount;  // ���λỰ����������
    // UIԪ��
    cocos2d::Label* m_gameOverLabel;
    cocos2d::ui::Button* m_restartButton;
    cocos2d::ui::Button* m_menuButton;
    cocos2d::LayerColor* m_background;

    // ���̼�����
    cocos2d::EventListenerKeyboard* m_keyboardListener;

    // ����UI
    void createUI();

    std::string GameOverScene::getDeathDescription(int count);

    // ��ť�ص�
    void onRestartClicked(cocos2d::Ref* sender);
    void onMenuClicked(cocos2d::Ref* sender);

    // �����¼�
    void initKeyboardListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // ��������
    virtual ~GameOverScene();
};

#endif // __GAME_OVER_SCENE_H__