#include "MainGameScene.h"
#include "GameConfig.h"
#include "Level1SceneBackground.h"  // Ensure correct header file
#include "LevelManager.h"
#include"PauseScene.h"
#include"HudManager.h"
#include"HudLayer.h"
#include"GameOverScene.h"
#include"AudioManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include <physics3d/CCPhysics3DObject.h>

USING_NS_CC;

// MainGameScene.cpp - Modified createScene function
Scene* MainGameScene::createScene() {
    // Use createWithPhysics to initialize scene with physics engine
    auto scene = Scene::createWithPhysics();
    auto layer = MainGameScene::create();
    scene->addChild(layer);

    // Configure physics world settings
    auto physicsWorld = scene->getPhysicsWorld();
    if (physicsWorld) {
        physicsWorld->setGravity(Vec2(0, -500.0f));
        physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
        log("Physics world created with gravity: (0, -500)");
    }

    return scene;
}

bool MainGameScene::init() {
    if (!Layer::init()) {
        return false;
    }



    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    log("=== MainGameScene init ===");
    log("Visible size: %f x %f", visibleSize.width, visibleSize.height);

    // Initialize achievement tracking variables//12/28/15.19
    _deathCount = 0;
    _airAttackCount = 0;
    _annoyNarratorCount = 0;

    // Check if first launch//12/28/15.19
    checkFirstLaunch();

    // Store screen width
    _screenWidth = visibleSize.width;

    // Initialize level manager
    _levelManager = LevelManager::getInstance();

    // Initialize camera
    initCamera();

    // Initialize background
    initBackground();

    // Initialize player
    initPlayer();

    // Initialize input
    initInput();

    initHud();//hy

    initItemBag();

    // Initialize debug UI
    initDebugUI();

    // Initialize level transition state
    _isTransitioning = false;
    _transitionTimer = 0.0f;

    // Schedule update
    this->scheduleUpdate();

    // Initialize items
    initItems();

    // Initialize collection UI
    showCollectionUI();

    // Initialize enemies
    initEnemies();

    return true;
}

void MainGameScene::initItemBag() {
    // 创建背包层
    _itemBagLayer = ItemBagLayer::create();
    _itemBagLayer->setLocalZOrder(1000); // 确保在最上层显示
    this->addChild(_itemBagLayer);
}

// MainGameScene.cpp - initHud function implementation
void MainGameScene::initHud() {
    log("=== initHUD ===");

    // Create HUD layer
    _hudLayer = HudLayer::create();
    if (_hudLayer) {
        // Set pause button callback
        _hudLayer->setPauseCallback([this](Ref* sender) {
            Director::getInstance()->pause();
            AudioManager::getInstance()->pauseAllVoices(); // 暂停旁白
            auto pauseScene = PauseScene::createScene();
            Director::getInstance()->pushScene(pauseScene);

            log("Game paused");
            });

        // Add HUD layer to scene, ensure it's visible on top
        this->addChild(_hudLayer, 1000); // Use higher z-order

        // Register with HUD Manager
        HudManager::getInstance()->setHudLayer(_hudLayer);

        // Set initial values
        _hudLayer->updateHealth(100.0f);
        _hudLayer->updateSheld(10);

        // !!! IMPORTANT: Connect DashBar to Player !!!
        if (_player && _hudLayer->getDashBar()) {
            _player->setDashBar(_hudLayer->getDashBar());
            log("DashBar connected to Player successfully");
        }

        log("HUD Layer initialized successfully");
    }
    else {
        log("ERROR: Failed to create HUD Layer!");
    }
}

