#include "PauseScene.h"
#include "MainGameScene.h"
#include "StartScene.h"
#include "cocos2d.h"
#include<iostream>

USING_NS_CC;
using namespace cocos2d::ui;

Scene* PauseScene::createScene()
{
    try
    {
        auto scene = PauseScene::create();
        if (!scene)
        {
            throw std::runtime_error("PauseScene 创建失败");
        }
        return scene;
    }
    catch (const std::exception& e)
    {
        std::cerr << "PauseScene::createScene 错误: " << e.what() << std::endl;
        return Scene::create();
    }
}

bool PauseScene::init()
{
    if (!Scene::init())
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
        // 创建半透明背景 
        m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
        m_background->setPosition(origin);
        this->addChild(m_background, 0);
        

        // 创建标题 - 使用TTF字体显示中文
        auto titleLabel = Label::createWithTTF(u8"游戏暂停", "fonts/forui2.ttf", 64);
        // 如果TTF创建失败，使用备选方案
        if (!titleLabel || titleLabel->getContentSize().width <= 0) {
            titleLabel = Label::createWithSystemFont("GAME PAUSED", "", 64); // 英文备选
        }

        titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7f);
        titleLabel->setTextColor(Color4B::WHITE);
        titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);
        this->addChild(titleLabel, 1);

        std::string btnNormal = "images/ui/start&exit_btn_normal.png";
        std::string btnPressed = "images/ui/start&exit_btn_pressed.png";

        // 继续按钮
        m_resumeButton = Button::create(btnNormal, btnPressed);
        if (m_resumeButton)
        {
            m_resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55f));
            m_resumeButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onResumeClicked, this));

            // 按钮文字使用TTF字体
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

        // 退出按钮
        m_exitButton = Button::create(btnNormal, btnPressed);
        if (m_exitButton)
        {
            m_exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
            m_exitButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onExitClicked, this));

            auto exitLabel = Label::createWithTTF(u8"返回菜单", "fonts/forui2.ttf", 32);
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
        // 先恢复Director
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
        // 紧急恢复：强制恢复Director
        Director::getInstance()->resume();
    }
}

void PauseScene::onRestartClicked(Ref* sender)
{
    log("Restart button clicked");

    try {
        // 重要：先恢复Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

         //等待一帧确保状态恢复
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 创建新的游戏场景
            auto gameScene = MainGameScene::createScene();

            // 替换场景
            director->replaceScene(TransitionFade::create(0.5f, gameScene));
            });

        log("Game restart initiated");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onRestartClicked: " << e.what() << std::endl;
        // 紧急恢复：强制恢复并切换到游戏场景
        Director::getInstance()->resume();
        auto gameScene = MainGameScene::createScene();
        Director::getInstance()->replaceScene(gameScene);
    }
}

void PauseScene::onExitClicked(Ref* sender)
{
    log("Exit to menu clicked");

    try {
        // 重要：先恢复Director
        auto director = Director::getInstance();
        if (director->isPaused()) 
        {
            director->resume();
        }

        // 等待一帧确保状态恢复
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 创建开始场景
            auto startScene = StartScene::createScene();

            // 替换场景（使用过渡效果）
            director->replaceScene(TransitionFade::create(0.5f, startScene));
            });

        log("Returning to main menu");
    }
    catch (const std::exception& e) {
        std::cerr << "Error in onExitClicked: " << e.what() << std::endl;
        // 紧急恢复：强制恢复并切换到开始场景
        Director::getInstance()->resume();
        auto startScene = StartScene::createScene();
        Director::getInstance()->replaceScene(startScene);
    }
}

PauseScene::~PauseScene()
{
    // 清理键盘监听器
    if (m_keyboardListener) 
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}

void PauseScene::initKeyboardListener()
{
    log("=== 初始化暂停场景键盘监听 ===");

    // 如果已有监听器，先移除
    if (m_keyboardListener) {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }

    // 创建新的键盘监听器
    m_keyboardListener = EventListenerKeyboard::create();

    // 设置按键按下回调
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);

    // 添加监听器到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);

    log("键盘监听器已注册，现在可以按ESC键了");
}

void PauseScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)//按esc结束暂停
    {
        onResumeClicked(nullptr);  // 直接调用恢复函数
        event->stopPropagation();
        return;
    }
}