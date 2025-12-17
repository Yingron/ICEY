// MainGameScene.h
#pragma once
#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "Level1SceneBackground.h"
#include "LevelManager.h"
#include "ItemManager.h"
#include "Item.h"
#include"HudLayer.h"

class MainGameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    CREATE_FUNC(MainGameScene);

private:
    Player* _player;
    SceneBackground* _currentBackground;

    HudLayer* _hudLayer;  // 添加 HUD 层指针hy*****

    // 关卡管理
    LevelManager* _levelManager;

    bool _isGameOver;//hy*****
    void checkPlayerHealth();
    void showGameOver();

    // 摄像机相关
    float _cameraOffsetX;
    float _screenWidth;
    float _worldWidth;
    float _minCameraX;
    float _maxCameraX;

    // 关卡切换相关
    bool _isTransitioning;
    float _transitionTimer;
    const float TRANSITION_DURATION = 1.0f;

    // 物品系统
    std::vector<Item*> _levelItems;
    cocos2d::Label* _collectionLabel;

    // 初始化方法
    void initBackground();
    void initPlayer();
    void initInput();
    void initDebugUI();
    void initCamera();
    void initItems();
    void initHud();//hy*****
    void showCollectionUI();

    // 更新方法
    void updateCamera(float delta);
    void checkLevelTransition(float delta);
    void switchToNextLevel();
    void checkItemCollisions(float delta);
    void updateCollectionUI();

    // 辅助函数
    bool isPlayerAtRightBoundary();
    void placeItemAt(const std::string& itemId, float worldX, float worldY);

    // 输入处理监听器
    cocos2d::EventListenerKeyboard* _keyboardListener;
    cocos2d::Label* _debugLabel;
    cocos2d::Label* _levelLabel;

    // 键盘事件回调
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 物理碰撞回调
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    // 场景生命周期函数
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // MAINGAMESCENE_H