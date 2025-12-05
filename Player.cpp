// Player.cpp
#include "Player.h"
#include "GameConfig.h"

USING_NS_CC;

Player* Player::create(const std::string& spriteFile) {
    Player* player = new (std::nothrow) Player();
    if (player && player->init(spriteFile)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init(const std::string& spriteFile) {
    log("=== Player init ===");

    // 更新路径检查，根据您的实际路径
    auto fileUtils = FileUtils::getInstance();
    std::string playerFile = "icey-run-to-right-1.png";

    log("Checking for player file: %s", playerFile.c_str());

    if (!fileUtils->isFileExist(playerFile)) {
        log("ERROR: Player file not found: %s", playerFile.c_str());

        // 尝试其他路径 - 根据您的实际目录结构调整
        std::vector<std::string> possiblePaths = {
            "icey-run-to-right-1.png",
            "Resources/icey-run-to-right-1.png",
            "Resources/images/icey-run-to-right-1.png",
            "Resources/images/character/icey-run-to-right-1.png",
            "Resources/images/characters/player/icey-run-to-right-1.png",  // 新增
            "images/icey-run-to-right-1.png",
            "images/character/icey-run-to-right-1.png",
            "images/characters/player/icey-run-to-right-1.png",           // 新增
            "../Resources/icey-run-to-right-1.png"
        };
        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                playerFile = path;
                found = true;
                log("Found at alternative path: %s", path.c_str());
                break;
            }
        }

        if (!found)
        {
            log("ERROR: Could not find player sprite anywhere!");
            // 输出所有搜索路径
            auto searchPaths = fileUtils->getSearchPaths();
            log("Current search paths:");
            for (const auto& path : searchPaths) {
                log("  - %s", path.c_str());
            }
            // 创建一个简单的矩形作为玩家
            if (!Sprite::init()) {
                return false;
            }
            this->setTextureRect(Rect(0, 0, 50, 100));
            this->setColor(Color3B::GREEN);

            // 继续初始化其他变量
            _currentState = PlayerState::IDLE;
            _velocity = Vec2::ZERO;
            _isMovingLeft = false;
            _isMovingRight = false;
            _facingRight = true;
            _moveSpeed = 400.0f;

            // 设置锚点
            this->setAnchorPoint(Vec2(0.5f, 0.5f));

            // 调度更新
            this->scheduleUpdate();

            return true;
        }
    }

    // 使用第一帧作为初始精灵
    if (!Sprite::initWithFile(playerFile)) {
        log("ERROR: initWithFile failed for: %s", playerFile.c_str());
        return false;
    }

    log("Player sprite loaded successfully, size: %f x %f",
        this->getContentSize().width, this->getContentSize().height);
  
    
    _currentState = PlayerState::IDLE;
    _velocity = Vec2::ZERO;
    _isMovingLeft = false;
    _isMovingRight = false;
    _facingRight = true; 
    _moveSpeed = GameConfig::PLAYER_SPEED;

    // 扢离昜燴扽俶
    auto body = PhysicsBody::createBox(this->getContentSize());
    body->setGravityEnable(true);
    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setCollisionBitmask(0x01); 
    body->setContactTestBitmask(0x02); 
    this->setPhysicsBody(body);

    // 扢离碍萸睿坫温
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 楼婥垀衄雄赒
    loadAnimations();

    // 覃仅载陔
    this->scheduleUpdate();

    return true;
}

