
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

// Player.cpp - 在 loadAnimations 函数中添加攻击动画加载
void Player::loadAnimations() {
    log("=== Player loadAnimations ===");

    // 清空现有动画
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();

    // 1. 加载待机动画（27帧）
    std::vector<std::string> idleFrames;
    auto fileUtils = FileUtils::getInstance(); // 在这里定义一次
    for (int i = 1; i <= 27; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-idle-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-idle-%d.png", i),
            StringUtils::format("icey-idle-%d.png", i),
            StringUtils::format("characters/player/icey-idle-%d.png", i),
            StringUtils::format("C:/aishi/test2/Resources/images/characters/player/icey-idle-%d.png", i)
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                idleFrames.push_back(path);
                log("Found idle frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find idle frame %d", i);
        }
    }

    // 2. 加载向右跑动动画（1-18帧）
    std::vector<std::string> rightRunFrames;
    for (int i = 1; i <= 18; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("icey-run-to-right-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("C:/aishi/test2/Resources/images/characters/player/icey-run-to-right-%d.png", i)
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                rightRunFrames.push_back(path);
                log("Found right run frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find right run frame %d", i);
        }
    }

    // 3. 加载向左跑动动画（1-19帧）
    std::vector<std::string> leftRunFrames;
    for (int i = 1; i <= 19; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("icey-run-to-left-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("C:/aishi/test2/Resources/images/characters/player/icey-run-to-left-%d.png", i)
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                leftRunFrames.push_back(path);
                log("Found left run frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find left run frame %d", i);
        }
    }

    // 4. 加载向右普通攻击动画（1-43帧）
    std::vector<std::string> normalAttackRightFrames;
    for (int i = 1; i <= 43; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-normalattack-rignt%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-normalattack-rignt%d.png", i),
            StringUtils::format("icey-normalattack-rignt%d.png", i),
            StringUtils::format("characters/player/icey-normalattack-rignt%d.png", i),
            StringUtils::format("C:/aishi/test2/Resources/images/characters/player/icey-normalattack-rignt%d.png", i)
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                normalAttackRightFrames.push_back(path);
                log("Found normal attack right frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find normal attack right frame %d", i);
        }
    }

    // 5. 加载跳跃动画（1-17帧）- 使用已定义的 fileUtils
    std::vector<std::string> jumpFrames;
    for (int i = 1; i <= 17; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-jump-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-jump-%d.png", i),
            StringUtils::format("icey-jump-%d.png", i),
            StringUtils::format("characters/player/icey-jump-%d.png", i),
            StringUtils::format("C:/aishi/test2/Resources/images/characters/player/icey-jump-%d.png", i)
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                jumpFrames.push_back(path);
                log("Found jump frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find jump frame %d", i);
        }
    }

    // 检查是否有动画帧
    if (idleFrames.empty() && rightRunFrames.empty() && leftRunFrames.empty() &&
        normalAttackRightFrames.empty() && jumpFrames.empty()) {
        log("ERROR: No animation frames found at all!");
        return;
    }

    // 创建待机动画
    if (!idleFrames.empty()) {
        cocos2d::Animation* idleAnim = createAnimationFromFiles(idleFrames, GameConfig::Animation::IDLE_FRAME_DELAY);
        if (idleAnim) {
            _animations["idle"] = idleAnim;
            idleAnim->retain();
            log("Created idle animation with %d frames", (int)idleFrames.size());
        }
        else {
            log("ERROR: Failed to create idle animation");
        }
    }

    // 创建向右跑动动画
    if (!rightRunFrames.empty()) {
        cocos2d::Animation* runRightAnim = createAnimationFromFiles(rightRunFrames, GameConfig::Animation::RUN_FRAME_DELAY);
        if (runRightAnim) {
            _animations["run_right"] = runRightAnim;
            runRightAnim->retain();
            log("Created right run animation with %d frames", (int)rightRunFrames.size());
        }
        else {
            log("ERROR: Failed to create right run animation");
        }
    }

    // 创建向左跑动动画
    if (!leftRunFrames.empty()) {
        cocos2d::Animation* runLeftAnim = createAnimationFromFiles(leftRunFrames, GameConfig::Animation::RUN_FRAME_DELAY);
        if (runLeftAnim) {
            _animations["run_left"] = runLeftAnim;
            runLeftAnim->retain();
            log("Created left run animation with %d frames", (int)leftRunFrames.size());
        }
        else {
            log("ERROR: Failed to create left run animation");
        }
    }

    // 创建向右普通攻击动画
    if (!normalAttackRightFrames.empty()) {
        cocos2d::Animation* normalAttackRightAnim = createAnimationFromFiles(
            normalAttackRightFrames,
            GameConfig::Animation::ATTACK_FRAME_DELAY
        );
        if (normalAttackRightAnim) {
            _animations["normal_attack_right"] = normalAttackRightAnim;
            normalAttackRightAnim->retain();
            log("Created normal attack right animation with %d frames", (int)normalAttackRightFrames.size());
        }
        else {
            log("ERROR: Failed to create normal attack right animation");
        }
    }

    // 创建跳跃动画
    if (!jumpFrames.empty()) {
        cocos2d::Animation* jumpAnim = createAnimationFromFiles(jumpFrames, GameConfig::Animation::JUMP_FRAME_DELAY);
        if (jumpAnim) {
            _animations["jump"] = jumpAnim;
            jumpAnim->retain();
            log("Created jump animation with %d frames", (int)jumpFrames.size());
        }
        else {
            log("ERROR: Failed to create jump animation");
        }
    }
}