void MainGameScene::onEnter() {
    Layer::onEnter();

    // Get physics world directly from parent scene
    auto scene = dynamic_cast<cocos2d::Scene*>(this->getParent());
    if (scene) {
        auto physicsWorld = scene->getPhysicsWorld();
        if (physicsWorld) {
            physicsWorld->setGravity(Vec2(0, -500.0f));
            physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
            log("Physics world configured successfully");

            // Create contact listener
            _contactListener = EventListenerPhysicsContact::create();
            _contactListener->onContactBegin = CC_CALLBACK_1(MainGameScene::onContactBegin, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(_contactListener, this);
            log("Physics contact listener added");
        }
    }
    else {
        log("WARNING: Physics world not available");
    }
}

void MainGameScene::onExit() {
    Layer::onExit();

    // 清理玩家资源
    if (_player) {
        _player->removeFromParentAndCleanup(true);
        _player = nullptr;
        log("Player resources cleaned up in onExit");
    }

    // 清理敌人管理器
    if (_enemyManager) {
        _enemyManager->removeAllEnemies();
        _enemyManager->cleanup();
        EnemyManager::destroyInstance(); // 销毁单例实例
        _enemyManager = nullptr;
        log("EnemyManager destroyed in onExit");
    }

    // 移除碰撞监听器
    if (_contactListener) {
        _eventDispatcher->removeEventListener(_contactListener);
        _contactListener = nullptr;
        log("Collision contact listener removed in onExit");
    }

    // 清理场景中的敌人列表（非拥有权）
    _enemiesList.clear();
    log("Scene enemy pointers cleared in onExit");

    // Clean up level manager reference
    if (_levelManager) {
        _levelManager = nullptr;
    }

    // Clean up other resources
    if (_levelLabel) {
        _levelLabel->removeFromParentAndCleanup(true);
        _levelLabel = nullptr;
    }

    // Clean up item resources
    for (auto item : _levelItems) {
        item->removeFromParentAndCleanup(true);
    }
    _levelItems.clear();
}

// MainGameScene.cpp - Fixed initItems function
void MainGameScene::initItems() {
    // Clear existing items
    for (auto item : _levelItems) {
        item->removeFromParent();
    }
    _levelItems.clear();

    auto itemManager = ItemManager::getInstance();
    auto currentLevel = _levelManager->getCurrentLevel();

    log("Initializing items for level: %d", (int)currentLevel);

    // Place items based on current level
    if (currentLevel == LevelManager::LevelState::LEVEL1) {
        // Place items in Cherry Blossom Avenue
        placeItemAt("study_du", 500, 100);
        placeItemAt("oolong", 1200, 150);
        placeItemAt("campus_card", 1800, 100);
        placeItemAt("do_not_know", 2200, 120);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_1) {
        // Place items in Gymnasium
        placeItemAt("safety_helmet", 600, 100);
        placeItemAt("poster", 900, 120);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_2) {
        placeItemAt("take_out", 500, 100);
        placeItemAt("donation_certificate", 800, 120);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_3) {
        placeItemAt("hanoi_tower", 600, 100);
        placeItemAt("sj_larry", 900, 120);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_4) {
        placeItemAt("65472", 500, 100);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_1) {
        placeItemAt("bulletin_board", 500, 120);
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_2) {
        placeItemAt("trash_bin_1", 600, 100);
    }
    // Can add items for other levels as needed

    log("Total items in level: %d", _levelItems.size());
}

// MainGameScene.cpp - Updated placeItemAt function
void MainGameScene::placeItemAt(const std::string & itemId, float worldX, float worldY) {
    Item* item = ItemManager::getInstance()->createItem(itemId);
    if (item) {
        // Set world position
        item->setWorldPosition(worldX, worldY);

        // Set tag for identification
        item->setTag(1000);

        // Add to scene
        this->addChild(item, 5);

        // Add to items list
        _levelItems.push_back(item);

        log("Placed item %s at (%.0f, %.0f)", itemId.c_str(), worldX, worldY);
    }
    else {
        log("ERROR: Failed to create item: %s", itemId.c_str());
    }
}

void MainGameScene::removeItem(Item* item) {
    if (!item) {
        log("Attempting to remove null item");
        return;
    }
    
    // 从物品列表中移除
    auto it = std::find(_levelItems.begin(), _levelItems.end(), item);
    if (it != _levelItems.end()) {
        // 从场景中移除
        item->removeFromParent();
        delete item;
        _levelItems.erase(it);
        log("Item removed successfully");
    } else {
        log("Item not found in _levelItems for removal");
    }
}

void MainGameScene::checkItemCollisions(float delta) {
    if (!_player) return;

    auto playerRect = _player->getBoundingBox();

    for (auto it = _levelItems.begin(); it != _levelItems.end();) {
        auto item = *it;
        auto itemRect = item->getBoundingBox();

        if (playerRect.intersectsRect(itemRect)) {
            item->collect();
            it = _levelItems.erase(it);

            // 更新UI
            updateCollectionUI();

            // 触发成就检查//12/28/15.19
            onCollectItem();  
        }
        else {
            ++it;
        }
    }
}

// MainGameScene.cpp - Fixed showCollectionUI function//12/28/15.19
void MainGameScene::showCollectionUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _collectionLabel = Label::createWithSystemFont("物品收集: 0/13", "Arial", 20);
    _collectionLabel->setPosition(Vec2(visibleSize.width - 120, visibleSize.height - 30));
    _collectionLabel->setColor(Color3B::GREEN);
    _collectionLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(_collectionLabel, 100);

    // 初始更新一次UI
    updateCollectionUI();
}

// MainGameScene.cpp - Updated updateCollectionUI function//12/28/15.19
void MainGameScene::updateCollectionUI() {
    if (!_collectionLabel) return;

    auto itemManager = ItemManager::getInstance();
    int collected = itemManager->getCollectedCount();
    int total = itemManager->getTotalItemCount();

    _collectionLabel->setString(StringUtils::format("Items Collected: %d/%d", collected, total));

    // Show special effect when all items are collected
    if (collected == total) {
        _collectionLabel->setColor(Color3B::RED);
        _collectionLabel->stopAllActions();
        _collectionLabel->runAction(RepeatForever::create(
            Sequence::create(
                ScaleTo::create(0.5f, 1.2f),
                ScaleTo::create(0.5f, 1.0f),
                nullptr
            )
        ));
    }
}

