#include "MainGameScene.h"
#include "GameConfig.h"
#include "Level1SceneBackground.h"  // 确保包含正确的头文件
#include "LevelManager.h"

USING_NS_CC;

Scene* MainGameScene::createScene() {
    auto scene = Scene::create();
    auto layer = MainGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainGameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    log("=== MainGameScene init ===");
    log("Visible size: %f x %f", visibleSize.width, visibleSize.height);

    // 保存屏幕宽度
    _screenWidth = visibleSize.width;

    // 初始化关卡管理器
    _levelManager = LevelManager::getInstance();

    // 初始化摄像机
    initCamera();

    // 初始化背景
    initBackground();

    // 初始化玩家
    initPlayer();

    // 初始化输入
    initInput();

    // 初始化调试UI
    initDebugUI();

    // 初始化关卡切换状态
    _isTransitioning = false;
    _transitionTimer = 0.0f;

    // 调度更新
    this->scheduleUpdate();

    return true;
}

void MainGameScene::initCamera() {
    _cameraOffsetX = 0.0f;
    _worldWidth = 0.0f;
    _minCameraX = 0.0f;
    _maxCameraX = 0.0f;

    log("Camera initialized with screen width: %.0f", _screenWidth);
}

void MainGameScene::initBackground() {
    log("=== initBackground ===");

    // 获取屏幕尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _screenWidth = visibleSize.width;
    float screenHeight = visibleSize.height;

    log("Screen width: %.0f, height: %.0f", _screenWidth, screenHeight);

    // 创建当前关卡的背景
    _currentBackground = SceneBackground::create();
    if (_currentBackground) {
        this->addChild(_currentBackground, -10);

        // 获取世界宽度
        _worldWidth = _currentBackground->getWorldWidth();

        log("Background created successfully");
        log("World width: %.0f, Screen width: %.0f", _worldWidth, _screenWidth);

        // 初始摄像机偏移设为0
        _cameraOffsetX = 0.0f;

        // 关键修复：立即更新背景位置
        _currentBackground->updateWithCameraOffset(_cameraOffsetX);

        // 检查背景是否完全覆盖屏幕
        if (_worldWidth < _screenWidth) {
            log("WARNING: Background width (%.0f) < screen width (%.0f)", _worldWidth, _screenWidth);

            // 强制背景覆盖整个屏幕
            _currentBackground->setScaleX(_screenWidth / _worldWidth);
            _worldWidth = _screenWidth;
            log("Forced background to cover screen by scaling");
        }

    }
    else {
        log("ERROR: Failed to create background");

        // 创建紧急后备背景
        auto emergencyBackground = Sprite::create();
        emergencyBackground->setTextureRect(Rect(0, 0, _screenWidth, visibleSize.height));
        emergencyBackground->setColor(Color3B(30, 30, 60)); // 深蓝色背景
        emergencyBackground->setPosition(Vec2(_screenWidth * 0.5f, visibleSize.height * 0.5f));
        this->addChild(emergencyBackground, -10);

        _worldWidth = _screenWidth;
        log("Created emergency background");
    }
}

void MainGameScene::initPlayer() {
    log("=== initPlayer ===");

    // 创建玩家
    _player = Player::create("icey-idle-1.png");
    if (_player) {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 设置玩家初始世界位置
        float initialWorldX = _screenWidth * 0.1f; // 从屏幕左侧10%位置开始
        float initialWorldY = 0.0f+100.0;               // 初始在地面

        _player->setWorldPositionX(initialWorldX);
        _player->setWorldPositionY(initialWorldY);

        // 初始屏幕位置
        float screenCenterY = visibleSize.height * GameConfig::PLAYER_GROUND_Y_PERCENT;
        _player->setPosition(Vec2(_screenWidth * 0.1f, screenCenterY));
        // 增加玩家缩放
        _player->setScale(4.0f);

        this->addChild(_player, 10);

        log("Player created at world position: (%.0f, %.0f), screen position: (%.0f, %.0f)",
            initialWorldX, initialWorldY, _player->getPositionX(), _player->getPositionY());
    }
    else {
        log("ERROR: Failed to create player!");
    }
}

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