void Player::loadAnimations() {
    log("=== Player loadAnimations ===");

    // 向右跑动动画（1-18帧）
    std::vector<std::string> rightRunFrames;
    for (int i = 1; i <= 18; i++) {
        // 更新路径列表
        std::vector<std::string> possiblePaths = {
            StringUtils::format("icey-run-to-right-%d.png", i),
            StringUtils::format("images/character/icey-run-to-right-%d.png", i),
            StringUtils::format("images/characters/player/icey-run-to-right-%d.png", i),  // 新增
            StringUtils::format("Resources/images/character/icey-run-to-right-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-right-%d.png", i),  // 新增
            StringUtils::format("character/icey-run-to-right-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-right-%d.png", i)  // 新增
        };

        std::string foundPath = "";
        for (const auto& path : possiblePaths) {
            if (FileUtils::getInstance()->isFileExist(path)) {
                foundPath = path;
                break;
            }
        }

        if (!foundPath.empty()) {
            rightRunFrames.push_back(foundPath);
            log("Found right run frame %d at: %s", i, foundPath.c_str());
        }
        else {
            log("WARNING: Could not find right run frame %d", i);
        }
    }

    // 向左跑动动画（1-19帧）
    std::vector<std::string> leftRunFrames;
    for (int i = 1; i <= 19; i++) {
        // 更新路径列表
        std::vector<std::string> possiblePaths = {
            StringUtils::format("icey-run-to-left-%d.png", i),
            StringUtils::format("images/character/icey-run-to-left-%d.png", i),
            StringUtils::format("images/characters/player/icey-run-to-left-%d.png", i),  // 新增
            StringUtils::format("Resources/images/character/icey-run-to-left-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-left-%d.png", i),  // 新增
            StringUtils::format("character/icey-run-to-left-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-left-%d.png", i)  // 新增
        };

        std::string foundPath = "";
        for (const auto& path : possiblePaths) {
            if (FileUtils::getInstance()->isFileExist(path)) {
                foundPath = path;
                break;
            }
        }

        if (!foundPath.empty()) {
            leftRunFrames.push_back(foundPath);
            log("Found left run frame %d at: %s", i, foundPath.c_str());
        }
        else {
            log("WARNING: Could not find left run frame %d", i);
        }
    }

    // 创建动画并缓存
    cocos2d::Animation* runRightAnim = createAnimationFromFiles(rightRunFrames, GameConfig::Animation::RUN_FRAME_DELAY);
    cocos2d::Animation* runLeftAnim = createAnimationFromFiles(leftRunFrames, GameConfig::Animation::RUN_FRAME_DELAY);

    // 创建待机动画（使用第一帧作为待机）
    std::vector<std::string> idleRightFrames;
    if (!rightRunFrames.empty()) {
        idleRightFrames.push_back(rightRunFrames[0]);
    }
    cocos2d::Animation* idleRightAnim = createAnimationFromFiles(idleRightFrames, 1.0f);

    std::vector<std::string> idleLeftFrames;
    if (!leftRunFrames.empty()) {
        idleLeftFrames.push_back(leftRunFrames[0]);
    }
    cocos2d::Animation* idleLeftAnim = createAnimationFromFiles(idleLeftFrames, 1.0f);

    // 将动画添加到缓存
    if (runRightAnim) {
        _animations["run_right"] = runRightAnim;
        runRightAnim->retain();
        log("Right run animation created with %d frames", (int)rightRunFrames.size());
    }

    if (runLeftAnim) {
        _animations["run_left"] = runLeftAnim;
        runLeftAnim->retain();
        log("Left run animation created with %d frames", (int)leftRunFrames.size());
    }

    if (idleRightAnim) {
        _animations["idle_right"] = idleRightAnim;
        idleRightAnim->retain();
    }

    if (idleLeftAnim) {
        _animations["idle_left"] = idleLeftAnim;
        idleLeftAnim->retain();
    }

    // 设置初始状态为向右待机
    this->setFlippedX(false);
}

cocos2d::Animation* Player::createAnimationFromFiles(const std::vector<std::string>& frames, float delay) {
    auto animation = Animation::create();

    for (const auto& frameName : frames) {
        // 检查文件是否存在
        if (FileUtils::getInstance()->isFileExist(frameName)) {
            // 创建精灵帧
            auto spriteFrame = SpriteFrame::create(frameName, Rect(0, 0, 0, 0));
            if (spriteFrame) {
                animation->addSpriteFrame(spriteFrame);
            }
            else {
                log("Failed to create sprite frame: %s", frameName.c_str());
            }
        }
        else {
            log("File not found: %s", frameName.c_str());
        }
    }

    if (animation->getFrames().empty()) {
        log("Warning: No frames loaded for animation");
        return nullptr;
    }

    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(false); // 不恢复原始帧，保持最后一帧

    return animation;
}

