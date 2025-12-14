// MainGameScene.h
#pragma once
#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "Level1SceneBackground.h"  // 包含SceneBackground的头文件
#include "LevelManager.h"

class MainGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    CREATE_FUNC(MainGameScene);

private:
    Player* _player;
    SceneBackground* _currentBackground;  // 使用SceneBackground类

    // 关卡管理
    LevelManager* _levelManager;

    // 摄像机相关
    float _cameraOffsetX;
    float _screenWidth;
    float _worldWidth;
    float _minCameraX;
    float _maxCameraX;

    // 关卡切换相关
    bool _isTransitioning;
    float _transitionTimer;
    const float TRANSITION_DURATION = 1.0f; // 关卡切换持续时间（秒）

    // 初始化方法
    void initBackground();
    void initPlayer();
    void initInput();
    void initDebugUI();
    void initCamera();

    // 更新方法
    void updateCamera(float delta);
    void checkLevelTransition(float delta);
    void switchToNextLevel();

    // 输入处理监听器
    cocos2d::EventListenerKeyboard* _keyboardListener;
    cocos2d::Label* _debugLabel;
    cocos2d::Label* _levelLabel;

    // 键盘事件回调
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 辅助函数
    void forceUpdateBackground();  // 强制更新背景
    bool isPlayerAtRightBoundary();  // 检查玩家是否到达右边界
};

#endif // MAINGAMESCENE_H