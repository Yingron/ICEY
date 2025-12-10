// MainGameScene.cpp
// MainGameScene.cpp
#include"HudManager.h"
#include"HudLayer.h"
#include "MainGameScene.h"
#include "GameConfig.h"

USING_NS_CC;

// 添加缺失的 createScene() 函数
Scene* MainGameScene::createScene()
{
    // 创建普通场景，而不是物理场景
    auto scene = Scene::create();
    // 移除物理世界设置
    auto layer = MainGameScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化 HUD*********
void MainGameScene::initHud()
{
    log("=== initHUD ===");

    // 创建 HUD 层
    _hudLayer = HudLayer::create();
    if (_hudLayer) 
    {
        // 设置暂停按钮回调

        // 将 HUD 添加到场景中，确保在最上层显示
        this->addChild(_hudLayer, 100); // 使用较高的 z-order

        // 注册到HUD管理器（新增）
        HudManager::getInstance()->setHudLayer(_hudLayer);

        // 设置初始值
        _hudLayer->updateHealth(100.0f);  // 初始满血
        _hudLayer->updateSheld(10);       // 初始满护盾

        log("HUD Layer initialized successfully");
    }
    else 
    {
        log("ERROR: Failed to create HUD Layer!");
    }
}

// MainGameScene.cpp - 在 initPlayer 后添加调试信息
// MainGameScene.cpp - 修改 initPlayer 函数
void MainGameScene::initPlayer() {
    log("=== initPlayer ===");

    // 创建玩家
    _player = Player::create("icey-run-to-right-1.png");
    if (_player) {
        // 设置玩家初始位置为 (400, 200)
        _player->setPosition(Vec2(400, 200));

        // 调整玩家缩放
        _player->setScale(2.0f);

        this->addChild(_player, 1);

        // 调试：检查玩家精灵帧
        if (_player->getSpriteFrame()) {
            log("Player sprite frame loaded successfully");
        }
        else {
            log("WARNING: Player sprite frame is null!");
        }

        log("Player created at position: 400, 200");
    }
    else {
        log("ERROR: Failed to create player!");
    }
}

// 添加缺失的 initInput() 函数
void MainGameScene::initInput() {
    log("=== initInput ===");

    // 创建键盘监听器
    _keyboardListener = EventListenerKeyboard::create();

    // 按键按下事件
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressed, this);

    // 按键释放事件
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(MainGameScene::onKeyReleased, this);

    // 添加监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    log("Input initialized");
}

// 添加缺失的 update() 函数
void MainGameScene::update(float delta) {
    if (_player) {
        _player->update(delta);

        // 调试：显示玩家位置
        auto pos = _player->getPosition();
        auto state = _player->getCurrentState();
        log("Player Pos: (%.1f, %.1f), State: %d", pos.x, pos.y, (int)state);
    }
}

// 添加缺失的 onKeyPressed() 函数
void MainGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyPressed(keyCode);
    }
    //测试HudManager
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_1:
            HudManager::updateHealth(30.0f);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_2:
            HudManager::updateHealth(90.0f);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_3:
            HudManager::updateSheld(3);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_4:
            static bool hudVisible = true;
            hudVisible = !hudVisible;
            HudManager::showHud(hudVisible);
            break;
    }
}

// 添加缺失的 onKeyReleased() 函数
void MainGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyReleased(keyCode);
    }
}

bool MainGameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 获取窗口尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    log("=== MainGameScene init ===");
    log("Visible size: %f x %f", visibleSize.width, visibleSize.height);
    log("Origin: %f, %f", origin.x, origin.y);

    // 添加调试信息显示
    auto debugLabel = Label::createWithSystemFont(
        StringUtils::format("Screen: %.0fx%.0f", visibleSize.width, visibleSize.height),
        "Arial", 18
    );
    debugLabel->setPosition(Vec2(visibleSize.width - 100, visibleSize.height - 20));
    debugLabel->setColor(Color3B::YELLOW);
    this->addChild(debugLabel, 10);

    // 初始化背景
    initBackground();

    // 初始化玩家
    initPlayer();

    // 初始化输入
    initInput();

    //添加Hud界面
    initHud();

    // 调度更新
    this->scheduleUpdate();

    return true;
}
void MainGameScene::initBackground() {
    log("=== initBackground ===");

    auto fileUtils = FileUtils::getInstance();
    std::string foundPath = "";

    // 尝试不同路径
    std::vector<std::string> possiblePaths = {
        "background1.jpg",
        "images/environment/background/background1.jpg",
        "images/background1.jpg",
        "environment/background/background1.jpg"
    };

    for (const auto& path : possiblePaths) {
        if (fileUtils->isFileExist(path)) {
            foundPath = path;
            log("Found background at: %s", path.c_str());
            break;
        }
    }

    if (foundPath.empty()) {
        log("ERROR: Could not find background1.jpg!");

        // 创建一个纯色背景
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto layerColor = LayerColor::create(Color4B(50, 50, 100, 255), visibleSize.width, visibleSize.height);
        layerColor->setPosition(0, 0);
        this->addChild(layerColor, 0);

        // 将 layerColor 赋值给 _background
        _background = layerColor;  // 现在类型匹配了

        return;
    }

    // 创建背景精灵
    auto backgroundSprite = Sprite::create(foundPath);

    if (backgroundSprite) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        backgroundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

        // 根据窗口尺寸调整背景大小
        auto bgSize = backgroundSprite->getContentSize();
        log("Background original size: %f x %f", bgSize.width, bgSize.height);
        log("Window size: %f x %f", visibleSize.width, visibleSize.height);

        // 计算缩放比例，保持宽高比
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;

        // 使用较大的缩放比例填充整个窗口
        float scale = std::max(scaleX, scaleY);
        backgroundSprite->setScale(scale);

        log("Background scale: %f", scale);

        this->addChild(backgroundSprite, 0);
        _background = backgroundSprite;  // 将精灵赋值给 _background
        log("Background loaded successfully");
    }
    else {
        log("ERROR: Failed to create background sprite!");

        // 创建一个纯色背景作为备用
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto layerColor = LayerColor::create(Color4B(100, 100, 200, 255), visibleSize.width, visibleSize.height);
        layerColor->setPosition(0, 0);
        this->addChild(layerColor, 0);
        _background = layerColor;  // 将 layerColor 赋值给 _background
    }
}