void Player::jump() {
    if (_isGrounded && _canJump && _currentState != PlayerState::ATTACKING) {
        _isGrounded = false;
        _canJump = false;
        _velocity.y = _jumpForce;
        _currentState = PlayerState::JUMPING;
        setCurrentState(PlayerState::JUMPING);

        log("Player jumps with force: %.0f", _jumpForce);
    }
}

cocos2d::Animation* Player::createAnimationFromFiles(const std::vector<std::string>& frames, float delay) {
    if (frames.empty()) {
        log("Warning: No frames provided for animation");
        return nullptr;
    }

    auto animation = Animation::create();
    int loadedFrames = 0;

    for (const auto& frameName : frames) {
        // 检查文件是否存在
        if (!FileUtils::getInstance()->isFileExist(frameName)) {
            log("File not found: %s", frameName.c_str());
            continue;
        }

        // 使用 SpriteFrameCache 加载精灵帧
        SpriteFrame* spriteFrame = nullptr;

        // 首先尝试从缓存获取
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

        // 如果缓存中没有，则从文件加载
        if (!spriteFrame) {
            // 使用正确的纹理加载方式
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                // 获取纹理的实际大小
                auto textureSize = texture->getContentSize();
                // 创建包含整个纹理的精灵帧
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, textureSize.width, textureSize.height));

                // 将精灵帧添加到缓存中
                if (spriteFrame) {
                    SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
                }
            }
        }

        if (spriteFrame && spriteFrame->getTexture()) {
            animation->addSpriteFrame(spriteFrame);
            loadedFrames++;
            log("Successfully loaded frame: %s, texture size: %f x %f",
                frameName.c_str(),
                spriteFrame->getTexture()->getContentSize().width,
                spriteFrame->getTexture()->getContentSize().height);
        }
        else {
            log("Failed to create sprite frame for: %s", frameName.c_str());
        }
    }

    if (loadedFrames == 0) {
        log("ERROR: No frames loaded for animation");
        return nullptr;
    }

    log("Loaded %d frames for animation", loadedFrames);
    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(false); // 设置为 false，避免动画结束后恢复第一帧

    return animation;
}

