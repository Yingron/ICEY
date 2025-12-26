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
        std::cerr << "PauseScene::createScene ����: " << e.what() << std::endl;
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
        // ������͸������ 
        m_background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
        m_background->setPosition(origin);
        this->addChild(m_background, 0);
        

        // �������� - ʹ��TTF������ʾ����
        auto titleLabel = Label::createWithTTF(u8"��Ϸ��ͣ", "fonts/forui3.ttf", 64);
        // ���TTF����ʧ�ܣ�ʹ�ñ�ѡ����
        if (!titleLabel || titleLabel->getContentSize().width <= 0) {
            titleLabel = Label::createWithSystemFont("GAME PAUSED", "", 64); // Ӣ�ı�ѡ
        }

        titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7f);
        titleLabel->setTextColor(Color4B::WHITE);
        titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);
        this->addChild(titleLabel, 1);

        std::string btnNormal = "images/ui/start&exit_btn_normal.png";
        std::string btnPressed = "images/ui/start&exit_btn_pressed.png";

        // ������ť
        m_resumeButton = Button::create(btnNormal, btnPressed);
        if (m_resumeButton)
        {
            m_resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.55f));
            m_resumeButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onResumeClicked, this));

            // ��ť����ʹ��TTF����
            auto resumeLabel = Label::createWithTTF(u8"������Ϸ", "fonts/forui2.ttf", 32);
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

        // ���¿�ʼ��ť
        m_restartButton = Button::create(btnNormal, btnPressed);
        if (m_restartButton)
        {
            m_restartButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.45f));
            m_restartButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onRestartClicked, this));

            auto restartLabel = Label::createWithTTF(u8"���¿�ʼ", "fonts/forui2.ttf", 32);
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

        // �˳���ť
        m_exitButton = Button::create(btnNormal, btnPressed);
        if (m_exitButton)
        {
            m_exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
            m_exitButton->addClickEventListener(CC_CALLBACK_1(PauseScene::onExitClicked, this));

            auto exitLabel = Label::createWithTTF(u8"���ز˵�", "fonts/forui2.ttf", 32);
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
        // �Ȼָ�Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

        // ������ǰ����
        director->popScene();

        log("Game resumed successfully");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onResumeClicked: " << e.what() << std::endl;
        // �����ָ���ǿ�ƻָ�Director
        Director::getInstance()->resume();
    }
}

void PauseScene::onRestartClicked(Ref* sender)
{
    log("Restart button clicked");

    try {
        AudioManager::getInstance()->playUISound("ui_button_click");
        // ��Ҫ���Ȼָ�Director
        auto director = Director::getInstance();
        if (director->isPaused())
        {
            director->resume();
        }

         //�ȴ�һ֡ȷ��״̬�ָ�
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // �����µ���Ϸ����
            auto gameScene = MainGameScene::createScene();

            // �滻����
            director->replaceScene(TransitionFade::create(0.5f, gameScene));
            });

        log("Game restart initiated");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error in onRestartClicked: " << e.what() << std::endl;
        // �����ָ���ǿ�ƻָ����л�����Ϸ����
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
        // ��Ҫ���Ȼָ�Director
        auto director = Director::getInstance();
        if (director->isPaused()) 
        {
            director->resume();
        }

        // �ȴ�һ֡ȷ��״̬�ָ�
        director->getScheduler()->performFunctionInCocosThread([director]() {
            // ������ʼ����
            auto startScene = StartScene::createScene();

            // �滻������ʹ�ù���Ч����
            director->replaceScene(TransitionFade::create(0.5f, startScene));
            });

        log("Returning to main menu");
    }
    catch (const std::exception& e) {
        std::cerr << "Error in onExitClicked: " << e.what() << std::endl;
        // �����ָ���ǿ�ƻָ����л�����ʼ����
        Director::getInstance()->resume();
        auto startScene = StartScene::createScene();
        Director::getInstance()->replaceScene(startScene);
    }
}

PauseScene::~PauseScene()
{
    // �������̼�����
    if (m_keyboardListener) 
    {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }
}

void PauseScene::initKeyboardListener()
{
    log("=== ��ʼ����ͣ�������̼��� ===");

    // ������м����������Ƴ�
    if (m_keyboardListener) {
        _eventDispatcher->removeEventListener(m_keyboardListener);
        m_keyboardListener = nullptr;
    }

    // �����µļ��̼�����
    m_keyboardListener = EventListenerKeyboard::create();

    // ���ð������»ص�
    m_keyboardListener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);

    // ���Ӽ��������¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);

    log("���̼�������ע�ᣬ���ڿ��԰�ESC����");
}

void PauseScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)//��esc������ͣ
    {
        onResumeClicked(nullptr);  // ֱ�ӵ��ûָ�����
        event->stopPropagation();
        return;
    }
}

// ��ͣ������ʾʱ
void PauseScene::onEnter() {
    Layer::onEnter();

    // ��ͣ��ǰBGM��������ͣ����BGM
    LevelMusicManager::getInstance()->pauseCurrentBGM();
    AudioManager::getInstance()->playBGM("bgm_pause");
}

// ��ͣ�����˳�ʱ
void PauseScene::onExit() {
    // ֹͣ��ͣ����BGM���ָ���ϷBGM
    AudioManager::getInstance()->stopBGM();
    LevelMusicManager::getInstance()->resumeCurrentBGM();

    Layer::onExit();
}