bool MainGameScene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    // 检查是否是玩家和物品的碰撞
    if ((nodeA == _player && dynamic_cast<Item*>(nodeB)) ||
        (nodeB == _player && dynamic_cast<Item*>(nodeA))) {

        Item* item = dynamic_cast<Item*>(nodeA != _player ? nodeA : nodeB);
        if (item) {
            item->collect();

            // 从_levelItems中移除
            auto it = std::find(_levelItems.begin(), _levelItems.end(), item);
            if (it != _levelItems.end()) {
                _levelItems.erase(it);
            }

            updateCollectionUI();
        }
    }

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

        // 获取背景的宽度
        _worldWidth = _currentBackground->getWorldWidth();

        log("Background created successfully");
        log("World width: %.0f, Screen width: %.0f", _worldWidth, _screenWidth);

        // 初始化相机偏移为0
        _cameraOffsetX = 0.0f;

        // 关键：根据相机偏移更新背景位置
        _currentBackground->updateWithCameraOffset(_cameraOffsetX);

        // 检查背景是否完全覆盖屏幕
        if (_worldWidth < _screenWidth) {
            log("WARNING: Background width (%.0f) < screen width (%.0f)", _worldWidth, _screenWidth);

            // 强制背景拉伸以覆盖屏幕
            _currentBackground->setScaleX(_screenWidth / _worldWidth);
            _worldWidth = _screenWidth;
            log("Forced background to cover screen by scaling");
        }

    }
    else {
        log("ERROR: Failed to create background");

        // 创建应急背景
        auto emergencyBackground = Sprite::create();
        emergencyBackground->setTextureRect(Rect(0, 0, _screenWidth, visibleSize.height));
        emergencyBackground->setColor(Color3B(30, 30, 60)); // 深色备用背景
        emergencyBackground->setPosition(Vec2(_screenWidth * 0.5f, visibleSize.height * 0.5f));
        this->addChild(emergencyBackground, -10);

        _worldWidth = _screenWidth;
        log("Created emergency background");
    }
}

// MainGameScene.cpp - 修改 initPlayer 函数
void MainGameScene::initPlayer() {
    log("=== initPlayer ===");

    _player = Player::create("icey-idle-1.png");
    if (_player) {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        float initialWorldX = _screenWidth * 0.1f;
        float initialWorldY = 0.0f;

        // 配置物理属性
        auto physicsBody = _player->getPhysicsBody();
        if (physicsBody) {
            physicsBody->setDynamic(true);
            physicsBody->setGravityEnable(false); // 关闭默认重力，改用自定义重力
            physicsBody->setRotationEnable(false);

            // 设置碰撞掩码
            physicsBody->setCategoryBitmask(0x01); // 玩家
            physicsBody->setCollisionBitmask(0x00); // 不与任何物体发生物理碰撞
            physicsBody->setContactTestBitmask(0x02 | 0x04); // 检测与物品和敌人的碰撞
        }

        _player->setWorldPositionX(initialWorldX);
        _player->setWorldPositionY(initialWorldY);

        float screenCenterY = visibleSize.height * GameConfig::PLAYER_GROUND_Y_PERCENT;
        _player->setPosition(Vec2(_screenWidth * 0.1f, screenCenterY));

        _player->setScale(2.0f);

        this->addChild(_player, 10);

        // 设置MainGameScene指针
        _player->setMainGameScene(this);

        // 在LEVEL1玩家生成后播放旁白音频
        if (_levelManager->getCurrentLevel() == LevelManager::LevelState::LEVEL1) {
            AudioManager::getInstance()->playVoice("narrator_prologue");
            log("Playing narrator prologue in LEVEL1");
        }

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

    // 注册按下事件
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressed, this);

    // 注册抬起事件
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(MainGameScene::onKeyReleased, this);

    // 添加监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    log("Input initialized");
}

// MainGameScene.cpp - 修改 initDebugUI 函数中的乱码注释
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

    // 创建操作提示标签 - 置于底部
    auto instructionLabel = Label::createWithSystemFont(
        "A: Move Left | D: Move Right | W: Jump | K: Attack | SPACE: Dash | Reach right end to next level",
        "Arial", 16
    );
    instructionLabel->setPosition(Vec2(visibleSize.width * 0.5f, 30));
    instructionLabel->setColor(Color3B::WHITE);
    this->addChild(instructionLabel, 100);

    // 创建右侧边界标记（示意）
    auto boundaryMarker = DrawNode::create();

    // 在屏幕右侧画一条线表示边界
    boundaryMarker->drawLine(
        Vec2(visibleSize.width - 50, 0),
        Vec2(visibleSize.width - 50, visibleSize.height),
        Color4F::RED
    );

    // 边界文字说明
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

