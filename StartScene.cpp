//测试版本标注掉了一些未完成需要引用的文件
#include"StartScene.h"
#include <stdexcept> 
#include"MainGameScene.h"
#include<iostream>
//#include"ResourceManager.h"
//#include"GameConfig.h"
//#include"GameSceneManager.h"
USING_NS_CC;
using namespace cocos2d::ui;


Scene* StartScene::createScene()//创建初始场景StartScene
{
	try
	{
		// 捕获场景创建过程中的异常
		auto scene = StartScene::create();
		if (!scene)
		{
			//主动抛出场景创建失败的异常
			throw std::runtime_error("StartScene 创建失败：返回空指针");
		}
		return scene;
	}
	catch (const std::exception& e)
	{
		//捕获标准异常并输出，避免程序崩溃
		std::cerr << "StartScene::createScene 异常：" << e.what() << std::endl;
		// +++ 新增：兜底返回空场景，防止程序崩溃
		return Scene::create();
	}
	catch (...)
	{
		// +++ 新增：捕获未知异常，覆盖所有异常类型
		std::cerr << "StartScene::createScene 未知异常" << std::endl;
		return Scene::create();
	}
}

bool StartScene::init()
{
	try 
	{
		if (!Scene::init())
		{
			//初始化父类失败时主动抛异常
			throw std::runtime_error("Scene::init() 初始化失败");
		}

		createUI();
		//loadSceneResources();
		return true;
	}
	catch (const std::exception& e) 
	{
		//捕获初始化阶段的标准异常
		std::cerr << "StartScene::init 异常：" << e.what() << std::endl;
		return false; 
	}
	catch (...) 
	{
		//捕获初始化阶段的未知异常
		std::cerr << "StartScene::init 未知异常" << std::endl;
		return false;
	}
}

void StartScene::createUI()
{
	try
	{
		//获取屏幕尺寸
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//创建背景
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
			throw std::runtime_error("背景图加载失败：" + bgPath);
		}


		//创建标题
		m_titleLabel = Label::createWithTTF(u8"艾希","fonts/forui.ttf", 188);
		if (!m_titleLabel)
		{
			throw std::runtime_error("标题Label创建失败");
		}
		m_titleLabel->setPosition(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height * 0.8f);
		m_titleLabel->setTextColor(Color4B(176, 224, 230, 255));  // 蓝紫色
		m_titleLabel->enableShadow(Color4B::WHITE, Size(2, -2), 2);  // 白色阴影
		m_titleLabel->enableOutline(Color4B(75, 0, 130, 255), 3);    // 靛蓝色描边
		m_titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
		m_titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
		this->addChild(m_titleLabel, 1);  // 添加到场景


		// 创建副标题
		m_subTitleLabel = Label::createWithTTF(u8"ICEY", "fonts/forui2.ttf", 48);
		if (!m_subTitleLabel)
		{
			// 如果副标题字体不存在，使用系统字体
			m_subTitleLabel = Label::createWithSystemFont(u8"ICEY", "Arial", 48);
			if (!m_subTitleLabel)
			{
				CCLOG("副标题创建失败");
			}
		}

		if (m_subTitleLabel)
		{
			// 设置副标题位置（在主标题下方）
			m_subTitleLabel->setPosition(origin.x + visibleSize.width / 2,
				m_titleLabel->getPositionY() - 120);  // 在主标题下方120像素

			// 设置副标题颜色（比主标题浅一些的紫色）
			m_subTitleLabel->setTextColor(Color4B(186, 85, 211, 255));  // 中兰花紫
			m_subTitleLabel->enableShadow(Color4B::WHITE, Size(1, -1), 1);  // 白色阴影
			m_subTitleLabel->enableGlow(Color4B(221, 160, 221, 128));       // 浅紫色发光
			m_subTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
			m_subTitleLabel->setVerticalAlignment(TextVAlignment::CENTER);

			this->addChild(m_subTitleLabel, 1);

			// 副标题添加淡入动画
			m_subTitleLabel->setOpacity(0);
			auto fadeIn = FadeIn::create(1.5f);
			auto delay = DelayTime::create(0.5f);
			m_subTitleLabel->runAction(Sequence::create(delay, fadeIn, nullptr));

			// 添加呼吸动画
			auto scaleUp = ScaleTo::create(1.2f, 1.05f);
			auto scaleDown = ScaleTo::create(1.2f, 0.95f);
			auto breathSequence = Sequence::create(scaleUp, scaleDown, nullptr);
			auto breathRepeat = RepeatForever::create(breathSequence);
			m_subTitleLabel->runAction(breathRepeat);
		}

		//开始按钮

		// 按钮2种状态的图片路径（正常/按下）
		std::string startNormal = "images/ui/start&exit_btn_normal.png";
		std::string startPressed = "images/ui/start&exit_btn_pressed.png";



		// 创建按钮（传入三种状态的完整路径）
		m_startButton = Button::create(
			/*GameConfig::TEXTURE_PATH +*/ startNormal,
			/*GameConfig::TEXTURE_PATH +*/ startPressed
		);

		if (m_startButton)  // 按钮创建成功
		{
			// 按钮位置：屏幕中间偏下（40%高度处，水平居中）
			m_startButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height * 0.5f));
			// 绑定点击事件（点击后调用 onStartClicked 函数）
			m_startButton->addClickEventListener(CC_CALLBACK_1(StartScene::onStartClicked, this));

			// 给按钮添加文字标签
			auto startLabel = Label::createWithTTF(u8"开始游戏", "fonts/forui2.ttf", 32);
			if (!startLabel)
			{
				throw std::runtime_error("开始按钮Label创建失败");
			}
			startLabel->setTextColor(Color4B::WHITE);  // 文字白色
			m_startButton->setTitleLabel(startLabel);  // 设置为按钮标题
			this->addChild(m_startButton, 1);  // 添加到场景,与标题同层
		}
		else
		{
			throw std::runtime_error("开始按钮创建失败：" + startNormal + "/" + startPressed);
		}
		//退出按钮

		// 按钮2种状态的图片路径（正常/按下）
		std::string exitNormal = "images/ui/start&exit_btn_normal.png";
		std::string exitPressed = "images/ui/start&exit_btn_pressed.png";



		// 创建按钮（传入2种状态的完整路径）
		m_exitButton = Button::create(
			/*GameConfig::TEXTURE_PATH +*/ exitNormal,
			/*GameConfig::TEXTURE_PATH +*/ exitPressed
		);

		if (m_exitButton)  // 按钮创建成功
		{
			// 按钮位置：屏幕中间偏下（40%高度处，水平居中）
			m_exitButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height * 0.3f));
			// 绑定点击事件（点击后调用 onExitClicked 函数）
			m_exitButton->addClickEventListener(CC_CALLBACK_1(StartScene::onExitClicked, this));

			// 给按钮添加文字标签
			auto exitLabel = Label::createWithTTF(u8"退出游戏", "fonts/forui2.ttf", 32);
			if (!exitLabel)
			{
				throw std::runtime_error("退出按钮Label创建失败");
			}
			exitLabel->setTextColor(Color4B::WHITE);  // 文字白色
			m_exitButton->setTitleLabel(exitLabel);  // 设置为按钮标题
			this->addChild(m_exitButton, 1);  // 添加到场景,与标题同层
		}
		else
		{
			throw std::runtime_error("退出按钮创建失败：" + exitNormal + "/" + exitPressed);
		}
	}
	catch (const std::exception& e) 
	{
		std::cerr << "StartScene::createUI 异常：" << e.what() << std::endl;
		throw;
	}
	catch (...) 
	{
		std::cerr << "StartScene::createUI 未知异常" << std::endl;
		throw;
	}
}