void MainGameScene::initDebugUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建调试标签
    _debugLabel = Label::createWithSystemFont(
        "Level 1 - Camera Follow Mode",
        "Arial", 18
    );
    _debugLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 30));
    _debugLabel->setColor(Color3B::YELLOW);
    this->addChild(_debugLabel, 100);

    // 创建关卡标签
    _levelLabel = Label::createWithSystemFont(
        "Level 1",
        "Arial", 24
    );
    _levelLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 60));
    _levelLabel->setColor(Color3B::GREEN);
    this->addChild(_levelLabel, 100);

    // 创建操作提示标签 - 添加跳跃提示
    auto instructionLabel = Label::createWithSystemFont(
        "A: Move Left | D: Move Right | W: Jump | K: Attack | Reach right end to next level",
        "Arial", 16
    );
    instructionLabel->setPosition(Vec2(visibleSize.width * 0.5f, 30));
    instructionLabel->setColor(Color3B::WHITE);
    this->addChild(instructionLabel, 100);

    // 创建边界标记（调试用）
    auto boundaryMarker = DrawNode::create();

    // 在屏幕右侧画一条红线表示边界
    boundaryMarker->drawLine(
        Vec2(visibleSize.width - 50, 0),
        Vec2(visibleSize.width - 50, visibleSize.height),
        Color4F::RED
    );

    // 添加文字说明
    auto boundaryLabel = Label::createWithSystemFont(
        "Boundary",
        "Arial", 12
    );
    boundaryLabel->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 20));
    boundaryLabel->setColor(Color3B::RED);
    boundaryLabel->setRotation(90);

    this->addChild(boundaryMarker, 99);
    this->addChild(boundaryLabel, 99);
}

void MainGameScene::updateCamera(float delta) {
    if (!_player || !_currentBackground) return;

    // 获取玩家世界位置
    float playerWorldX = _player->getWorldPositionX();
    float playerWorldY = _player->getWorldPositionY();

    // 限制玩家世界位置在有效范围内
    if (playerWorldX < 0) {
        playerWorldX = 0;
        _player->setWorldPositionX(0);
    }
    else if (playerWorldX > _worldWidth) {
        playerWorldX = _worldWidth;
        _player->setWorldPositionX(_worldWidth);
    }

    // Y方向限制（不能低于地面）
    if (playerWorldY < 0) {
        playerWorldY = 0;
        _player->setWorldPositionY(0);
    }

    // 计算摄像机偏移 - 让玩家保持在屏幕的40%位置（更靠前）
    float targetPlayerScreenX = _screenWidth * 0.4f;
    float targetCameraOffset = targetPlayerScreenX - playerWorldX;

    // 确保背景不会移出边界
    if (_worldWidth > _screenWidth) {
        // 背景比屏幕宽，需要滚动
        float minOffset = -(_worldWidth - _screenWidth);
        float maxOffset = 0.0f;

        // 添加平滑过渡
        float smoothFactor = 0.1f; // 平滑因子
        targetCameraOffset = _cameraOffsetX + (targetCameraOffset - _cameraOffsetX) * smoothFactor;

        // 确保偏移量在合理范围内
        targetCameraOffset = MAX(targetCameraOffset, minOffset);
        targetCameraOffset = MIN(targetCameraOffset, maxOffset);
    }
    else {
        // 背景宽度等于屏幕宽度，不需要移动
        targetCameraOffset = 0.0f;
    }

    // 应用摄像机偏移
    _cameraOffsetX = targetCameraOffset;

    // 更新背景位置
    _currentBackground->updateWithCameraOffset(_cameraOffsetX);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 根据实际摄像机偏移重新计算玩家屏幕位置
    float actualPlayerScreenX = playerWorldX + _cameraOffsetX;
    float actualPlayerScreenY = visibleSize.height * GameConfig::PLAYER_GROUND_Y_PERCENT + playerWorldY;
    _player->setPosition(Vec2(actualPlayerScreenX, actualPlayerScreenY));

    // 调试信息
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 120 == 0) {
        log("[Camera] PlayerWorldPos: (%.0f, %.0f), CameraOffset: %.0f, ScreenPos: (%.0f, %.0f)",
            playerWorldX, playerWorldY, _cameraOffsetX, actualPlayerScreenX, actualPlayerScreenY);
    }
}