bool Player::init(const std::string& spriteFile) {

    log("=== Player init ===");

    // 首先初始化所有成员变量
    _currentState = PlayerState::IDLE;
    _velocity = Vec2::ZERO;
    _isMovingLeft = false;
    _isMovingRight = false;
    _facingRight = true;
    _moveSpeed = GameConfig::PLAYER_SPEED;
    _currentAnimationKey = "";
    _worldPositionX = 0;

    // 初始化跳跃相关变量
    _isGrounded = true;
    _canJump = true;
    _jumpForce = 600.0f;      // 跳跃力量
    _gravity = 1500.0f;       // 重力加速度
    _worldPositionY = 0.0f;   // 初始Y位置

    auto fileUtils = FileUtils::getInstance();
    std::string playerFile = "icey-idle-1.png";  // 使用待机动画的第一帧作为初始图像

    log("Checking for player file: %s", playerFile.c_str());

    if (!fileUtils->isFileExist(playerFile)) {
        log("ERROR: Player file not found: %s", playerFile.c_str());

        std::vector<std::string> possiblePaths = {
            "icey-idle-1.png",
            "icey-run-to-right-1.png",
            "Resources/icey-idle-1.png",
            "Resources/images/icey-idle-1.png",
            "Resources/images/character/icey-idle-1.png",
            "Resources/images/characters/player/icey-idle-1.png",
            "images/icey-idle-1.png",
            "images/character/icey-idle-1.png",
            "images/characters/player/icey-idle-1.png",
            "../Resources/icey-idle-1.png"
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

        if (!found) {
            log("ERROR: Could not find player sprite anywhere!");
            if (!Sprite::init()) {
                return false;
            }
            this->setTextureRect(Rect(0, 0, 50, 100));
            this->setColor(Color3B::GREEN);

            this->setAnchorPoint(Vec2(0.5f, 0.5f));
            loadAnimations();
            setCurrentState(PlayerState::IDLE);
            this->scheduleUpdate();

            return true;
        }
    }

    // 如果找到了文件，使用文件初始化
    if (!Sprite::initWithFile(playerFile)) {
        log("ERROR: initWithFile failed for: %s", playerFile.c_str());
        // 创建占位符
        if (!Sprite::init()) {
            return false;
        }
        this->setTextureRect(Rect(0, 0, 50, 100));
        this->setColor(Color3B::GREEN);
    }

    log("Player sprite loaded successfully, size: %f x %f",
        this->getContentSize().width, this->getContentSize().height);

    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadAnimations();
    setCurrentState(PlayerState::IDLE);
    this->scheduleUpdate();

    return true;
}

// Player.cpp - 修改 update 函数
void Player::update(float delta) {
    // 更新物理（包括重力）
    updatePhysics(delta);

    // 更新世界位置
    updateWorldPosition(delta);
}

void Player::updatePhysics(float delta) {
    // 应用重力
    _velocity.y -= _gravity * delta;

    // 更新Y方向的世界位置
    _worldPositionY += _velocity.y * delta;

    // 检查是否落地
    if (_worldPositionY <= 0) {
        _worldPositionY = 0;
        _velocity.y = 0;
        _isGrounded = true;
        _canJump = true;

        // 如果之前是跳跃状态，落地后恢复之前的移动状态
        if (_currentState == PlayerState::JUMPING) {
            if (_isMovingLeft || _isMovingRight) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                _currentState = PlayerState::IDLE;
                setCurrentState(PlayerState::IDLE);
            }
        }
    }
}

void Player::updateWorldPosition(float delta) {
    // 如果正在攻击或跳跃，不更新X方向位置（或减小移动速度）
    if (_currentState == PlayerState::ATTACKING) {
        return;
    }

    // 跳跃时移动速度减半
    float moveSpeedMultiplier = (_currentState == PlayerState::JUMPING) ? 0.5f : 1.0f;

    // 根据移动状态更新世界位置
    if (_isMovingLeft) {
        _worldPositionX -= _moveSpeed * moveSpeedMultiplier * delta;
        _velocity.x = -_moveSpeed * moveSpeedMultiplier;

        // 防止玩家移动到世界左侧边界之外
        if (_worldPositionX < 0) {
            _worldPositionX = 0;
        }
    }
    else if (_isMovingRight) {
        _worldPositionX += _moveSpeed * moveSpeedMultiplier * delta;
        _velocity.x = _moveSpeed * moveSpeedMultiplier;
    }
    else {
        _velocity.x = 0;
    }
}