// MainGameScene.cpp - 修改 updateCamera 函数
void MainGameScene::updateCamera(float delta) {
    if (!_player || !_currentBackground) return;

    float playerWorldX = _player->getWorldPositionX();
    float playerWorldY = _player->getWorldPositionY();

    if (playerWorldX < 0) {
        playerWorldX = 0;
        _player->setWorldPositionX(0);
    }
    else if (playerWorldX > _worldWidth) {
        playerWorldX = _worldWidth;
        _player->setWorldPositionX(_worldWidth);
    }

    if (playerWorldY < 0) {
        playerWorldY = 0;
        _player->setWorldPositionY(0);
    }

    float targetPlayerScreenX = _screenWidth * 0.4f;
    float targetCameraOffset = targetPlayerScreenX - playerWorldX;

    if (_worldWidth > _screenWidth) {

        float minOffset = -(_worldWidth - _screenWidth);
        float maxOffset = 0.0f;

        float smoothFactor = 0.1f;
        targetCameraOffset = _cameraOffsetX + (targetCameraOffset - _cameraOffsetX) * smoothFactor;

        targetCameraOffset = MAX(targetCameraOffset, minOffset);
        targetCameraOffset = MIN(targetCameraOffset, maxOffset);
    }
    else {

        targetCameraOffset = 0.0f;
    }

    _cameraOffsetX = targetCameraOffset;

    _currentBackground->updateWithCameraOffset(_cameraOffsetX);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    float actualPlayerScreenX = playerWorldX + _cameraOffsetX;

    // 修正：角色的屏幕 Y 应保持在地面高度（除非跳跃时映射）
    // 由于跳跃高度已经用世界坐标计算，这里只需映射到屏幕
    float groundScreenY = visibleSize.height * GameConfig::PLAYER_GROUND_Y_PERCENT;

    // 当玩家在地面上时，使用固定的地面 Y 坐标
    if (_player->isGrounded()) {
        _player->setPosition(Vec2(actualPlayerScreenX, groundScreenY));
    }
    else {
        // 跳跃中：将世界 Y 映射到屏幕，略做缩放以更自然
        float jumpScaleFactor = 0.5f; // 调小可让跳跃更自然
        float actualPlayerScreenY = groundScreenY + (playerWorldY * jumpScaleFactor);
        _player->setPosition(Vec2(actualPlayerScreenX, actualPlayerScreenY));
    }

    static int frameCount = 0;
    frameCount++;
    if (frameCount % 120 == 0) {
        log("[Camera] PlayerWorldPos: (%.0f, %.0f), CameraOffset: %.0f, ScreenPos: (%.0f, %.0f)",
            playerWorldX, playerWorldY, _cameraOffsetX, actualPlayerScreenX, _player->getPositionY());
    }
}

float MainGameScene::getCameraOffsetX() const {
    return _cameraOffsetX;
}