void StartScene::onStartClicked(Ref* sender)
{
	try 
	{
		// 通过场景管理器切换到游戏主场景
		auto gameScene = MainGameScene::createScene();
		//校验游戏场景创建结果
		if (!gameScene)
		{
			throw std::runtime_error("MainGameScene 创建失败：返回空指针");
		}
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
	}
	catch (const std::exception& e) 
	{
		//捕获场景切换异常并输出
		std::cerr << "StartScene::onStartClicked 异常：" << e.what() << std::endl;
		//异常兜底，显示错误提示
		auto alert = Label::createWithSystemFont("Failed to start game!", "", 48);
		alert->setTextColor(Color4B::RED);
		alert->setPosition(Director::getInstance()->getVisibleSize() / 2);
		this->addChild(alert, 10);
	}
	catch (...) 
	{
		//捕获未知异常
		std::cerr << "StartScene::onStartClicked 未知异常" << std::endl;
	}
}

void StartScene::onExitClicked(Ref* sender)
{
	try 
	{
		Director::getInstance()->end();
		//Windows平台兜底退出逻辑
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		exit(0);
#endif
	}
	catch (const std::exception& e) 
	{
		std::cerr << "StartScene::onExitClicked 异常：" << e.what() << std::endl;
		//强制退出兜底
		exit(1);
	}
	catch (...) 
	{
		// 捕获未知异常
		std::cerr << "StartScene::onExitClicked 未知异常" << std::endl;
		exit(1);
	}
}



/*
void StartScene::loadSceneResources()
{
	// 预加载背景图、按钮图片等资源
	ResourceManager::getInstance()->preloadTextures({
		"start_bg.png",
		"ui/start&exit_btn_normal.png",
		"ui/start&exit_btn_pressed.png",
		});

	// 预加载音效
	//SoundManager::getInstance()->preloadSounds();
}

*/

