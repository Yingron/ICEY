//Test version: commented out some incomplete referenced files
#include"StartScene.h"
#include <stdexcept> 
#include"MainGameScene.h"
#include<iostream>
#include"AudioManager.h"
#include "LevelMusicManager.h"
//#include"ResourceManager.h"
//#include"GameConfig.h"
//#include"GameSceneManager.h"
USING_NS_CC;
using namespace cocos2d::ui;


Scene* StartScene::createScene()//Create initial scene StartScene
{
	try
	{
		// Catch exceptions during scene creation
		auto scene = StartScene::create();
		if (!scene)
		{
			//Actively throw exception if scene creation fails
			throw std::runtime_error("StartScene creation failed: returned null pointer");
		}
		return scene;
	}
	catch (const std::exception& e)
	{
		//Catch standard exceptions and output to avoid program crash
		std::cerr << "StartScene::createScene exception: " << e.what() << std::endl;
		// +++ Added: fallback to return empty scene to prevent program crash
		return Scene::create();
	}
	catch (...)
	{
		// +++ Added: catch unknown exceptions, cover all exception types
		std::cerr << "StartScene::createScene unknown exception" << std::endl;
		return Scene::create();
	}
}

bool StartScene::init()
{
	
	// Play title music
	AudioManager::getInstance()->playBGM("bgm_title");
	try 
	{
		if (!Scene::init())
		{
			//Actively throw exception if parent class initialization fails
			throw std::runtime_error("Scene::init() initialization failed");
		}

		createUI();
		//loadSceneResources();
		return true;
	}
	catch (const std::exception& e) 
	{
		//Catch standard exceptions during initialization phase
		std::cerr << "StartScene::init exception: " << e.what() << std::endl;
		return false; 
	}
	catch (...) 
	{
		//Catch unknown exceptions during initialization phase
		std::cerr << "StartScene::init unknown exception" << std::endl;
		return false;
	}
}

