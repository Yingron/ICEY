#include "PauseScene.h"
#include "MainGameScene.h"
#include "StartScene.h"
#include "cocos2d.h"
#include<iostream>
#include"AudioManager.h"
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
        std::cerr << "PauseScene::createScene 锟斤拷锟斤拷: " << e.what() << std::endl;
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
        // 锟斤拷锟斤拷锟斤拷透锟斤拷锟斤拷锟斤拷 
        m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
        m_background->setPosition(origin);
        this->addChild(m_background, 0);
        

        // 锟斤拷锟斤拷锟斤拷锟斤拷 - 使锟斤拷TTF锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷
        auto titleLabel = Label::createWithTTF(u8"锟斤拷戏锟斤拷停", "fonts/forui3.ttf", 64);
        // 锟斤拷锟絋TF锟斤拷锟斤拷失锟杰ｏ拷使锟矫憋拷选锟斤拷锟斤拷
        if (!titleLabel || titleLabel->getContentSize().width <= 0) {
            titleLabel = Label::createWithSystemFont("GAME PAUSED", "", 64); // 英锟侥憋拷选
        }

        titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7f);
        titleLabel->setTextColor(Color4B::WHITE);
        titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);
        this->addChild(titleLabel, 1);

        std::string btnNormal = "images/ui/start&exit_btn_normal.png";
        std::string btnPressed = "images/ui/start&exit_btn_pressed.png";

        // 锟斤拷锟斤拷锟斤拷钮
        m_resumeButton = Button::create(btnNormal, btnPressed);
        if (m_resumeButton)
        {
            m_resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55f));
            m_resumeButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onResumeClicked, this));

            // 锟斤拷钮锟斤拷锟斤拷使锟斤拷TTF锟斤拷锟斤拷
            auto resumeLabel = Label::createWithTTF(u8"锟斤拷锟斤拷锟斤拷戏", "fonts/forui2.ttf", 32);
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

        // 锟斤拷锟铰匡拷始锟斤拷钮
        m_restartButton = Button::create(btnNormal, btnPressed);
        if (m_restartButton)
        {
            m_restartButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.45f));
            m_restartButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onRestartClicked, this));

            auto restartLabel = Label::createWithTTF(u8"锟斤拷锟铰匡拷始", "fonts/forui2.ttf", 32);
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

        // 锟剿筹拷锟斤拷钮
        m_exitButton = Button::create(btnNormal, btnPressed);
        if (m_exitButton)
        {
            m_exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
            m_exitButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onExitClicked, this));

            auto exitLabel = Label::createWithTTF(u8"锟斤拷锟截菜碉拷", "fonts/forui2.ttf", 32);
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
        // 锟饺恢革拷Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

        // 锟斤拷锟斤拷锟斤拷前锟斤拷锟斤拷
        director->popScene();

        log("Game resumed successfully");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onResumeClicked: " << e.what() << std::endl;
        // 锟斤拷锟斤拷锟街革拷锟斤拷强锟狡恢革拷Director
        Director::getInstance()->resume();
    }
}

void PauseScene::onRestartClicked(Ref* sender)
{
    log("Restart button clicked");

    try {
        AudioManager::getInstance()->playUISound("ui_button_click");
        // 锟斤拷要锟斤拷锟饺恢革拷Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

         //锟饺达拷一帧确锟斤拷状态锟街革拷
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 锟斤拷锟斤拷锟铰碉拷锟斤拷戏锟斤拷锟斤拷
            auto gameScene = MainGameScene::createScene();

            // 锟芥换锟斤拷锟斤拷
            director->replaceScene(TransitionFade::create(0.5f, gameScene));
            });

        log("Game restart initiated");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onRestartClicked: " << e.what() << std::endl;
        // 锟斤拷锟斤拷锟街革拷锟斤拷强锟狡恢革拷锟斤拷锟叫伙拷锟斤拷锟斤拷戏锟斤拷锟斤拷
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
        // 锟斤拷要锟斤拷锟饺恢革拷Director
        auto director = Director::getInstance();
        if (director->isPaused()) 
        {
            director->resume();
        }

        // 锟饺达拷一帧确锟斤拷状态锟街革拷
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // 锟斤拷锟斤拷锟斤拷始锟斤拷锟斤拷
            auto startScene = StartScene::createScene();

            // 锟芥换锟斤拷锟斤拷锟斤拷使锟矫癸拷锟斤拷效锟斤拷锟斤拷
            director->replaceScene(TransitionFade::create(0.5f, startScene));
            });

        log("Returning to main menu");
    }
    catch (const std::exception& e) {
        std::cerr << "Error in onExitClicked: " << e.what() << std::endl;
        // 锟斤拷锟斤拷锟街革拷锟斤拷强锟狡恢革拷锟斤拷锟叫伙拷锟斤拷锟斤拷始锟斤拷锟斤拷
        Director::getInstance()->resume();
        auto startScene = StartScene::createScene();
        Director::getInstance()->replaceScene(startScene);
    }
}

PauseScene::~PauseScene()
{
    // 锟斤拷锟斤拷锟斤拷锟教硷拷锟斤拷锟斤拷
    if (m_keyboardListener) 
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}

void PauseScene::initKeyboardListener()
{
    log("=== 锟斤拷始锟斤拷锟斤拷停锟斤拷锟斤拷锟斤拷锟教硷拷锟斤拷 ===");

    // 锟斤拷锟斤拷锟斤拷屑锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷瞥锟?
    if (m_keyboardListener) {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }

    // 锟斤拷锟斤拷锟铰的硷拷锟教硷拷锟斤拷锟斤拷
    m_keyboardListener = EventListenerKeyboard::create();

    // 锟斤拷锟矫帮拷锟斤拷锟斤拷锟铰回碉拷
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);

    // 锟斤拷锟接硷拷锟斤拷锟斤拷锟斤拷锟铰硷拷锟街凤拷锟斤拷
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);

    log("锟斤拷锟教硷拷锟斤拷锟斤拷锟斤拷注锟结，锟斤拷锟节匡拷锟皆帮拷ESC锟斤拷锟斤拷");
}

void PauseScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)//锟斤拷esc锟斤拷锟斤拷锟斤拷停
    {
        onResumeClicked(nullptr);  // 直锟接碉拷锟矫恢革拷锟斤拷锟斤拷
        event->stopPropagation();
        return;
    }
}

// 锟斤拷停锟斤拷锟斤拷锟斤拷示时
void PauseScene::onEnter() {
    Layer::onEnter();

    // 锟斤拷停锟斤拷前BGM锟斤拷锟斤拷锟斤拷锟斤拷停锟斤拷锟斤拷BGM
    LevelMusicManager::getInstance()->pauseCurrentBGM();
    AudioManager::getInstance()->playBGM("bgm_pause");
}

// 锟斤拷停锟斤拷锟斤拷锟剿筹拷时
void PauseScene::onExit() {
    // 止停锟斤拷停锟斤拷锟斤拷BGM锟斤拷锟街革拷锟斤拷戏BGM
    AudioManager::getInstance()->stopBGM();
    LevelMusicManager::getInstance()->resumeCurrentBGM();

    Layer::onExit();
}