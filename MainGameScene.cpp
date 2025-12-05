// MainGameScene.cpp
// MainGameScene.cpp
#include "MainGameScene.h"
#include "GameConfig.h"

USING_NS_CC;

// 添加缺失的 createScene() 函数
Scene* MainGameScene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -GameConfig::GRAVITY));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = MainGameScene::create();
    scene->addChild(layer);

    return scene;
}

// MainGameScene.cpp - 确保地面位置正确
void MainGameScene::initPlayer() {
    log("=== initPlayer ===");

    // 创建玩家
    _player = Player::create("icey-run-to-right-1.png");
    if (_player) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        log("Window size for player placement: %f x %f", visibleSize.width, visibleSize.height);

        // 设置玩家位置（场景中央，靠近地面）
        float groundHeight = 50.0f;
        float playerY = groundHeight + _player->getContentSize().height / 2 * 2.0f; // 考虑缩放

        _player->setPosition(Vec2(visibleSize.width / 2, playerY));

        // 调整玩家缩放
        _player->setScale(2.0f);

        // 设置玩家物理属性 - 简化版本，避免重力问题
        auto physicsBody = PhysicsBody::createBox(Size(_player->getContentSize().width * 0.8,
            _player->getContentSize().height * 0.8));
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false);  // 暂时禁用重力
        physicsBody->setRotationEnable(false);
        physicsBody->setCategoryBitmask(0x01);
        physicsBody->setContactTestBitmask(0x02);
        physicsBody->setCollisionBitmask(0x02);
        _player->setPhysicsBody(physicsBody);

        this->addChild(_player, 1);

        // 添加地面 - 确保位置正确
        auto ground = Sprite::create();
        ground->setTextureRect(Rect(0, 0, visibleSize.width, groundHeight));
        ground->setColor(Color3B::GREEN);
        ground->setPosition(Vec2(visibleSize.width / 2, groundHeight / 2));

        auto groundBody = PhysicsBody::createBox(ground->getContentSize());
        groundBody->setDynamic(false);
        groundBody->setCategoryBitmask(0x02);
        groundBody->setCollisionBitmask(0x01);
        ground->setPhysicsBody(groundBody);

        this->addChild(ground, 0);

        log("Player created at position: %f, %f", visibleSize.width / 2, playerY);
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
    }
}

// 添加缺失的 onKeyPressed() 函数
void MainGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyPressed(keyCode);
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

