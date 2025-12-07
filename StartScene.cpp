//测试版本标注掉了一些未完成需要引用的文件
#include"StartScene.h"
//#include"ResourceManager.h"
//#include"GameConfig.h"
//#include"GameSceneManager.h"
USING_NS_CC;
using namespace cocos2d::ui;

Scene* StartScene::createScene()//创建初始场景StartScene
{
	return StartScene::create();
}

bool StartScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	createUI();
	//loadSceneResources();
	return true;
}

void StartScene::createUI()
{
	//获取屏幕尺寸
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//创建背景
	std::string bgPath = /*GameConfig::TEXTURE_PATH + */"start_bg.png";
	auto background = Sprite::create(bgPath);
	if(background)
	{
		background->setPosition(origin + visibleSize / 2);
		background->setScale(
			visibleSize.width / background->getContentSize().width,
			visibleSize.height / background->getContentSize().height
		);
		this->addChild(background, 0);
	}
	

	//创建标题，使用系统自带的字体
	m_titleLabel = Label::createWithSystemFont("ICEY", "", 128);

	m_titleLabel->setPosition(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height * 0.7f);
	m_titleLabel->setTextColor(Color4B::WHITE);  // 文字颜色白色
	m_titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);  // 添加黑色阴影
	this->addChild(m_titleLabel, 1);  // 添加到场景

	//开始按钮

	// 按钮2种状态的图片路径（正常/按下）
	std::string startNormal = "ui/start&exit_btn_normal.png";
	std::string startPressed = "ui/start&exit_btn_pressed.png";



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
		auto startLabel = Label::createWithSystemFont("Start", "", 32);
		startLabel->setTextColor(Color4B::WHITE);  // 文字白色
		m_startButton->setTitleLabel(startLabel);  // 设置为按钮标题
		this->addChild(m_startButton, 1);  // 添加到场景,与标题同层
	}

	//退出按钮

	// 按钮2种状态的图片路径（正常/按下）
	std::string exitNormal = "ui/start&exit_btn_normal.png";
	std::string exitPressed = "ui/start&exit_btn_pressed.png";



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
		auto exitLabel = Label::createWithSystemFont("Exit", "", 32);
		exitLabel->setTextColor(Color4B::WHITE);  // 文字白色
		m_exitButton->setTitleLabel(exitLabel);  // 设置为按钮标题
		this->addChild(m_exitButton, 1);  // 添加到场景,与标题同层
	}
}

void StartScene::onStartClicked(Ref* sender)
{
	// 通过场景管理器切换到游戏主场景
	//GameSceneManager::createGameScene();
}

void StartScene::onExitClicked(Ref* sender)
{
	// 调用 Cocos2d-x 导演结束游戏
	Director::getInstance()->end();
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

