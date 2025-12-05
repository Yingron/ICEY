#pragma once
// MainGameScene.h
#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "cocos2d.h"
#include "Player.h"

class MainGameScene : public cocos2d::Scene 
{
public:
    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化
    virtual bool init();

    // 更新
    void update(float delta);

    // 创建方法
    CREATE_FUNC(MainGameScene);

private:

    Player* _player;
    cocos2d::Node* _background;  // 将 Sprite* 改为 Node*
  
    // 初始化方法
    void initBackground();
    void initPlayer();
    void initInput();

    // 输入监听器
    cocos2d::EventListenerKeyboard* _keyboardListener;

    // 键盘事件回调
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif // MAINGAMESCENE_H