// Player.cpp - 修改 update 函数
void Player::update(float delta) {
    // 处理移动
    if (_isMovingLeft) {
        moveLeft(delta);
    }
    else if (_isMovingRight) {
        moveRight(delta);
    }
    else if (_currentState == PlayerState::RUNNING) {
        stopMoving();
    }

    // 更新位置 - 使用物理引擎或者直接设置位置
    auto physicsBody = this->getPhysicsBody();
    if (physicsBody) {
        // 设置速度时保持Y轴速度为0（防止重力影响）
        _velocity.y = 0;
        physicsBody->setVelocity(_velocity);

        // 保持角色在地面上
        auto pos = this->getPosition();
        if (pos.y < 100) { // 地面高度
            this->setPositionY(100);
            physicsBody->setVelocity(Vec2(_velocity.x, 0));
        }
    }
}

// Player.cpp - 改进移动函数
void Player::moveLeft(float delta) {
    _velocity.x = -_moveSpeed;
    _facingRight = false;  // 确保面向方向正确
    if (_currentState != PlayerState::RUNNING) {
        setCurrentState(PlayerState::RUNNING);
    }
    this->setFlippedX(true);  // 向左翻转
}

void Player::moveRight(float delta) {
    _velocity.x = _moveSpeed;
    _facingRight = true;   // 确保面向方向正确
    if (_currentState != PlayerState::RUNNING) {
        setCurrentState(PlayerState::RUNNING);
    }
    this->setFlippedX(false);  // 不翻转（向右）
}

// Player.cpp - 改进停止移动函数
void Player::stopMoving() {
    _velocity.x = 0;
    _velocity.y = 0;
    if (_currentState == PlayerState::RUNNING) {
        setCurrentState(PlayerState::IDLE);
    }
}
// Player.cpp - 修复动画播放逻辑
void Player::setCurrentState(PlayerState state) {
    if (_currentState != state) {
        _currentState = state;

        // 停止当前所有动作
        this->stopAllActions();

        // 根据状态播放动画
        switch (_currentState) {
        case PlayerState::IDLE:
        {
            // 根据面向方向播放待机动画
            if (_facingRight) {
                auto it = _animations.find("run_right");
                if (it != _animations.end()) {
                    // 取第一帧作为待机
                    auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                    this->setSpriteFrame(spriteFrame);
                }
            }
            else {
                auto it = _animations.find("run_left");
                if (it != _animations.end()) {
                    auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                    this->setSpriteFrame(spriteFrame);
                }
            }
        }
        break;

        case PlayerState::RUNNING:
        {
            // 播放跑动动画
            std::string animationKey;
            if (_velocity.x > 0) { // 向右移动
                animationKey = "run_right";
                _facingRight = true;
                this->setFlippedX(false);
            }
            else { // 向左移动
                animationKey = "run_left";
                _facingRight = false;
                this->setFlippedX(true);
            }

            auto it = _animations.find(animationKey);
            if (it != _animations.end()) {
                auto animate = Animate::create(it->second);
                this->runAction(RepeatForever::create(animate));
            }
        }
        break;

        default:
            break;
        }
    }
}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode) {
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = true;
        _isMovingRight = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = true;
        _isMovingLeft = false;
    }
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode) {
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = false;
    }

    // 如果左右键都释放了，停止移动
    if (!_isMovingLeft && !_isMovingRight) {
        stopMoving();
    }
}

void Player::setupAnimations() {
    // 这里可以初始化各种动画
    // 目前我们会在运行时创建跑动动画
}

Player::~Player() {
    // 释放所有缓存的动画
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
}