void MainGameScene::checkLevelTransition(float delta) {
    if (!_player || _isTransitioning) return;

    // 获取玩家世界位置
    float playerWorldX = _player->getWorldPositionX();

    // 检查是否到达当前关卡边界
    if (isPlayerAtRightBoundary()) {
        // 记录当前关卡状态
        auto currentLevel = _levelManager->getCurrentLevel();

        // 检查是否可以切换到下一关卡
        if (_levelManager->canSwitchToNextLevel(playerWorldX)) {
            // 开始关卡切换
            _isTransitioning = true;
            _transitionTimer = 0.0f;

            log("Starting level transition from level %d...", (int)currentLevel);
            log("Player position: %.0f, World width: %.0f", playerWorldX, _worldWidth);
        }
        else {
            log("Cannot switch to next level from level %d", (int)currentLevel);
        }
    }
}

void MainGameScene::update(float delta) {
    // 更新玩家（这会更新玩家的世界位置）
    if (_player) {
        _player->update(delta);

        // 定期打印玩家位置（用于调试）
        static float debugTimer = 0.0f;
        debugTimer += delta;
        if (debugTimer > 1.0f) {
            debugTimer = 0.0f;
            float playerWorldX = _player->getWorldPositionX();
            float playerWorldY = _player->getWorldPositionY();
            auto currentLevel = _levelManager->getCurrentLevel();

            log("[DEBUG] Level: %d, Player Pos: (%.0f, %.0f)/%.0f, At boundary: %s",
                (int)currentLevel, playerWorldX, playerWorldY, _worldWidth,
                isPlayerAtRightBoundary() ? "YES" : "NO");
        }
    }

    // 检查关卡切换
    checkLevelTransition(delta);

    // 处理关卡切换过渡效果
    if (_isTransitioning) {
        _transitionTimer += delta;

        // 简单的淡出效果
        float fadeProgress = _transitionTimer / TRANSITION_DURATION;
        if (fadeProgress >= 1.0f) {
            // 切换关卡
            switchToNextLevel();
        }
    }
    else {
        // 只有在非切换状态下才更新摄像机
        updateCamera(delta);
    }

    // 更新调试信息
    if (_player && _debugLabel) {
        float playerWorldX = _player->getWorldPositionX();
        float playerWorldY = _player->getWorldPositionY();
        float currentLevel = (int)_levelManager->getCurrentLevel();

        // 显示更详细的调试信息
        _debugLabel->setString(StringUtils::format(
            "Level: %d | Player: (%.0f, %.0f)/%.0f | Cam: %.0f | Vel: (%.0f, %.0f)",
            currentLevel, playerWorldX, playerWorldY, _worldWidth, _cameraOffsetX,
            _player->getCurrentVelocity().x, _player->getCurrentVelocity().y
        ));

        // 如果正在切换关卡，显示切换提示
        if (_isTransitioning) {
            _debugLabel->setString(StringUtils::format(
                "Switching to next level... %.0f%%",
                (_transitionTimer / TRANSITION_DURATION) * 100
            ));
            _debugLabel->setColor(Color3B::RED);
        }
        else {
            _debugLabel->setColor(Color3B::YELLOW);
        }
    }
}

void MainGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyPressed(keyCode);
    }
}

void MainGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyReleased(keyCode);
    }
}