void MainGameScene::checkLevelTransition(float delta) {
    if (!_player || _isTransitioning) return;

    // Get player's current position
    float playerWorldX = _player->getWorldPositionX();
    auto currentLevel = _levelManager->getCurrentLevel();

    // Check if player reached current level boundary
    if (isPlayerAtRightBoundary()) {
        // Check if all enemies are defeated, except for level1
        if (currentLevel != LevelManager::LevelState::LEVEL1) {
            if (_enemyManager && _enemyManager->getAliveEnemiesCount() > 0) {
                log("Cannot transition: %d enemies still alive", _enemyManager->getAliveEnemiesCount());
                return;
            }
        }

        // 判断是否可以切换到下一关
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
    // 更新玩家
    if (_player) {
        _player->update(delta);

        // 定期检查玩家动画状态是否正常
        static float stateCheckTimer = 0.0f;
        stateCheckTimer += delta;
        if (stateCheckTimer > 0.5f) {
            stateCheckTimer = 0.0f;

            // 如果玩家在冲刺状态但冲刺已经结束，强制重置状态
            if (_player->isDashing()) {
                // 这里可以添加检查冲刺是否应该结束的逻辑
            }
        }
    }

    // 检查是否跳跃到上边界//12/28/15.19
    if (_player && _player->getWorldPositionY() > Director::getInstance()->getVisibleSize().height * 0.8f) {
        static bool boundaryTriggered = false;
        if (!boundaryTriggered) {
            AchievementSystem::getInstance()->triggerTouchTopBoundary();
            boundaryTriggered = true;
        }
    }

    // 处理关卡切换
    checkLevelTransition(delta);

    checkPlayerHealth();//******************hy

    // 关卡切换过渡效果
    if (_isTransitioning) {
        _transitionTimer += delta;

        // 简单的淡入淡出效果
        float fadeProgress = _transitionTimer / TRANSITION_DURATION;
        if (fadeProgress >= 1.0f) {
            // 切换关卡
            switchToNextLevel();
        }
    }
    else {
        // 仅在非切换状态下更新相机
        updateCamera(delta);
    }

    // 检查道具碰撞
    checkItemCollisions(delta);

    // 更新敌人
    updateEnemies(delta);

    // 检查敌人碰撞
    checkEnemyCollisions(delta);

    // 清理死亡敌人
    cleanupDeadEnemies();

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

        // 如果正在切换关卡则显示切换提示
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

void MainGameScene::checkPlayerHealth() {
    float currentHealth = HudManager::getCurrentHealth();

    // If health is 0 and game is not over, show game over
    if (currentHealth <= 0.0f && !_isGameOver) {
        log("Health is 0, showing game over");
        showGameOver();
    }
}

// Show game over interface//********hy
void MainGameScene::showGameOver() {
    log("Showing game over interface");
    onPlayerDeath();//12/28/15.19
־
    _isGameOver = true;

    if (_player) {
        _player->stopMoving();
        _player->setCurrentState(PlayerState::IDLE);
    }

    auto gameOverScene = GameOverScene::createScene();
    Director::getInstance()->pushScene(gameOverScene);

    log("��Ϸ������������ʾ");
}

void MainGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (_player) {
        _player->onKeyPressed(keyCode);
    }
    // 添加测试热键 T 来强制重置玩家状态
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_T) {
        if (_player) {
            log("Forcing player to idle state");

            // 停止所有动作
            _player->stopAllActions();

            // 强制设置为待机状态
            _player->setCurrentState(PlayerState::IDLE);
        }
    }
    // 添加直接通关键B，按下就可以直接进入后续关卡
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        log("B key pressed, switching to next level");
        switchToNextLevel();
    }

    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)//��esc��ͣ     hy
    {
        Director::getInstance()->pause();
        AudioManager::getInstance()->pauseAllVoices(); // 暂停旁白
        auto pauseScene = PauseScene::createScene();
        Director::getInstance()->pushScene(pauseScene);

        log("Game paused");
        event->stopPropagation();
        return;
    }

    if (keyCode == EventKeyboard::KeyCode::KEY_TAB) {
        _itemBagLayer->onKeyPressed(keyCode, event);
        event->stopPropagation();
    }

    int currentMax = 0;
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
            HudManager::updateHealth(0.0f);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_5:
            HudManager::addCombo();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_6:
            HudManager::resetCombo();
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_7:
            HudManager::setCombo(10);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_8:
            HudManager::setMaxSheld(5);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_9:
            HudManager::setMaxHealth(150.0f);
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_Z:
            // B key: use dash
            if (HudManager::useDash()) {
                log("Dash used successfully");
            }
            else {
                log("Dash on cooldown, cannot use");
            }
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_X:
            // G key: recharge all dashes
            HudManager::rechargeAllDashes();
            log("All dashes recharged");
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_C:
            // M key: increase max dash count

            if (auto hudLayer = HudManager::getInstance()->getHudLayer()) {
                if (auto dashBar = hudLayer->getDashBar()) {
                    currentMax = dashBar->getMaxDashes();
                    dashBar->setMaxDashes(currentMax + 1);
                    log("Max dashes: %d -> %d", currentMax, currentMax + 1);
                }
            }
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_V:
            // N key: decrease max dash count

            if (auto hudLayer = HudManager::getInstance()->getHudLayer()) {
                if (auto dashBar = hudLayer->getDashBar()) {
                    currentMax = dashBar->getMaxDashes();
                    int newMax = currentMax - 1;
                    if (newMax < 1) newMax = 1;
                    dashBar->setMaxDashes(newMax);
                    log("����̴���: %d -> %d", currentMax, newMax);
                }
            }
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_B:
            if (auto hudLayer = HudManager::getInstance()->getHudLayer()) {
                if (auto dashBar = hudLayer->getDashBar()) {
                    float currentTime = dashBar->getRechargeTime();
                    float newTime = currentTime - 0.5f;
                    if (newTime < 0.5f) newTime = 0.5f; 
                    dashBar->setRechargeTime(newTime);
                    log("����ʱ��: %.1f�� -> %.1f��", currentTime, newTime);
                }
            }
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_N:
            if (auto hudLayer = HudManager::getInstance()->getHudLayer()) {
                if (auto dashBar = hudLayer->getDashBar()) {
                    float currentTime = dashBar->getRechargeTime();
                    float newTime = currentTime + 0.5f;
                    dashBar->setRechargeTime(newTime);
                    log("����ʱ��: %.1f�� -> %.1f��", currentTime, newTime);
                }
            }
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_0:
            static bool hudVisible = true;
            hudVisible = !hudVisible;
            HudManager::showHud(hudVisible);
            break;
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

    // 清理当前地图的敌人
    if (_enemyManager) {
        _enemyManager->removeAllEnemies();
        log("EnemyManager cleared all enemies for level transition");
    }
    // 清理场景中的敌人列表（非拥有权）
    _enemiesList.clear();
    log("Cleared enemy pointers from scene list for level transition");

    // Get current level and next level
    auto oldLevel = _levelManager->getCurrentLevel();
    LevelManager::LevelState nextLevel = _levelManager->switchToNextLevel();

    // If switching from level1 to other levels, pause narration
    if (oldLevel == LevelManager::LevelState::LEVEL1) {
        AudioManager::getInstance()->pauseAllVoices();
        log("Paused narrator audio when leaving level1");
    }

    log("=== LEVEL TRANSITION ===");
    log("From level: %d, To level: %d", (int)oldLevel, (int)nextLevel);

    // Update UI for new level
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
        case LevelManager::LevelState::LEVEL4_1:  // ����
            levelName = "Level 4-1";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::LEVEL4_2:  // ����
            levelName = "Level 4-2";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::LEVEL4_3:  // ����
            levelName = "Level 4-3";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::LEVEL4_4:  // ����
            levelName = "Level 4-4";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::LEVEL4_5:  // ����
            levelName = "Level 4-5";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::LEVEL4_6:  // ����
            levelName = "Level 4-6";
            _levelLabel->setColor(Color3B::BLUE);
            break;
        case LevelManager::LevelState::FINAL_LEVEL:
            levelName = "Final Level";
            _levelLabel->setColor(Color3B::RED);
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

    // 重置玩家位置到新关卡起点
    if (_player) {
        _player->setWorldPositionX(100.0f); // 从100的世界坐标开始
        log("Reset player position to x=100");
    }

    // 重置相机
    _cameraOffsetX = 0.0f;

    // 更新玩家屏幕位置
    updateCamera(0);

    // 清理当前关卡道具
    for (auto item : _levelItems) {
        item->removeFromParent();
    }
    _levelItems.clear();

    // 初始化新关卡道具
    initItems();

    // 初始化新关卡敌人
    initEnemies();

    // 结束切换状态
    _isTransitioning = false;

    log("Level transition completed");

}

