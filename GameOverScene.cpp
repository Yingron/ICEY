#include "GameOverScene.h"
#include "MainGameScene.h"
#include "StartScene.h"
#include "cocos2d.h"
#include <iostream>
#include"AudioManager.h"

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
    if (!Scene::init())
    {
        return false;
    }

    createUI();
    initKeyboardListener();

    return true;
}


std::string GameOverScene::getDeathDescription(int count)
{
    s_currentSessionDeathCount++;  // 增加本次会话的计数
    if (s_currentSessionDeathCount == 1) {
        return u8"你死亡了";
    }
    else if (s_currentSessionDeathCount == 2) {
        return u8"你又死亡了";
    }
    else if (s_currentSessionDeathCount == 3) {
        return u8"你双死亡了";
    }
    else if (s_currentSessionDeathCount == 4) {
        return u8"你叒死亡了";
    }
    else if (s_currentSessionDeathCount == 5) {
        return u8"你叕死亡了";
    }
    else if (s_currentSessionDeathCount == 6) {
        return u8"你叕又死亡了";
    }
    else if (s_currentSessionDeathCount == 7) {
        return u8"你叕双死亡了";
    }
    else if (s_currentSessionDeathCount == 8) {
        return u8"你叕叒死亡了";
    }
    else if (s_currentSessionDeathCount == 9) {
        return u8"你叕叕死亡了";
    }
    else if (s_currentSessionDeathCount == 10) {
        return u8"你叕叕又死亡了";
    }
    else if (s_currentSessionDeathCount == 11) {
        return u8"你叕叕双死亡了";
    }
    else if (s_currentSessionDeathCount == 12) {
        return u8"你叕叕叒死亡了";
    }
    else if (s_currentSessionDeathCount == 13) {
        return u8"你叕叕叕死亡了";
    }
    else {
        
        return u8"已经记不清你的死亡次数了";
    }
}

void GameOverScene::createUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建半透明黑色背景
    m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    m_background->setPosition(origin);
    this->addChild(m_background, 0);


    // 获取并增加死亡次数
    auto userDefault = UserDefault::getInstance();
    int deathCount = userDefault->getIntegerForKey("death_count", 0);
    deathCount++;  // 增加本次死亡
    userDefault->setIntegerForKey("death_count", deathCount);
    userDefault->flush();

    // 获取死亡描述
    std::string deathDescription = getDeathDescription(deathCount);

    // 创建Game Over标题
    m_gameOverLabel = Label::createWithTTF(deathDescription, "fonts/forui3.ttf", 72);
    if (!m_gameOverLabel)
    {
        m_gameOverLabel = Label::createWithSystemFont("GAME OVER", "Arial", 72);
    }

    m_gameOverLabel->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.7f);
    m_gameOverLabel->setTextColor(Color4B(220, 20, 60, 255)); // 红色
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

    // 主菜单按钮
    m_menuButton = Button::create(btnNormal, btnPressed);
    if (m_menuButton)
    {
        m_menuButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4f));
        m_menuButton->addClickEventListener(CC_CALLBACK_1(GameOverScene::onMenuClicked, this));

        auto menuLabel = Label::createWithTTF(u8"返回主菜单", "fonts/forui2.ttf", 32);
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
    // 创建新的游戏场景并切换
    auto gameScene = MainGameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
}

void GameOverScene::onMenuClicked(Ref* sender)
{
    log("返回主菜单");
    AudioManager::getInstance()->playUISound("ui_button_click");
    // 创建开始场景并切换
    auto startScene = StartScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, startScene));
}

void GameOverScene::initKeyboardListener()
{
    // 创建键盘监听器
    m_keyboardListener = EventListenerKeyboard::create();

    // 设置按键回调
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(GameOverScene::onKeyPressed, this);

    // 添加监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);
}

void GameOverScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
        case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
            // Enter键：重新开始游戏
            onRestartClicked(nullptr);
            event->stopPropagation();
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
            // ESC键：返回主菜单
            onMenuClicked(nullptr);
            event->stopPropagation();
            break;

        default:
            break;
    }
}

GameOverScene::~GameOverScene()
{
    // 清理键盘监听器
    if (m_keyboardListener)
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}