void MainGameScene::switchToNextLevel() {
    if (!_player) return;

    // 停止玩家移动
    _player->stopMoving();

    // 获取当前关卡和下一关卡
    auto oldLevel = _levelManager->getCurrentLevel();
    LevelManager::LevelState nextLevel = _levelManager->switchToNextLevel();

    log("=== LEVEL TRANSITION ===");
    log("From level: %d, To level: %d", (int)oldLevel, (int)nextLevel);

    // 根据新关卡更新UI
    std::string levelName;

    switch (nextLevel) {
    case LevelManager::LevelState::LEVEL1:
        levelName = "Level 1";
        _levelLabel->setColor(Color3B::GREEN);
        break;
    case LevelManager::LevelState::LEVEL2_1:
        levelName = "Level 2-1";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL2_2:
        levelName = "Level 2-2";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL2_3:
        levelName = "Level 2-3";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL2_4:
        levelName = "Level 2-4";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL2_5:
        levelName = "Level 2-5";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL2_6:
        levelName = "Level 2-6";
        _levelLabel->setColor(Color3B::ORANGE);
        break;
    case LevelManager::LevelState::LEVEL3_1:
        levelName = "Level 3-1";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL3_2:
        levelName = "Level 3-2";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL3_3:
        levelName = "Level 3-3";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL3_4:
        levelName = "Level 3-4";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL3_5:
        levelName = "Level 3-5";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL3_6:
        levelName = "Level 3-6";
        _levelLabel->setColor(Color3B(128, 0, 128));
        break;
    case LevelManager::LevelState::LEVEL4_1:  // 新增
        levelName = "Level 4-1";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::LEVEL4_2:  // 新增
        levelName = "Level 4-2";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::LEVEL4_3:  // 新增
        levelName = "Level 4-3";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::LEVEL4_4:  // 新增
        levelName = "Level 4-4";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::LEVEL4_5:  // 新增
        levelName = "Level 4-5";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::LEVEL4_6:  // 新增
        levelName = "Level 4-6";
        _levelLabel->setColor(Color3B::BLUE);
        break;
    case LevelManager::LevelState::COMPLETED:
        levelName = "Game Completed!";
        _levelLabel->setColor(Color3B::RED);
        break;
    default:
        levelName = "Unknown Level";
        _levelLabel->setColor(Color3B::MAGENTA);
        break;
    }

    _levelLabel->setString(levelName);
    log("New level name: %s", levelName.c_str());

    // 重新加载背景
    if (_currentBackground) {
        _currentBackground->removeFromParent();
        _currentBackground = nullptr;
        log("Removed old background");
    }

    // 创建新背景
    _currentBackground = SceneBackground::create();
    if (_currentBackground) {
        this->addChild(_currentBackground, -10);
        _worldWidth = _currentBackground->getWorldWidth();
        log("Created new background for %s, world width: %.0f", levelName.c_str(), _worldWidth);
    }

    // 重置玩家位置到新关卡的起点
    if (_player) {
        _player->setWorldPositionX(100.0f); // 从100像素位置开始
        log("Reset player position to x=100");
    }

    // 重置摄像机
    _cameraOffsetX = 0.0f;

    // 立即更新摄像机位置
    updateCamera(0);

    // 结束切换状态
    _isTransitioning = false;

    log("Level transition completed");

}

bool MainGameScene::isPlayerAtRightBoundary() {
    if (!_player) return false;

    float playerWorldX = _player->getWorldPositionX();
    float boundaryThreshold = 150.0f; // 从50增加到150，提高灵敏度

    // 增加调试信息
    static float debugTimer = 0.0f;
    debugTimer += 1.0f / 60.0f;
    if (debugTimer > 1.0f) {
        debugTimer = 0.0f;
        log("[Boundary Check] Player X: %.0f, World Width: %.0f, Threshold: %.0f, At boundary: %s",
            playerWorldX, _worldWidth, boundaryThreshold,
            (playerWorldX >= _worldWidth - boundaryThreshold) ? "YES" : "NO");
    }

    // 如果背景完全覆盖屏幕，使用全宽检查
    if (_worldWidth >= _screenWidth) {
        return (playerWorldX >= _worldWidth - boundaryThreshold);
    }

    // 否则使用背景的实际宽度
    return (playerWorldX >= _worldWidth - boundaryThreshold);
}
