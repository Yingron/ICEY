﻿#include "GameOverScene.h"
#include "MainGameScene.h"
#include "StartScene.h"
#include "cocos2d.h"
#include <iostream>
#include "AudioManager.h"
#include "LevelMusicManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

int GameOverScene::s_currentSessionDeathCount = 0;

Scene* GameOverScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

bool GameOverScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createUI();
    initKeyboardListener();

    return true;
}

std::string GameOverScene::getDeathDescription(int count)
{
    s_currentSessionDeathCount++;  // 增加当前会话的死亡次数
    if (s_currentSessionDeathCount == 1)
    {
        return "你死了";
    }
    else if (s_currentSessionDeathCount == 2)
    {
        return "第二次失败";
    }
    else if (s_currentSessionDeathCount == 3)
    {
        return "第三次失败";
    }
    else if (s_currentSessionDeathCount == 4)
    {
        return "第四次失败";
    }
    else if (s_currentSessionDeathCount == 5)
    {
        return "第五次失败";
    }
    else if (s_currentSessionDeathCount == 6)
    {
        return "第六次失败";
    }
    else if (s_currentSessionDeathCount == 7)
    {
        return "第七次失败";
    }
    else if (s_currentSessionDeathCount == 8)
    {
        return "第八次失败";
    }
    else if (s_currentSessionDeathCount == 9)
    {
        return "第九次失败";
    }
    else if (s_currentSessionDeathCount == 10)
    {
        return "第十次失败";
    }
    else if (s_currentSessionDeathCount == 11)
    {
        return "第十一次失败";
    }
    else if (s_currentSessionDeathCount == 12)
    {
        return "第十二次失败";
    }
    else if (s_currentSessionDeathCount == 13)
    {
        return "第十三次失败";
    }
    else
    {
        return "已经失败多次了...";
    }
}

void GameOverScene::createUI()
{
    LevelMusicManager::getInstance()->playBGMForLevel(LevelManager::LevelState::GAMEOVER_SCENE);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建半透明黑色背景
    m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    m_background->setPosition(origin);
    this->addChild(m_background, 0);

    // 读取并递增死亡次数
    auto userDefault = UserDefault::getInstance();
    int deathCount = userDefault->getIntegerForKey("death_count", 0);
    deathCount++;  // 本次死亡+1
    userDefault->setIntegerForKey("death_count", deathCount);
    userDefault->flush();

    // 获取死亡描述
    std::string deathDescription = getDeathDescription(deathCount);

    // 创建 Game Over 文本
    m_gameOverLabel = Label::createWithTTF(deathDescription, "fonts/forui3.ttf", 72);
    if (!m_gameOverLabel)
    {
        m_gameOverLabel = Label::createWithSystemFont("GAME OVER", "Arial", 72);
    }

    m_gameOverLabel->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.7f);
    m_gameOverLabel->setTextColor(Color4B(220, 20, 60, 255)); // 深红色
    m_gameOverLabel->enableShadow(Color4B::BLACK, Size(3, -3), 3);
    this->addChild(m_gameOverLabel, 1);

    // 按钮图片路径
    std::string btnNormal = "images/ui/start&exit_btn_normal.png";
    std::string btnPressed = "images/ui/start&exit_btn_pressed.png";

    // 重新开始按钮
    m_restartButton = Button::create(btnNormal, btnPressed);
    if (m_restartButton)
    {
        m_restartButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.5f));
        m_restartButton->addClickEventListener(CC_CALLBACK_1(GameOverScene::onRestartClicked, this));

        auto restartLabel = Label::createWithTTF(u8"重新开始", "fonts/forui2.ttf", 32);
        if (!restartLabel)
        {
            restartLabel = Label::createWithSystemFont("RESTART", "Arial", 32);
        }
        restartLabel->setTextColor(Color4B::WHITE);
        m_restartButton->setTitleLabel(restartLabel);

        this->addChild(m_restartButton, 1);
    }

    // 返回菜单按钮
    m_menuButton = Button::create(btnNormal, btnPressed);
    if (m_menuButton)
    {
        m_menuButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4f));
        m_menuButton->addClickEventListener(CC_CALLBACK_1(GameOverScene::onMenuClicked, this));

        auto menuLabel = Label::createWithTTF(u8"返回菜单", "fonts/forui2.ttf", 32);
        if (!menuLabel)
        {
            menuLabel = Label::createWithSystemFont("MAIN MENU", "Arial", 32);
        }
        menuLabel->setTextColor(Color4B::WHITE);
        m_menuButton->setTitleLabel(menuLabel);

        this->addChild(m_menuButton, 1);
    }
}

void GameOverScene::onRestartClicked(Ref* sender)
{
    log("重新开始游戏");
    AudioManager::getInstance()->playUISound("ui_button_click");
    // 切换到新的游戏场景
    auto gameScene = MainGameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
}

void GameOverScene::onMenuClicked(Ref* sender)
{
    log("返回主菜单");
    AudioManager::getInstance()->playUISound("ui_button_click");
    // 切换到开始场景
    auto startScene = StartScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, startScene));
}

void GameOverScene::initKeyboardListener()
{
    // 注册键盘监听
    m_keyboardListener = EventListenerKeyboard::create();

    // 绑定按键回调
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(GameOverScene::onKeyPressed, this);

    // 添加监听
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);
}

void GameOverScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
        case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
            // Enter 重新开始游戏
            onRestartClicked(nullptr);
            event->stopPropagation();
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
            // ESC 返回主菜单
            onMenuClicked(nullptr);
            event->stopPropagation();
            break;

        default:
            break;
    }
}

GameOverScene::~GameOverScene()
{
    // 移除键盘监听
    if (m_keyboardListener)
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}
