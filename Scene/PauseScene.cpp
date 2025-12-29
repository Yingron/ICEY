#include "PauseScene.h"
#include "MainGameScene.h"
#include "StartScene.h"
#include "cocos2d.h"
#include <iostream>
#include "AudioManager.h"
#include "LevelMusicManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* PauseScene::createScene()
{
    try
    {
        auto scene = Scene::create();
        auto pauseLayer = PauseScene::create();
        if (pauseLayer)
        {
            scene->addChild(pauseLayer);
        }
        return scene;
    }
    catch (const std::exception& e)
    {
        std::cerr << "PauseScene::createScene 创建异常: " << e.what() << std::endl;
        return Scene::create();
    }
}

bool PauseScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createUI();
    initKeyboardListener();
    return true;
}

void PauseScene::createUI()
{
    try
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        // 添加半透明遮罩背景
        m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
        m_background->setPosition(origin);
        this->addChild(m_background, 0);
        

        // 标题文字 - 优先使用 TTF 以正确显示中文
        auto titleLabel = Label::createWithTTF(u8"游戏暂停", "fonts/forui3.ttf", 64);
        // 如果 TTF 加载失败，则使用系统字体兜底
        if (!titleLabel || titleLabel->getContentSize().width <= 0) {
            titleLabel = Label::createWithSystemFont("GAME PAUSED", "", 64); // 英文兜底
        }

        titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7f);
        titleLabel->setTextColor(Color4B::WHITE);
        titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);
        this->addChild(titleLabel, 1);

        std::string btnNormal = "images/ui/start&exit_btn_normal.png";
        std::string btnPressed = "images/ui/start&exit_btn_pressed.png";

        // 继续游戏按钮
        m_resumeButton = Button::create(btnNormal, btnPressed);
        if (m_resumeButton)
        {
            m_resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55f));
            m_resumeButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onResumeClicked, this));

            // 按钮标题，优先使用 TTF
            auto resumeLabel = Label::createWithTTF(u8"继续游戏", "fonts/forui2.ttf", 32);
            if (!resumeLabel || resumeLabel->getContentSize().width <= 0) {
                resumeLabel = Label::createWithSystemFont("RESUME", "", 32);
            }
            resumeLabel->setTextColor(Color4B::WHITE);
            m_resumeButton->setTitleLabel(resumeLabel);
            this->addChild(m_resumeButton, 1);
        }
        else
        {
            throw std::runtime_error("Continue button creation failed: " + btnNormal);
        }

        // 重新开始按钮
        m_restartButton = Button::create(btnNormal, btnPressed);
        if (m_restartButton)
        {
            m_restartButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.45f));
            m_restartButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onRestartClicked, this));

            auto restartLabel = Label::createWithTTF(u8"重新开始", "fonts/forui2.ttf", 32);
            if (!restartLabel || restartLabel->getContentSize().width <= 0)
            {
                restartLabel = Label::createWithSystemFont("RESTART", "", 32);
            }
            restartLabel->setTextColor(Color4B::WHITE);
            m_restartButton->setTitleLabel(restartLabel);
            this->addChild(m_restartButton, 1);
        }
        else
        {
            throw std::runtime_error("Restart button creation failed: " + btnNormal);
        }

        // 返回主菜单按钮
        m_exitButton = Button::create(btnNormal, btnPressed);
        if (m_exitButton)
        {
            m_exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
            m_exitButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onExitClicked, this));

            auto exitLabel = Label::createWithTTF(u8"返回主菜单", "fonts/forui2.ttf", 32);
            if (!exitLabel || exitLabel->getContentSize().width <= 0) {
                exitLabel = Label::createWithSystemFont("MAIN MENU", "", 32);
            }
            exitLabel->setTextColor(Color4B::WHITE);
            m_exitButton->setTitleLabel(exitLabel);
            this->addChild(m_exitButton, 1);
        }
        else
        {
            throw std::runtime_error("Exit button creation failed: " + btnNormal);
        }
        LevelMusicManager::getInstance()->playBGMForLevel(LevelManager::LevelState::PAUSE_SCENE);
    }
    catch (const std::exception& e)
    {
        std::cerr << "PauseScene::createUI Error: " << e.what() << std::endl;
        throw;
    }
}


void PauseScene::onResumeClicked(Ref* sender)//***
{
    log("Resume button clicked");

    try {
        AudioManager::getInstance()->playUISound("ui_button_click");
        // 确保恢复 Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

        // 弹出当前场景
        director->popScene();

        log("Game resumed successfully");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onResumeClicked: " << e.what() << std::endl;
        // 出现异常仍强制恢复 Director
        Director::getInstance()->resume();
    }
}

void PauseScene::onRestartClicked(Ref* sender)
{
    log("Restart button clicked");

    try {
        AudioManager::getInstance()->playUISound("ui_button_click");
        // 需要先恢复 Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

         // 延迟到下一帧确保状态更新
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 创建新的游戏场景
            auto gameScene = MainGameScene::createScene();

            // 切换场景
            director->replaceScene(TransitionFade::create(0.5f, gameScene));
            });

        log("Game restart initiated");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onRestartClicked: " << e.what() << std::endl;
        // 异常时强制恢复并直接切换到新游戏场景
        Director::getInstance()->resume();
        auto gameScene = MainGameScene::createScene();
        Director::getInstance()->replaceScene(gameScene);
    }
}

void PauseScene::onExitClicked(Ref* sender)
{
    log("Exit to menu clicked");

    try {
        AudioManager::getInstance()->playUISound("ui_button_click");
        // 需要先恢复 Director
        auto director = Director::getInstance();
        if (director->isPaused()) 
        {
            director->resume();
        }

        // 延迟到下一帧确保状态更新
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 创建开始菜单场景
            auto startScene = StartScene::createScene();

            // 切换场景并使用淡入淡出效果
            director->replaceScene(TransitionFade::create(0.5f, startScene));
            });

        log("Returning to main menu");
    }
    catch (const std::exception& e) {
        std::cerr << "Error in onExitClicked: " << e.what() << std::endl;
        // 异常时强制恢复并直接切换到开始场景
        Director::getInstance()->resume();
        auto startScene = StartScene::createScene();
        Director::getInstance()->replaceScene(startScene);
    }
}

PauseScene::~PauseScene()
{
    // 释放键盘监听器
    if (m_keyboardListener) 
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}

void PauseScene::initKeyboardListener()
{
    log("=== 初始化暂停场景的键盘监听 ===");

    // 如果之前已有监听，先移除
    if (m_keyboardListener) {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }

    // 创建新的键盘事件监听器
    m_keyboardListener = EventListenerKeyboard::create();

    // 绑定按键回调
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);

    // 注册监听到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);

    log("键盘监听器已注册，可使用 ESC 关闭暂停界面");
}

void PauseScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)//按 ESC 继续游戏
    {
        onResumeClicked(nullptr);  // 直接恢复游戏
        event->stopPropagation();
        return;
    }
}

// 暂停场景显示时
void PauseScene::onEnter() {
    Layer::onEnter();

    // 暂停当前关卡 BGM 并播放暂停界面 BGM
    LevelMusicManager::getInstance()->pauseCurrentBGM();
    AudioManager::getInstance()->playBGM("bgm_pause");
}

// 暂停场景关闭时
void PauseScene::onExit() {
    // 停止暂停界面 BGM 并恢复游戏 BGM
    AudioManager::getInstance()->stopBGM();
    LevelMusicManager::getInstance()->resumeCurrentBGM();

    Layer::onExit();
}