// 修改移动函数，不再设置精灵位置
void Player::moveLeft(float delta) {
    _isMovingLeft = true;
    _isMovingRight = false;
}

void Player::moveRight(float delta) {
    _isMovingRight = true;
    _isMovingLeft = false;
}

void Player::stopMoving() {
    _isMovingLeft = false;
    _isMovingRight = false;
}

void Player::setCurrentState(PlayerState state) {
    bool stateChanged = (_currentState != state);
    bool needUpdateAnimation = false;

    // 跳跃期间不能切换到其他动画
    if (_currentState == PlayerState::JUMPING && state != PlayerState::JUMPING) {
        // 只有在落地后才能切换状态
        if (_isGrounded) {
            stateChanged = true;
        }
        else {
            return; // 空中保持跳跃状态
        }
    }

    // 如果当前正在攻击，并且不是要切换到攻击状态，需要特殊处理
    if (_currentState == PlayerState::ATTACKING && state != PlayerState::ATTACKING) {
        // 停止攻击动画
        this->stopAllActions();
        stateChanged = true;
    }

    // 确定需要播放的动画键
    std::string targetAnimationKey = "";
    if (state == PlayerState::RUNNING) {
        if (_velocity.x > 0 || _facingRight) {
            targetAnimationKey = "run_right";
        }
        else {
            targetAnimationKey = "run_left";
        }
        needUpdateAnimation = stateChanged || (_currentAnimationKey != targetAnimationKey);
    }
    else if (state == PlayerState::IDLE) {
        targetAnimationKey = "idle";
        needUpdateAnimation = stateChanged || (_currentAnimationKey != targetAnimationKey);
    }
    else if (state == PlayerState::ATTACKING) {
        // 根据面向方向选择攻击动画
        if (_facingRight) {
            targetAnimationKey = "normal_attack_right";
        }
        else {
            // 如果还没有向左攻击动画，暂时使用向右攻击动画但翻转
            targetAnimationKey = "normal_attack_right";
        }
        needUpdateAnimation = true; // 攻击动画总是需要更新
    }
    else if (state == PlayerState::JUMPING) {
        targetAnimationKey = "jump";
        needUpdateAnimation = true;
    }
    else {
        needUpdateAnimation = stateChanged;
    }

    if (needUpdateAnimation) {
        _currentState = state;

        // 停止当前所有动作
        this->stopAllActions();

        // 根据状态播放动画
        switch (_currentState) {
        case PlayerState::IDLE:
        {
            // 播放待机动画
            _currentAnimationKey = "idle";
            auto it = _animations.find("idle");
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                if (_facingRight) {
                    this->setFlippedX(false);
                }
                else {
                    this->setFlippedX(true);
                }

                auto animate = Animate::create(it->second);
                this->runAction(RepeatForever::create(animate));
                log("Playing idle animation with %d frames", (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Idle animation not found or empty");
                if (!this->getSpriteFrame()) {
                    this->setTextureRect(Rect(0, 0, 50, 100));
                    this->setColor(Color3B::YELLOW);
                }
            }
        }
        break;

        case PlayerState::RUNNING:
        {
            // 播放跑动动画
            std::string animationKey;
            if (_velocity.x > 0) {
                animationKey = "run_right";
                _facingRight = true;
                this->setFlippedX(false);
            }
            else if (_velocity.x < 0) {
                animationKey = "run_left";
                _facingRight = false;
                this->setFlippedX(false);
            }
            else {
                if (_facingRight) {
                    animationKey = "run_right";
                    this->setFlippedX(false);
                }
                else {
                    animationKey = "run_left";
                    this->setFlippedX(false);
                }
            }

            _currentAnimationKey = animationKey;

            auto it = _animations.find(animationKey);
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                auto animate = Animate::create(it->second);
                this->runAction(RepeatForever::create(animate));
                log("Playing %s animation with %d frames",
                    animationKey.c_str(), (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Animation not found or empty: %s", animationKey.c_str());
            }
        }
        break;

        case PlayerState::ATTACKING:
        {
            // 播放攻击动画
            std::string animationKey;
            if (_facingRight) {
                animationKey = "normal_attack_right";
                this->setFlippedX(false);
            }
            else {
                animationKey = "normal_attack_right";
                this->setFlippedX(true); // 向左时翻转
            }

            _currentAnimationKey = animationKey;

            auto it = _animations.find(animationKey);
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                auto animate = Animate::create(it->second);

                // 创建回调序列：攻击动画完成后返回待机状态
                auto callback = CallFunc::create([this]() {
                    if (_currentState == PlayerState::ATTACKING) {
                        setCurrentState(PlayerState::IDLE);
                    }
                    });

                auto sequence = Sequence::create(animate, callback, nullptr);
                this->runAction(sequence);

                log("Playing %s animation with %d frames",
                    animationKey.c_str(), (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Attack animation not found or empty: %s", animationKey.c_str());
                // 如果没有攻击动画，直接回到待机状态
                setCurrentState(PlayerState::IDLE);
            }
        }
        break;

        case PlayerState::JUMPING:
        {
            // 播放跳跃动画
            _currentAnimationKey = "jump";
            auto it = _animations.find("jump");
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                if (_facingRight) {
                    this->setFlippedX(false);
                }
                else {
                    this->setFlippedX(true);
                }

                auto animate = Animate::create(it->second);
                this->runAction(RepeatForever::create(animate));
                log("Playing jump animation with %d frames", (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Jump animation not found or empty");
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
        _facingRight = false;

        // 如果不是正在攻击，才能移动
        if (_currentState != PlayerState::ATTACKING) {
            // 跳跃时可以移动，但速度会降低
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = true;
        _isMovingLeft = false;
        _facingRight = true;

        // 如果不是正在攻击，才能移动
        if (_currentState != PlayerState::ATTACKING) {
            // 跳跃时可以移动，但速度会降低
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_K) {
        // K键：普通攻击
        // 只有在非攻击状态且在地面时才能触发攻击
        if (_currentState != PlayerState::ATTACKING && _isGrounded) {
            // 检查是否面朝右方
            if (_facingRight) {
                _currentState = PlayerState::ATTACKING;
                setCurrentState(PlayerState::ATTACKING);

                // 攻击时停止移动
                _isMovingLeft = false;
                _isMovingRight = false;
                _velocity.x = 0;

                log("Player performs normal attack to the right");
            }
            else {
                log("Player is not facing right, cannot perform right normal attack");
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        // W键：跳跃
        jump();
    }
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode) {
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = false;
    }

    // 如果两个移动键都释放了
    if (!_isMovingLeft && !_isMovingRight) {
        // 跳跃中不改变状态
        if (_currentState != PlayerState::JUMPING && _currentState != PlayerState::ATTACKING) {
            _currentState = PlayerState::IDLE;
            setCurrentState(PlayerState::IDLE);
        }
    }
    else {
        // 如果还有一个键按下，强制更新RUNNING状态（除非在跳跃中）
        if (_currentState != PlayerState::JUMPING && _currentState != PlayerState::ATTACKING) {
            _currentState = PlayerState::RUNNING;
            setCurrentState(PlayerState::RUNNING);
        }
    }
}

std::string Player::getDebugInfo() const {
    std::string info = "State: ";
    info += std::to_string((int)_currentState);
    info += ", Velocity: (" + std::to_string(_velocity.x) + ", " + std::to_string(_velocity.y) + ")";
    info += ", Facing: " + std::string(_facingRight ? "Right" : "Left");
    info += ", MovingLeft: " + std::string(_isMovingLeft ? "Yes" : "No");
    info += ", MovingRight: " + std::string(_isMovingRight ? "Yes" : "No");
    info += ", Grounded: " + std::string(_isGrounded ? "Yes" : "No");
    info += ", WorldPos: (" + std::to_string(_worldPositionX) + ", " + std::to_string(_worldPositionY) + ")";
    return info;
}

void Player::setupAnimations() {
    // 此函数暂时留空，动画在loadAnimations中加载
}

Player::~Player() {
    // 释放所有缓存的动画
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
}