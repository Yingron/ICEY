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

// Player.cpp - �Ľ� loadAnimations ����
void Player::loadAnimations() {
    log("=== Player loadAnimations ===");

    // ���ö�������
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();

    // 加载向右跑动动画（1-18帧）
    std::vector<std::string> rightRunFrames;
    auto fileUtils = FileUtils::getInstance();
    for (int i = 1; i <= 18; i++) {
        // 尝试所有可能的路径，优先使用正确的路径
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("icey-run-to-right-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-right-%d.png", i)
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
            log("ERROR: Could not find right run frame %d", i);
        }
    }

    // 加载向左跑动动画（1-19帧）
    std::vector<std::string> leftRunFrames;
    for (int i = 1; i <= 19; i++) {
        // 尝试所有可能的路径，优先使用正确的路径
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("icey-run-to-left-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-left-%d.png", i)
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
            log("ERROR: Could not find left run frame %d", i);
        }
    }

    // ��������
    if (rightRunFrames.empty() && leftRunFrames.empty()) {
        log("ERROR: No animation frames found at all!");
        return;
    }

    // ���������ܶ�����
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

    // ���������ܶ�����
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

    // ��������������ʹ�õ�һ֡��
    if (!rightRunFrames.empty()) {
        std::vector<std::string> idleRightFrames;
        idleRightFrames.push_back(rightRunFrames[0]);
        cocos2d::Animation* idleRightAnim = createAnimationFromFiles(idleRightFrames, 1.0f);
        if (idleRightAnim) {
            _animations["idle_right"] = idleRightAnim;
            idleRightAnim->retain();
        }
    }

    if (!leftRunFrames.empty()) {
        std::vector<std::string> idleLeftFrames;
        idleLeftFrames.push_back(leftRunFrames[0]);
        cocos2d::Animation* idleLeftAnim = createAnimationFromFiles(idleLeftFrames, 1.0f);
        if (idleLeftAnim) {
            _animations["idle_left"] = idleLeftAnim;
            idleLeftAnim->retain();
        }
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

// Player.cpp - ��ȫ�޸� init �������Ƴ�������
bool Player::init(const std::string& spriteFile) {
    log("=== Player init ===");

    auto fileUtils = FileUtils::getInstance();
    std::string playerFile = "icey-run-to-right-1.png";

    log("Checking for player file: %s", playerFile.c_str());

    if (!fileUtils->isFileExist(playerFile)) {
        log("ERROR: Player file not found: %s", playerFile.c_str());

        std::vector<std::string> possiblePaths = {
            "icey-run-to-right-1.png",
            "Resources/icey-run-to-right-1.png",
            "Resources/images/icey-run-to-right-1.png",
            "Resources/images/character/icey-run-to-right-1.png",
            "Resources/images/characters/player/icey-run-to-right-1.png",
            "images/icey-run-to-right-1.png",
            "images/character/icey-run-to-right-1.png",
            "images/characters/player/icey-run-to-right-1.png",
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

        if (!found) {
            log("ERROR: Could not find player sprite anywhere!");
            if (!Sprite::init()) {
                return false;
            }
            this->setTextureRect(Rect(0, 0, 50, 100));
            this->setColor(Color3B::GREEN);

            _currentState = PlayerState::IDLE;
            _velocity = Vec2::ZERO;
            _isMovingLeft = false;
            _isMovingRight = false;
            _facingRight = true;
            _moveSpeed = 400.0f;
            _currentAnimationKey = "";

            this->setAnchorPoint(Vec2(0.5f, 0.5f));
            this->scheduleUpdate();

            return true;
        }
    }

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
    _currentAnimationKey = "";

    // ��ȫ�Ƴ�����������
    // �������κ������壬ʹ��ֱ��λ�ÿ���

    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    loadAnimations();

    this->scheduleUpdate();

    return true;
}


// Player.cpp - �޸� update ������ʹ��ֱ��λ�ÿ���
void Player::update(float delta) {
    // �����ƶ�
    if (_isMovingLeft) {
        moveLeft(delta);
    }
    else if (_isMovingRight) {
        moveRight(delta);
    }
    else if (_currentState == PlayerState::RUNNING) {
        stopMoving();
    }

    // ֱ�Ӹ���λ�ã���ʹ����������
    auto currentPos = this->getPosition();
    float newX = currentPos.x + _velocity.x * delta;
    float newY = currentPos.y + _velocity.y * delta;

    // �����ƶ���Χ����ѡ�����ݳ����߽磩
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (newX < 0) newX = 0;
    if (newX > visibleSize.width) newX = visibleSize.width;

    // ����Y��λ�ò���
    newY = currentPos.y;

    this->setPosition(Vec2(newX, newY));
}

// Player.cpp - 修改 moveLeft 和 moveRight 函数
void Player::moveLeft(float delta) {
    bool wasFacingRight = _facingRight;
    _velocity.x = -_moveSpeed;
    _facingRight = false; // 向左移动，面向左

    // 如果状态不是RUNNING，或者方向改变了，都需要更新动画
    if (_currentState != PlayerState::RUNNING || wasFacingRight != _facingRight) {
        setCurrentState(PlayerState::RUNNING);
    }
    // 注意：不要设置setFlippedX(true)，因为我们使用专门的向左动画
}

void Player::moveRight(float delta) {
    bool wasFacingRight = _facingRight;
    _velocity.x = _moveSpeed;
    _facingRight = true; // 向右移动，面向右

    // 如果状态不是RUNNING，或者方向改变了，都需要更新动画
    if (_currentState != PlayerState::RUNNING || wasFacingRight != _facingRight) {
        setCurrentState(PlayerState::RUNNING);
    }
    // 注意：不要设置setFlippedX(false)，因为我们使用专门的向右动画
}

void Player::stopMoving() {
    _velocity.x = 0;
    if (_currentState == PlayerState::RUNNING) {
        setCurrentState(PlayerState::IDLE);
    }
}
// Player.cpp - 修改状态设置逻辑
// Player.cpp - 修改 setCurrentState 函数
void Player::setCurrentState(PlayerState state) {
    bool stateChanged = (_currentState != state);
    bool needUpdateAnimation = false;
    
    // 确定需要播放的动画键
    std::string targetAnimationKey = "";
    if (state == PlayerState::RUNNING) {
        if (_velocity.x > 0 || _facingRight) {
            targetAnimationKey = "run_right";
        } else {
            targetAnimationKey = "run_left";
        }
        // 如果状态改变，或者动画键改变，都需要更新动画
        needUpdateAnimation = stateChanged || (_currentAnimationKey != targetAnimationKey);
    } else if (state == PlayerState::IDLE) {
        needUpdateAnimation = stateChanged;
    } else {
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
            // 待机状态显示第一帧
            if (_facingRight) {
                auto it = _animations.find("idle_right");
                if (it != _animations.end() && it->second->getFrames().size() > 0) {
                    auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                    this->setSpriteFrame(spriteFrame);
                    this->setFlippedX(false); // 面向右，不翻转
                    log("Set idle right frame");
                }
                else {
                    // 如果idle动画不存在，使用第一帧
                    it = _animations.find("run_right");
                    if (it != _animations.end() && it->second->getFrames().size() > 0) {
                        auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                        this->setSpriteFrame(spriteFrame);
                        this->setFlippedX(false); // 面向右，不翻转
                        log("Using run right first frame for idle");
                    }
                }
            }
            else {
                auto it = _animations.find("idle_left");
                if (it != _animations.end() && it->second->getFrames().size() > 0) {
                    auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                    this->setSpriteFrame(spriteFrame);
                    this->setFlippedX(false); // 面向左，也不翻转
                    log("Set idle left frame");
                }
                else {
                    it = _animations.find("run_left");
                    if (it != _animations.end() && it->second->getFrames().size() > 0) {
                        auto spriteFrame = it->second->getFrames().front()->getSpriteFrame();
                        this->setSpriteFrame(spriteFrame);
                        this->setFlippedX(false); // 面向左，也不翻转
                        log("Using run left first frame for idle");
                    }
                }
            }
            _currentAnimationKey = "";
        }
        break;

        // Player.cpp - 修改 setCurrentState 函数中的动画播放部分
        case PlayerState::RUNNING:
        {
            // 播放跑动动画
            std::string animationKey;
            if (_velocity.x > 0) { // 速度为正，向右移动
                animationKey = "run_right";
                _facingRight = true;
                this->setFlippedX(false); // 不翻转，使用向右跑的动画
            }
            else if (_velocity.x < 0) { // 速度为负，向左移动
                animationKey = "run_left";
                _facingRight = false;
                this->setFlippedX(false); // 不翻转，使用向左跑的动画
            }
            else {
                // 如果速度为0，根据面向方向决定动画
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

    // ������Ҽ����ͷ��ˣ�ֹͣ�ƶ�
    if (!_isMovingLeft && !_isMovingRight) {
        stopMoving();
    }
}

void Player::setupAnimations() {
    // ������Գ�ʼ�����ֶ���
    // Ŀǰ���ǻ�������ʱ�����ܶ�����
}

Player::~Player() {
    // �ͷ����л���Ķ���
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
}