// MainGameScene.cpp - 添加敌人管理函数
void MainGameScene::removeEnemy(Enemy* enemy) {
    if (!enemy) return;

    auto it = std::find(_enemiesList.begin(), _enemiesList.end(), enemy);
    if (it != _enemiesList.end()) {
        _enemiesList.erase(it);
        log("Enemy removed from list");
    }
}

int MainGameScene::getEnemyCount() const {
    return _enemiesList.size();
}

void MainGameScene::cleanupDeadEnemies() {
    for (auto it = _enemiesList.begin(); it != _enemiesList.end();) {
        Enemy* enemy = *it;
        if (!enemy || enemy->isDead() || !enemy->getParent() || enemy->getTag() == -1) {
            // 检查敌人是否仍然存在于 EnemyManager 中
            bool stillInManager = false;
            if (_enemyManager) {
                auto allEnemies = _enemyManager->getAllEnemies();
                for (auto mgrEnemy : allEnemies) {
                    if (mgrEnemy == enemy) {
                        stillInManager = true;
                        break;
                    }
                }
            }

            // 如果敌人不在 EnemyManager 中或者已经死亡，从场景列表中移除
            if (!stillInManager || enemy->isDead()) {
                it = _enemiesList.erase(it);
                log("Cleaned up enemy from scene list (dead or removed from manager)");
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

// 初始化敌人系统
void MainGameScene::initEnemies() {
    log("=== initEnemies ===");

    // 清理上一个地图的敌人列表（非拥有权）
    _enemiesList.clear();
    log("Cleared previous level enemy pointers");

    // 获取或初始化敌人管理器实例
    _enemyManager = EnemyManager::getInstance();
    if (!_enemyManager) {
        log("ERROR: EnemyManager not initialized!");
        return;
    }

    // 清空EnemyManager中的敌人
    _enemyManager->removeAllEnemies();

    // 设置敌人的目标为玩家
    if (_player) {
        _enemyManager->setPlayer(_player);
    } else {
        log("WARNING: Player is null, cannot set as enemy target");
    }

    // 根据当前关卡生成敌人
    if (!_levelManager) {
        log("ERROR: LevelManager is null, cannot determine current level");
        return;
    }
    auto currentLevel = _levelManager->getCurrentLevel();
    log("Initializing enemies for level: %d", (int)currentLevel);

    // 定义可能的敌人类型
    const std::vector<std::string> enemyTypes = { "melee", "ranged", "shield" };

    // 设置随机种子
    srand(time(nullptr));

    // 不生成敌人的关卡列表
    if (currentLevel == LevelManager::LevelState::LEVEL3_1 ||
        currentLevel == LevelManager::LevelState::LEVEL3_2 ||
        currentLevel == LevelManager::LevelState::LEVEL4_3 ||
        currentLevel == LevelManager::LevelState::LEVEL4_5 ||
        currentLevel == LevelManager::LevelState::LEVEL4_6 ||
        currentLevel == LevelManager::LevelState::FINAL_LEVEL) {
        log("This level is in no-enemy list, skipping enemy generation");
        return;
    }

    // 根据关卡类型生成不同配置的敌人
    if (currentLevel == LevelManager::LevelState::LEVEL1) {
        // Level1：教程关卡，不生成任何敌人
        log("Level 1 is tutorial level, no enemies generated");
    }
    else if (currentLevel >= LevelManager::LevelState::LEVEL2_1 &&
        currentLevel <= LevelManager::LevelState::LEVEL4_6) {

        // 检查当前关卡是否有BOSS
        bool hasBoss = false;
        // 3个boss分别生成在level2-6, level3-6, level4-4
        if (currentLevel == LevelManager::LevelState::LEVEL2_6 ||
            currentLevel == LevelManager::LevelState::LEVEL3_6 ||
            currentLevel == LevelManager::LevelState::LEVEL4_4) {
            hasBoss = true;
        }

        // 普通敌人数：3-5个
        // 如果有关卡boss，则不生成普通敌人（确保玩家专注于boss战）
        int regularEnemyCount = 0;
        if (!hasBoss) {
            // 只在2-4关卡随机生成3-5个普通和精英敌人
            // 检查是否是2-4关卡
            bool isLevel2_4 = (currentLevel >= LevelManager::LevelState::LEVEL2_1 && currentLevel <= LevelManager::LevelState::LEVEL2_4) ||
                (currentLevel >= LevelManager::LevelState::LEVEL3_1 && currentLevel <= LevelManager::LevelState::LEVEL3_4) ||
                (currentLevel >= LevelManager::LevelState::LEVEL4_1 && currentLevel <= LevelManager::LevelState::LEVEL4_4);

            if (isLevel2_4) {
                regularEnemyCount = 3 + rand() % 3; // 3-5个
            }
        }

        log("Generating enemies for level %d (has boss: %s)",
            (int)currentLevel, hasBoss ? "YES" : "NO");
        log("Generating %d regular enemies", regularEnemyCount);

        // 生成普通敌人
        if (_player) {
            for (int i = 0; i < regularEnemyCount; i++) {
                // 随机选择敌人类型
                int typeIndex = rand() % enemyTypes.size();
                std::string enemyType = enemyTypes[typeIndex];

                // 获取玩家当前位置
                float playerX = _player->getWorldPositionX();

                // 在玩家前方随机位置生成敌人
                // X坐标在玩家前方200-1000单位之间，Y坐标与玩家相近
                float worldX = playerX + 200 + rand() % 800;
                float worldY = _player->getWorldPositionY() + (rand() % 100) - 50;

                // 创建敌人
                createEnemyAt(enemyType, worldX, worldY);
            }
        } else {
            log("WARNING: Player is null, cannot generate regular enemies with player position");
        }

        // 生成BOSS（如果有）
        if (hasBoss) {
            if (_player) {
                // 获取玩家当前位置
                float playerX = _player->getWorldPositionX();
                float worldX = playerX + 800; // BOSS在更远的位置
                float worldY = _player->getWorldPositionY();

                // 根据关卡确定BOSS类型
                std::string bossType = "";

                // level2-6生成BOSS1-CAIXUNKUN
                if (currentLevel == LevelManager::LevelState::LEVEL2_6) {
                    bossType = "BOSS1-CAIXUNKUN";
                }
                // level3-6生成BOSS2-MAODIE
                else if (currentLevel == LevelManager::LevelState::LEVEL3_6) {
                    bossType = "BOSS2-MAODIE";
                }
                // level4-4生成BOSS3-NAILONG
                else if (currentLevel == LevelManager::LevelState::LEVEL4_4) {
                    bossType = "BOSS3-NAILONG";
                }

                if (!bossType.empty()) {
                    log("Generating boss: %s at position (%.0f, %.0f)", bossType.c_str(), worldX, worldY);
                    createEnemyAt(bossType, worldX, worldY);
                }
            } else {
                log("WARNING: Player is null, cannot generate boss with player position");
            }
        }
    }
    else if (currentLevel == LevelManager::LevelState::FINAL_LEVEL) {
        // 最终关卡：生成3个强力敌人
        int enemyCount = 3;
        log("Generating %d enemies for Final Level", enemyCount);

        if (_player) {
            for (int i = 0; i < enemyCount; i++) {
                // 最终关卡使用所有敌人类型
                std::string enemyType = enemyTypes[i % enemyTypes.size()];

                // 计算敌人位置
                float playerX = _player->getWorldPositionX();
                float worldX = playerX + 300 + (i * 300);
                float worldY = _player->getWorldPositionY() + 50;

                createEnemyAt(enemyType, worldX, worldY);
            }
        } else {
            log("WARNING: Player is null, cannot generate final level enemies with player position");
        }
    }

    log("Enemy initialization completed. Total enemies: %d", _enemiesList.size());
}

// ���˵ļ���
void MainGameScene::updateEnemies(float delta) {
    if (_enemyManager) {
        _enemyManager->update(delta);
    }
}

// ���˲�ײ
void MainGameScene::checkEnemyCollisions(float delta) {
    if (!_player || !_enemyManager) return;

    // TODO: �����˲�ײ��ʵ��
    // ��ǰ���˲�ײ��EnemyManager��ͨ��PhysicsContact::beginʵ��
}

// MainGameScene.cpp - 修改 createEnemyAt 函数
void MainGameScene::createEnemyAt(const std::string& enemyTypeStr, float worldX, float worldY) {
    if (!_enemyManager) return;

    Enemy* enemy = _enemyManager->createEnemy(enemyTypeStr, worldX, worldY);
    if (enemy) {
        // 设置敌人位置
        enemy->setWorldPosition(worldX, worldY);

        // 设置标签用于识别
        enemy->setTag(2000);

        // 添加到场景
        this->addChild(enemy, 5);

        // 添加到敌人列表
        _enemiesList.push_back(enemy);

        log("Placed enemy type %s at (%.0f, %.0f)", enemyTypeStr.c_str(), worldX, worldY);
    }
    else {
        log("ERROR: Failed to create enemy type: %s", enemyTypeStr.c_str());
    }
}

bool MainGameScene::isPlayerAtRightBoundary() {
    if (!_player) return false;

    float playerWorldX = _player->getWorldPositionX();
    float boundaryThreshold = 150.0f; // 将50扩大到150，增加缓冲区域

    // 打印调试信息
    static float debugTimer = 0.0f;
    debugTimer += 1.0f / 60.0f;
    if (debugTimer > 1.0f) {
        debugTimer = 0.0f;
        log("[Boundary Check] Player X: %.0f, World Width: %.0f, Threshold: %.0f, At boundary: %s",
            playerWorldX, _worldWidth, boundaryThreshold,
            (playerWorldX >= _worldWidth - boundaryThreshold) ? "YES" : "NO");
    }

    // 如果世界宽度大于屏幕宽度，使用完整世界宽度判断
    if (_worldWidth >= _screenWidth) {
        return (playerWorldX >= _worldWidth - boundaryThreshold);
    }

    // 如果世界小于屏幕，使用背景的实际宽度
    return (playerWorldX >= _worldWidth - boundaryThreshold);
}

void MainGameScene::checkFirstLaunch() {//12/28/15.19
    static bool firstLaunchChecked = false;
    
    if (!firstLaunchChecked) {
        // 触发首次启动成就
        AchievementSystem::getInstance()->triggerFirstLaunch();
        firstLaunchChecked = true;
        log("首次启动成就已触发");
    }
}

void MainGameScene::onPlayerDeath() {//12/28/15.19
    _deathCount++;

    // 每次死亡都触发成就系统
    AchievementSystem::getInstance()->triggerPlayerDeath();

    log("玩家死亡次数: %d", _deathCount);

    // 检查是否达到5次死亡，这里可以输出额外日志
    if (_deathCount >= 5) {
        log("玩家已死亡5次，应该已触发'生与死'成就");
    }
}

void MainGameScene::onJumpToTopBoundary() {//12/28/15.19
    // 检查玩家是否触碰到上边界
    // 这个方法应该在玩家跳跃时调用，或者在update中检查玩家Y坐标
    float playerWorldY = _player->getWorldPositionY();
    float screenHeight = Director::getInstance()->getVisibleSize().height;
    float maxJumpHeight = screenHeight * 0.8f; // 假设上边界是屏幕高度的80%

    if (playerWorldY >= maxJumpHeight) {
        AchievementSystem::getInstance()->triggerTouchTopBoundary();
        log("玩家触碰到上边界");
    }
}

void MainGameScene::onCollectItem() {//12/28/15.19
    // 这个方法应该在收集物品时调用
    auto itemManager = ItemManager::getInstance();
    int totalCollected = itemManager->getCollectedCount();

    // 检查是否收集到所有物品
    AchievementSystem::getInstance()->triggerItemCollected(totalCollected);

    log("收集物品: %d/8", totalCollected);
}

void MainGameScene::onGameCompleted() {//12/28/15.19
    // 当游戏通关时调用
    AchievementSystem::getInstance()->triggerGameCompleted();
    log("游戏通关成就已触发");
}