void StartScene::createUI()
{
	try
	{
		//Get screen size
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//Create background
		std::string bgPath = /*GameConfig::TEXTURE_PATH + */"images/ui/start_bg.png";
		auto background = Sprite::create(bgPath);
		if (background)
		{
			background->setPosition(origin + visibleSize / 2);
			background->setScale(
				visibleSize.width / background->getContentSize().width,
				visibleSize.height / background->getContentSize().height
			);
			this->addChild(background, 0);
		}
		else
		{
			throw std::runtime_error("Background image loading failed: " + bgPath);
		}


		//Create title
		m_titleLabel = Label::createWithTTF(u8"鑹惧笇","fonts/forui.ttf", 188);
		if (!m_titleLabel)
		{
			throw std::runtime_error("Title Label creation failed");
		}
		m_titleLabel->setPosition(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height * 0.8f);
		m_titleLabel->setTextColor(Color4B(176, 224, 230, 255));  // Azure
		m_titleLabel->enableShadow(Color4B::WHITE, Size(2, -2), 2);  // White shadow
		m_titleLabel->enableOutline(Color4B(75, 0, 130, 255), 3);    // Indigo outline
		m_titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
		m_titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
		this->addChild(m_titleLabel, 1);  // Add to scene


		// Create subtitle
		m_subTitleLabel = Label::createWithTTF(u8"ICEY", "fonts/forui2.ttf", 48);
		if (!m_subTitleLabel)
		{
			// If subtitle font doesn't exist, use system font
			m_subTitleLabel = Label::createWithSystemFont(u8"ICEY", "Arial", 48);
			if (!m_subTitleLabel)
			{
				CCLOG("Subtitle creation failed");
			}
		}

		if (m_subTitleLabel)
		{
			// Set subtitle position (below main title)
			m_subTitleLabel->setPosition(origin.x + visibleSize.width / 2,
				m_titleLabel->getPositionY() - 120);  // 120 pixels below main title

			// Set subtitle color (lighter purple than main title)
			m_subTitleLabel->setTextColor(Color4B(186, 85, 211, 255));  // Medium Orchid
			m_subTitleLabel->enableShadow(Color4B::WHITE, Size(1, -1), 1);  // White shadow
			m_subTitleLabel->enableGlow(Color4B(221, 160, 221, 128));       // Light purple glow
			m_subTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
			m_subTitleLabel->setVerticalAlignment(TextVAlignment::CENTER);

			this->addChild(m_subTitleLabel, 1);

			// Add fade-in animation to subtitle
			m_subTitleLabel->setOpacity(0);
			auto fadeIn = FadeIn::create(1.5f);
			auto delay = DelayTime::create(0.5f);
			m_subTitleLabel->runAction(Sequence::create(delay, fadeIn, nullptr));

			// Add breathing animation
			auto scaleUp = ScaleTo::create(1.2f, 1.05f);
			auto scaleDown = ScaleTo::create(1.2f, 0.95f);
			auto breathSequence = Sequence::create(scaleUp, scaleDown, nullptr);
			auto breathRepeat = RepeatForever::create(breathSequence);
			m_subTitleLabel->runAction(breathRepeat);
		}

		// Start button

		// Button two state image paths (normal/pressed)
		std::string startNormal = "images/ui/start&exit_btn_normal.png";
		std::string startPressed = "images/ui/start&exit_btn_pressed.png";



		// Create button (pass full paths for three states)
		m_startButton = Button::create(
			/*GameConfig::TEXTURE_PATH +*/ startNormal,
			/*GameConfig::TEXTURE_PATH +*/ startPressed
		);

		if (m_startButton)  // 鎸夐挳鍒涘缓鎴愬姛
		{
			// Button position: middle bottom of screen (40% height, horizontally centered)
			m_startButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height * 0.5f));
			// Bind click event (call onStartClicked function when clicked)
			m_startButton->addClickEventListener(CC_CALLBACK_1(StartScene::onStartClicked, this));

			// Add text label to button
			auto startLabel = Label::createWithTTF(u8"Start Game", "fonts/forui2.ttf", 32);
			if (!startLabel)
			{
				throw std::runtime_error("Start button label creation failed");
			}
			startLabel->setTextColor(Color4B::WHITE);  // 鏂囧瓧鐧借壊
			m_startButton->setTitleLabel(startLabel);  // 璁剧疆涓烘寜閽爣棰?
			this->addChild(m_startButton, 1);  // 娣诲姞鍒板満鏅?涓庢爣棰樺悓灞?
		}
		else
		{
			throw std::runtime_error("寮€濮嬫寜閽垱寤哄け璐ワ細" + startNormal + "/" + startPressed);
		}
		// Exit button

		// 鎸夐挳2绉嶇姸鎬佺殑鍥剧墖璺緞锛堟甯?鎸変笅锛?
		std::string exitNormal = "images/ui/start&exit_btn_normal.png";
		std::string exitPressed = "images/ui/start&exit_btn_pressed.png";



		// Create button (pass full paths for two states)
		m_exitButton = Button::create(
			/*GameConfig::TEXTURE_PATH +*/ exitNormal,
			/*GameConfig::TEXTURE_PATH +*/ exitPressed
		);

		if (m_exitButton)  // 鎸夐挳鍒涘缓鎴愬姛
		{
			// 鎸夐挳浣嶇疆锛氬睆骞曚腑闂村亸涓嬶紙40%楂樺害澶勶紝姘村钩灞呬腑锛?
			m_exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height * 0.3f));
			// Bind click event (call onExitClicked function when clicked)
			m_exitButton->addClickEventListener(CC_CALLBACK_1(StartScene::onExitClicked, this));

			// 缁欐寜閽坊鍔犳枃瀛楁爣绛?
			auto exitLabel = Label::createWithTTF(u8"Exit Game", "fonts/forui2.ttf", 32);
			if (!exitLabel)
			{
				throw std::runtime_error("Exit button label creation failed");
			}
			exitLabel->setTextColor(Color4B::WHITE);  // 鏂囧瓧鐧借壊
			m_exitButton->setTitleLabel(exitLabel);  // 璁剧疆涓烘寜閽爣棰?
			this->addChild(m_exitButton, 1);  // 娣诲姞鍒板満鏅?涓庢爣棰樺悓灞?
		}
		else
		{
			throw std::runtime_error("Exit button creation failed: " + exitNormal + "/" + exitPressed);
		}


		// Play start scene background music
		LevelMusicManager::getInstance()->playBGMForLevel(LevelManager::LevelState::START_SCENE);

	}
	catch (const std::exception& e) 
	{
		std::cerr << "StartScene::createUI exception: " << e.what() << std::endl;
		throw;
	}
	catch (...) 
	{
		std::cerr << "StartScene::createUI unknown exception" << std::endl;
		throw;
	}
}

void StartScene::onStartClicked(Ref* sender)
{
	try 
	{
		AudioManager::getInstance()->playUISound("ui_button_click");
		// Switch to game main scene through scene manager
		auto gameScene = MainGameScene::createScene();
		// Verify game scene creation result
		if (!gameScene)
		{
			throw std::runtime_error("MainGameScene creation failed: returned null pointer");
		}
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
	}
	catch (const std::exception& e) 
	{
		//鎹曡幏鍦烘櫙鍒囨崲寮傚父骞惰緭鍑?
		std::cerr << "StartScene::onStartClicked exception: " << e.what() << std::endl;
		// Exception fallback, show error message
		auto alert = Label::createWithSystemFont("Failed to start game!", "", 48);
		alert->setTextColor(Color4B::RED);
		alert->setPosition(Director::getInstance()->getVisibleSize() / 2);
		this->addChild(alert, 10);
	}
	catch (...) 
	{
		//鎹曡幏鏈煡寮傚父
		std::cerr << "StartScene::onStartClicked unknown exception" << std::endl;
	}
}

void StartScene::onExitClicked(Ref* sender)
{
	try 
	{
		AudioManager::getInstance()->playUISound("ui_button_click");
		Director::getInstance()->end();
		// Windows platform fallback exit logic
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		exit(0);
#endif
	}
	catch (const std::exception& e) 
	{
		std::cerr << "StartScene::onExitClicked exception: " << e.what() << std::endl;
		// Force exit fallback
		exit(1);
	}
	catch (...) 
	{
		// Catch unknown exception
		std::cerr << "StartScene::onExitClicked unknown exception" << std::endl;
		exit(1);
	}
}


