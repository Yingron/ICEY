
// Player.cpp
#include "Player.h"
#include "GameConfig.h"
#include"AudioManager.h"
#include "MainGameScene.h"
#include "Enemy.h"
#include "EnemyManager.h"

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

// Player.cpp - 修复 optimizeAttackFrames 函数中的警告
std::vector<std::string> Player::optimizeAttackFrames(const std::vector<std::string>& originalFrames, int targetFrameCount) {
    if ((int)originalFrames.size() <= targetFrameCount) {  // 将 originalFrames.size() 转换为 int
        return originalFrames;
    }

    std::vector<std::string> optimizedFrames;
    float step = (float)originalFrames.size() / targetFrameCount;

    for (int i = 0; i < targetFrameCount; i++) {
        int index = (int)(i * step);
        if (index < (int)originalFrames.size()) {  // 将 originalFrames.size() 转换为 int
            optimizedFrames.push_back(originalFrames[index]);
        }
    }

    log("Optimized attack frames from %d to %d",
        (int)originalFrames.size(), (int)optimizedFrames.size());

    return optimizedFrames;
}

// Player.cpp - 在 loadAnimations 函数中添加攻击动画加载
void Player::loadAnimations() {
    log("=== Player loadAnimations ===");

    // 清空现有动画
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();

    auto fileUtils = FileUtils::getInstance();

    // 1. 加载待机动画（27帧）
    std::vector<std::string> idleFrames;
    for (int i = 1; i <= 27; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-idle-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-idle-%d.png", i),
            StringUtils::format("icey-idle-%d.png", i),
            StringUtils::format("characters/player/icey-idle-%d.png", i),
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
    std::vector<std::string> optimizedAttackFrames = optimizeAttackFrames(normalAttackRightFrames, 20);
    // 5. 加载跳跃动画（1-17帧）- 使用已定义的 fileUtils
    std::vector<std::string> jumpFrames;
    for (int i = 1; i <= 17; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-jump-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-jump-%d.png", i),
            StringUtils::format("icey-jump-%d.png", i),
            StringUtils::format("characters/player/icey-jump-%d.png", i),
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
    // 6. 加载冲刺动画
     // 向右冲刺动画（使用向右跑动动画的帧）
    std::vector<std::string> dashRightFrames;
    for (int i = 1; i <= 18; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-right-%d.png", i),
            StringUtils::format("icey-run-to-right-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-right-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                dashRightFrames.push_back(path);
                log("Found dash right frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find dash right frame %d", i);
        }
    }

    // 向左冲刺动画（使用向左跑动动画的帧）
    std::vector<std::string> dashLeftFrames;
    for (int i = 1; i <= 19; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-run-to-left-%d.png", i),
            StringUtils::format("icey-run-to-left-%d.png", i),
            StringUtils::format("characters/player/icey-run-to-left-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                dashLeftFrames.push_back(path);
                log("Found dash left frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find dash left frame %d", i);
        }
    }
    // 创建向右冲刺动画（使用更快的帧延迟）
    if (!dashRightFrames.empty()) {
        cocos2d::Animation* dashRightAnim = createAnimationFromFiles(dashRightFrames, 0.03f); // 比跑步快
        if (dashRightAnim) {
            _animations["dash_right"] = dashRightAnim;
            dashRightAnim->retain();
            log("Created dash right animation with %d frames", (int)dashRightFrames.size());
        }
    }

    // 创建向左冲刺动画（使用更快的帧延迟）
    if (!dashLeftFrames.empty()) {
        cocos2d::Animation* dashLeftAnim = createAnimationFromFiles(dashLeftFrames, 0.03f); // 比跑步快
        if (dashLeftAnim) {
            _animations["dash_left"] = dashLeftAnim;
            dashLeftAnim->retain();
            log("Created dash left animation with %d frames", (int)dashLeftFrames.size());
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

    // 7. 加载技能1动画
    std::vector<std::string> skill1Frames;
    for (int i = 1; i <= 40; i++) { // 技能1动画有40帧
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-skill1-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-skill1-%d.png", i),
            StringUtils::format("icey-skill1-%d.png", i),
            StringUtils::format("characters/player/icey-skill1-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                skill1Frames.push_back(path);
                log("Found skill1 frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find skill1 frame %d", i);
        }
    }

    // 创建技能1动画（不区分左右方向）
    if (!skill1Frames.empty()) {
        cocos2d::Animation* skill1Anim = createAnimationFromFiles(skill1Frames, 0.05f); // 使用较快的帧延迟
        if (skill1Anim) {
            _animations["skill1"] = skill1Anim;
            skill1Anim->retain();
            log("Created skill1 animation with %d frames", (int)skill1Frames.size());
        }
        else {
            log("ERROR: Failed to create skill1 animation");
        }
    }

    // 8. 加载技能2动画
    std::vector<std::string> skill2Frames;
    for (int i = 1; i <= 25; i++) { // 技能2动画有25帧
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-skill2-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-skill2-%d.png", i),
            StringUtils::format("icey-skill2-%d.png", i),
            StringUtils::format("characters/player/icey-skill2-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                skill2Frames.push_back(path);
                log("Found skill2 frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find skill2 frame %d", i);
        }
    }

    // 创建技能2动画（不区分左右方向）
    if (!skill2Frames.empty()) {
        cocos2d::Animation* skill2Anim = createAnimationFromFiles(skill2Frames, 0.05f); // 使用较快的帧延迟
        if (skill2Anim) {
            _animations["skill2"] = skill2Anim;
            skill2Anim->retain();
            log("Created skill2 animation with %d frames", (int)skill2Frames.size());
        }
        else {
            log("ERROR: Failed to create skill2 animation");
        }
    }
}

void Player::jump() {
    // 获取当前时间
    auto currentTime = cocos2d::utils::getTimeInMilliseconds() / 1000.0f;

    // 重置跳跃计数的条件：如果在地上，或者距离上次跳跃超过0.5秒
    if (_isGrounded || (currentTime - _lastJumpTime > 0.5f)) {
        _jumpCount = 0;
    }

    // 检查是否可以跳跃（未超过最大跳跃次数）
    if (_jumpCount < _maxJumpCount) {
        _isGrounded = false;
        _velocity.y = _jumpForce;
        _jumpCount++;
        _lastJumpTime = currentTime;
        _currentState = PlayerState::JUMPING;
        setCurrentState(PlayerState::JUMPING);

        log("Player jumps (count: %d) with force: %.0f", _jumpCount, _jumpForce);
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

    _keyStates.clear();
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
    _jumpForce = 600.0f;      // 减小跳跃力量
    _gravity = 800.0f;        // 减小重力加速度
    _worldPositionY = 0.0f;   // 初始Y位置
    _jumpCount = 0;           // 初始化跳跃计数
    _maxJumpCount = 2;        // 最大二连跳
    _lastJumpTime = 0.0f;     // 初始化上次跳跃时间

    // 初始化连击相关变量
    _canCombo = false;
    _comboCount = 0;
    _comboTimer = 0.0f;
    
    // 初始化生命值相关变量
    _maxHealth = 100.0f;
    _currentHealth = _maxHealth;
    
    // 初始化护盾相关变量
    _maxShield = 3;
    _currentShield = _maxShield;
    
    // 初始化MainGameScene指针
    _mainGameScene = nullptr;

    // 初始化冲刺相关变量
    _dashSpeed = 1500.0f;
    _dashDistance = 300.0f;
    _dashDuration = 0.2f;
    // _dashCooldown = 0.0f; // 删除这行，改为使用DashBar管理
    _currentDashDistance = 0.0f;
    _isDashLeft = false;

    // 冲刺动画键
    _dashLeftAnimKey = "dash_left";
    _dashRightAnimKey = "dash_right";

    // 初始化DashBar引用为nullptr
    _dashBar = nullptr;
    
    // 初始化技能1相关变量
    _skill1Damage = 50.0f;         // 技能1伤害
    _skill1Range = 100.0f;         // 技能1范围
    _skill1Cooldown = 2.0f;        // 技能1冷却时间（秒）
    _skill1CooldownTimer = 0.0f;   // 技能1冷却计时器
    
    // 初始化技能2相关变量
    _skill2Damage = 80.0f;         // 技能2伤害
    _skill2Range = 150.0f;         // 技能2范围
    _skill2Cooldown = 3.0f;        // 技能2冷却时间（秒）
    _skill2CooldownTimer = 0.0f;   // 技能2冷却计时器
    
    // 技能动画键
    _skill1AnimKey = "skill1";
    _skill2AnimKey = "skill2";

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

            // 添加物理体
            auto physicsBody = PhysicsBody::createBox(this->getContentSize());
            if (physicsBody) {
                physicsBody->setDynamic(true);
                physicsBody->setGravityEnable(false); // 禁用物理引擎重力
                physicsBody->setCategoryBitmask(0x01); // 玩家类别
                physicsBody->setCollisionBitmask(0x00); // 不与任何物体发生物理碰撞
                physicsBody->setContactTestBitmask(0x02 | 0x04); // 检测与物品和敌人的碰撞
                this->setPhysicsBody(physicsBody);
            }

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

    // 添加物理体
    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    if (physicsBody) {
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false); // 禁用物理引擎重力
        physicsBody->setCategoryBitmask(0x01); // 玩家类别
        physicsBody->setCollisionBitmask(0x00); // 不与任何物体发生物理碰撞
        physicsBody->setContactTestBitmask(0x02 | 0x04); // 检测与物品和敌人的碰撞
        this->setPhysicsBody(physicsBody);
    }

    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadAnimations();
    setCurrentState(PlayerState::IDLE);
    this->scheduleUpdate();

    return true;
}

// Player.cpp - 在类定义中添加辅助方法
EnemyManager* Player::getSafeEnemyManager() {
    auto enemyManager = EnemyManager::getInstance();
    if (!enemyManager) {
        static bool warned = false;
        if (!warned) {
            log("CRITICAL: EnemyManager instance is null!");
            warned = true;
        }
    }
    return enemyManager;
}

// 技能1实现
void Player::skill1() {
    // 基础安全检查
    if (!this || isDead()) {
        log("Skill1: Invalid player or player is dead");
        return;
    }

    if (!canUseSkill1()) {
        log("Skill1: Cannot use skill (cooldown: %.2f, state: %d)",
            _skill1CooldownTimer, (int)_currentState);
        return;
    }

    log("=== Player uses Skill1 ===");

    // 停止当前所有动作和移动
    this->stopAllActions();
    stopMoving();
    _velocity.x = 0;

    // 设置技能状态
    _currentState = PlayerState::SKILL1;

    // 播放技能音效
    auto audioManager = AudioManager::getInstance();
    if (audioManager) {
        audioManager->playEffect("sfx_skill1");
    }

    // 检查是否有技能动画
    if (_animations.find("skill1") == _animations.end() ||
        !_animations["skill1"] ||
        _animations["skill1"]->getFrames().empty()) {
        log("ERROR: Skill1 animation not found or empty!");

        // 如果没有动画，直接应用技能效果然后返回
        applySkill1Effect();
        return;
    }

    // 获取技能动画
    auto skillAnim = _animations["skill1"];
    float totalDuration = skillAnim->getDelayPerUnit() * skillAnim->getFrames().size();

    log("Skill1 animation duration: %.2f seconds", totalDuration);

    // 播放技能动画
    auto animate = Animate::create(skillAnim);

    // 在动画中间帧应用技能效果
    float effectDelay = totalDuration * 0.5f;

    // 技能效果回调
    auto applyEffect = CallFunc::create([this]() {
        applySkill1Effect();
        });

    // 技能完成回调
    auto skillComplete = CallFunc::create([this]() {
        if (!this || !this->getParent()) {
            log("Skill1 complete: Player node no longer exists");
            return;
        }

        log("Skill1 animation complete, returning to appropriate state");

        // 恢复状态
        if (_isGrounded) {
            if (_isMovingLeft || _isMovingRight) {
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                setCurrentState(PlayerState::IDLE);
            }
        }
        else {
            setCurrentState(PlayerState::JUMPING);
        }
        });

    // 创建动画序列
    auto sequence = Sequence::create(
        animate,
        skillComplete,
        nullptr
    );

    // 创建效果序列
    auto effectSequence = Sequence::create(
        DelayTime::create(effectDelay),
        applyEffect,
        nullptr
    );

    // 运行动作
    this->runAction(sequence);
    this->runAction(effectSequence);

    // 设置冷却时间
    _skill1CooldownTimer = _skill1Cooldown;
}

// 技能1效果应用函数
void Player::applySkill1Effect() {
    log("Applying Skill1 effect");

    auto enemyManager = getSafeEnemyManager();
    if (!enemyManager) {
        log("ERROR: Cannot get EnemyManager instance");
        return;
    }

    const auto& enemies = enemyManager->getAllEnemies();
    if (enemies.empty()) {
        log("No enemies to attack with skill1");
        return;
    }

    log("Checking %d enemies for skill1", (int)enemies.size());

    int hitCount = 0;
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->isDead()) {
            continue;
        }

        float distance = std::abs(enemy->getWorldPositionX() - _worldPositionX);
        if (distance <= _skill1Range) {
            enemy->takeDamage(_skill1Damage);
            hitCount++;

            log("Skill1 hit enemy at distance %.0f, damage: %.0f, enemy health: %.0f",
                distance, _skill1Damage, enemy->getCurrentHealth());
        }
    }

    log("Skill1 hit %d enemies", hitCount);
}

// 技能2实现
void Player::skill2() {
    if (!this || isDead()) {
        log("Skill2: Invalid player or player is dead");
        return;
    }

    if (!canUseSkill2()) {
        log("Skill2: Cannot use skill (cooldown: %.2f, state: %d)",
            _skill2CooldownTimer, (int)_currentState);
        return;
    }

    log("=== Player uses Skill2 ===");

    this->stopAllActions();
    stopMoving();
    _velocity.x = 0;

    _currentState = PlayerState::SKILL2;

    auto audioManager = AudioManager::getInstance();
    if (audioManager) {
        audioManager->playEffect("sfx_skill2");
    }

    if (_animations.find("skill2") == _animations.end() ||
        !_animations["skill2"] ||
        _animations["skill2"]->getFrames().empty()) {
        log("ERROR: Skill2 animation not found or empty!");
        applySkill2Effect();
        return;
    }

    auto skillAnim = _animations["skill2"];
    float totalDuration = skillAnim->getDelayPerUnit() * skillAnim->getFrames().size();

    log("Skill2 animation duration: %.2f seconds", totalDuration);

    auto animate = Animate::create(skillAnim);
    float effectDelay = totalDuration * 0.5f;

    auto applyEffect = CallFunc::create([this]() {
        applySkill2Effect();
        });

    auto skillComplete = CallFunc::create([this]() {
        if (!this || !this->getParent()) {
            log("Skill2 complete: Player node no longer exists");
            return;
        }

        log("Skill2 animation complete");

        if (_isGrounded) {
            if (_isMovingLeft || _isMovingRight) {
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                setCurrentState(PlayerState::IDLE);
            }
        }
        else {
            setCurrentState(PlayerState::JUMPING);
        }
        });

    auto sequence = Sequence::create(animate, skillComplete, nullptr);
    auto effectSequence = Sequence::create(DelayTime::create(effectDelay), applyEffect, nullptr);

    this->runAction(sequence);
    this->runAction(effectSequence);

    _skill2CooldownTimer = _skill2Cooldown;
}

// 技能2效果应用函数
void Player::applySkill2Effect() {
    log("Applying Skill2 effect");

    auto enemyManager = getSafeEnemyManager();
    if (!enemyManager) {
        log("ERROR: Cannot get EnemyManager instance");
        return;
    }

    const auto& enemies = enemyManager->getAllEnemies();
    if (enemies.empty()) {
        log("No enemies to attack with skill2");
        return;
    }

    log("Checking %d enemies for skill2", (int)enemies.size());

    int hitCount = 0;
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->isDead()) {
            continue;
        }

        float distance = std::abs(enemy->getWorldPositionX() - _worldPositionX);
        if (distance <= _skill2Range) {
            enemy->takeDamage(_skill2Damage);
            hitCount++;

            log("Skill2 hit enemy at distance %.0f, damage: %.0f, enemy health: %.0f",
                distance, _skill2Damage, enemy->getCurrentHealth());
        }
    }

    log("Skill2 hit %d enemies", hitCount);
}

// Player.cpp - 修改 update 函数
void Player::update(float delta) {
    // 更新连击计时器
    if (_comboTimer > 0) {
        _comboTimer -= delta;
        if (_comboTimer <= 0) {
            _comboCount = 0;
            _canCombo = false;
        }
    }
    
    // 更新技能冷却时间
    if (_skill1CooldownTimer > 0) {
        _skill1CooldownTimer -= delta;
        if (_skill1CooldownTimer < 0) {
            _skill1CooldownTimer = 0;
        }
    }
    if (_skill2CooldownTimer > 0) {
        _skill2CooldownTimer -= delta;
        if (_skill2CooldownTimer < 0) {
            _skill2CooldownTimer = 0;
        }
    }
    
    // 防止无限下落 - 添加最大下落限制
    if (_worldPositionY < -1000.0f) {
        _worldPositionY = 0;
        _velocity.y = 0;
        _isGrounded = true;
        log("WARNING: Player fell too far, resetting position!");
    }
    // 更新冲刺
    updateDash(delta);
    // 更新物理（包括重力）
    updatePhysics(delta);

    // 更新世界位置
    updateWorldPosition(delta);
}

// Player.cpp - 修改 updatePhysics 函数
void Player::updatePhysics(float delta) {
    // 应用重力
    _velocity.y -= _gravity * delta;

    // 更新Y方向的世界位置
    float newWorldY = _worldPositionY + _velocity.y * delta;

    // 检查是否落地 - 添加更严格的边界检查
    if (newWorldY <= 0) {
        _worldPositionY = 0;
        _velocity.y = 0;
        _isGrounded = true;
        _jumpCount = 0; // 落地时重置跳跃计数

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
    else {
        _worldPositionY = newWorldY;
        _isGrounded = false;
    }
}

// Player.cpp - 修改updateWorldPosition函数
void Player::updateWorldPosition(float delta) {
    // 如果正在攻击或跳跃，不更新X方向位置（或减小移动速度）
    if (_currentState == PlayerState::ATTACKING || _currentState == PlayerState::DASHING) {
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

    // 如果正在冲刺，允许切换到其他状态（特别是结束冲刺时）
    if (_currentState == PlayerState::DASHING && state != PlayerState::DASHING) {
        // 允许从冲刺状态切换到其他状态
        _currentState = state;

        // 停止所有动作
        this->stopAllActions();

        // 根据新状态设置动画
        switch (state) {
        case PlayerState::IDLE:
            _currentAnimationKey = "idle";
            break;
        case PlayerState::RUNNING:
            if (_facingRight) {
                _currentAnimationKey = "run_right";
            }
            else {
                _currentAnimationKey = "run_left";
            }
            break;
        default:
            break;
        }

        // 播放对应动画
        auto it = _animations.find(_currentAnimationKey);
        if (it != _animations.end() && it->second->getFrames().size() > 0) {
            auto animate = Animate::create(it->second);
            this->runAction(RepeatForever::create(animate));
        }

        return;
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
    else if (state == PlayerState::DASHING) {
        // 冲刺状态已经在dash方法中处理，这里不需要重复处理
        return;
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
            // 播放攻击动画 - 不使用循环，只播放一次
            std::string animationKey;
            if (_facingRight) {
                animationKey = "normal_attack_right";
                this->setFlippedX(false);
            }
            else {
                animationKey = "normal_attack_right";
                this->setFlippedX(true);
            }

            _currentAnimationKey = animationKey;

            auto it = _animations.find(animationKey);
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                // 停止当前所有动作
                this->stopAllActions();

                // 创建攻击动画 - 设置不恢复原始帧，立即开始
                auto animate = Animate::create(it->second);
                float frameDelay = it->second->getDelayPerUnit();
                int totalFrames = (int)it->second->getFrames().size();
                float animationDuration = frameDelay * totalFrames;
                
                // 在攻击动画的中间帧（约第3/4处）进行伤害检测
                int damageFrame = std::max(1, std::min(totalFrames - 2, totalFrames * 3 / 4));
                float damageDelay = frameDelay * damageFrame;
                
                // 创建伤害检测回调
                auto damageCallback = CallFunc::create([this]() {
                    if (_currentState == PlayerState::ATTACKING) {
                        // 检测攻击范围内的敌人并造成伤害
                        detectAndDamageEnemies();
                    }
                });

                // 攻击动画完成后确保状态重置的回调
                auto attackCompleteCallback = CallFunc::create([this]() {
                    // 确保攻击状态被重置，防止玩家卡住
                    if (_currentState == PlayerState::ATTACKING) {
                        // 立即重置为IDLE状态
                        _currentState = PlayerState::IDLE;
                        
                        // 播放待机动画
                        if (_animations.count("idle") > 0) {
                            this->stopAllActions();
                            auto idleAnim = _animations["idle"];
                            auto idleAction = RepeatForever::create(Animate::create(idleAnim));
                            this->runAction(idleAction);
                            _currentAnimationKey = "idle";
                        }
                    }
                });

                // 攻击完成后的连击处理回调
                auto comboCallback = CallFunc::create([this]() {
                    // 允许连击
                    _canCombo = true;

                    // 如果还有连击时间窗口，等待下一击
                    if (_comboTimer > 0 && _comboCount < 3) {
                        // 保持攻击姿态一小段时间
                        auto delay = DelayTime::create(0.1f);
                        auto checkReset = CallFunc::create([this]() {
                            if (_currentState == PlayerState::ATTACKING) {
                                _currentState = PlayerState::IDLE;
                                
                                // 播放待机动画
                                if (_animations.count("idle") > 0) {
                                    this->stopAllActions();
                                    auto idleAnim = _animations["idle"];
                                    auto idleAction = RepeatForever::create(Animate::create(idleAnim));
                                    this->runAction(idleAction);
                                    _currentAnimationKey = "idle";
                                }
                            }
                        });
                        auto sequence = Sequence::create(delay, checkReset, nullptr);
                        this->runAction(sequence);
                    }
                    else {
                        // 直接重置为IDLE状态
                        _currentState = PlayerState::IDLE;
                        
                        // 播放待机动画
                        if (_animations.count("idle") > 0) {
                            this->stopAllActions();
                            auto idleAnim = _animations["idle"];
                            auto idleAction = RepeatForever::create(Animate::create(idleAnim));
                            this->runAction(idleAction);
                            _currentAnimationKey = "idle";
                        }
                    }
                });

                // 创建主序列：攻击动画 -> 攻击完成回调 -> 连击处理
                auto mainSequence = Sequence::create(
                    animate,
                    attackCompleteCallback,
                    comboCallback,
                    nullptr);
                this->runAction(mainSequence);
                
                // 单独运行伤害检测序列
                auto damageSequence = Sequence::create(
                    DelayTime::create(damageDelay),
                    damageCallback,
                    nullptr);
                this->runAction(damageSequence);

                log("Playing attack animation with %d frames, duration: %.2f seconds, damage at frame %d",
                    totalFrames,
                    it->second->getDuration(),
                    damageFrame);
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
                AudioManager::getInstance()->playEffect("sfx_jump");//新增
                log("Playing jump animation with %d frames", (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Jump animation not found or empty");
            }
        }
        break;
        
        // Player.cpp - 修改setCurrentState函数中的技能状态处理
        case PlayerState::SKILL1:
        case PlayerState::SKILL2:
        {
            // 技能状态已经在skill1()和skill2()函数中处理动画
            // 这里只设置状态，不播放动画，避免重复播放
            log("Setting skill state %d", (int)state);

            // 停止当前所有动作（但保留技能动作）
            // 注意：这里不要调用stopAllActions()，以免中断正在播放的技能动画
            _currentState = state;

            // 根据技能类型设置动画键
            if (state == PlayerState::SKILL1) {
                _currentAnimationKey = "skill1";
            }
            else {
                _currentAnimationKey = "skill2";
            }
        }
        break;
        
        case PlayerState::DEAD:
        {
            log("Player entering DEAD state");
            
            // 停止所有动作
            this->stopAllActions();
            
            // 设置死亡状态
            _currentState = PlayerState::DEAD;
            _currentAnimationKey = "dead";
            
            // 禁用并移除物理碰撞体
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->setEnabled(false);
                this->removeComponent(this->getPhysicsBody());
            }
            
            // 清理动画资源，避免内存泄漏
            for (auto& pair : _animations) {
                CC_SAFE_RELEASE(pair.second);
            }
            _animations.clear();
            
            // 重置状态变量
            _isMovingLeft = false;
            _isMovingRight = false;
            _velocity = Vec2::ZERO;
            _isGrounded = false;
            
            log("Player DEAD state processed: animations cleared, physics disabled");
        }
        break;

        default:
            break;
        }
    }
}
// Player.cpp - 修改canDash方法
bool Player::canDash() const {
    // 使用DashBar管理冲刺次数
    if (_dashBar) {
        return _dashBar->canDash();
    }

    // 如果没有DashBar，使用原来的逻辑（向后兼容）
    return (_dashCooldown <= 0.0f && _currentState != PlayerState::ATTACKING);
}

// 技能1可用性检查
bool Player::canUseSkill1() const {
    // 技能1可用条件：不在冷却中，不在攻击、冲刺、受伤、死亡或其他技能状态
    log("canUseSkill1: cooldown=%.2f, state=%d, grounded=%d", _skill1CooldownTimer, (int)_currentState, _isGrounded);
    return _skill1CooldownTimer <= 0.0f && 
           _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

// 技能2可用性检查
bool Player::canUseSkill2() const {
    // 技能2可用条件：不在冷却中，不在攻击、冲刺、受伤、死亡或其他技能状态
    log("canUseSkill2: cooldown=%.2f, state=%d, grounded=%d", _skill2CooldownTimer, (int)_currentState, _isGrounded);
    return _skill2CooldownTimer <= 0.0f && 
           _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

// Player.cpp - 添加冲刺方法实现
void Player::dash() {
    // 默认向右冲刺
    dashRight();
}

// Player.cpp - 修改 dashLeft 函数中的动画设置
void Player::dashLeft() {
    if (!canDash() || _currentState == PlayerState::ATTACKING) {
        log("Cannot dash: canDash=%d, state=%d", canDash(), (int)_currentState);
        return;
    }

    if (_dashBar && !_dashBar->useDash()) {
        log("Failed to use dash from DashBar");
        return;
    }

    // 设置冲刺状态
    _currentState = PlayerState::DASHING;
    _facingRight = false;  // 面向左
    _isDashLeft = true;
    _currentDashDistance = 0.0f;

    // 停止当前所有动作
    this->stopAllActions();

    // 播放冲刺动画
    std::string dashKey = "dash_left";
    auto it = _animations.find(dashKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        // 关键修复：向左冲刺时，如果使用向右的动画，需要水平翻转
        if (dashKey == "dash_right") {
            this->setFlippedX(true);  // 水平翻转，使其面向左
        }
        else {
            this->setFlippedX(false); // 如果已经是向左动画，不需要翻转
        }

        // 播放冲刺动画（不循环）
        auto animate = Animate::create(it->second);
        auto callback = CallFunc::create([this]() {
            // 冲刺动画结束后，根据情况回到跑步或待机状态
            if (_isMovingLeft || _isMovingRight) {
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                setCurrentState(PlayerState::IDLE);
            }
            });

        auto sequence = Sequence::create(animate, callback, nullptr);
        this->runAction(sequence);

        log("Playing dash left animation, remaining dashes: %d",
            _dashBar ? _dashBar->getAvailableDashes() : 0);
    }
    else {
        // 如果没有冲刺动画，使用跑步动画但更快，并设置翻转
        _currentState = PlayerState::RUNNING;
        this->setFlippedX(true);  // 面向左时水平翻转
        setCurrentState(PlayerState::RUNNING);
        // 设置一个标志表示正在冲刺
        _currentState = PlayerState::DASHING;
    }
}

// Player.cpp - 修改 dashRight 函数中的动画设置
void Player::dashRight() {
    if (!canDash() || _currentState == PlayerState::ATTACKING) {
        log("Cannot dash: canDash=%d, state=%d", canDash(), (int)_currentState);
        return;
    }

    if (_dashBar && !_dashBar->useDash()) {
        log("Failed to use dash from DashBar");
        return;
    }

    // 设置冲刺状态
    _currentState = PlayerState::DASHING;
    _facingRight = true;   // 面向右
    _isDashLeft = false;
    _currentDashDistance = 0.0f;

    // 停止当前所有动作
    this->stopAllActions();

    // 播放冲刺动画
    std::string dashKey = "dash_right";
    auto it = _animations.find(dashKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        // 关键修复：确保面向右时不翻转
        this->setFlippedX(false);

        // 播放冲刺动画（不循环）
        auto animate = Animate::create(it->second);
        auto callback = CallFunc::create([this]() {
            // 冲刺动画结束后，根据情况回到跑步或待机状态
            if (_isMovingLeft || _isMovingRight) {
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                setCurrentState(PlayerState::IDLE);
            }
            });

        auto sequence = Sequence::create(animate, callback, nullptr);
        this->runAction(sequence);

        log("Playing dash right animation, remaining dashes: %d",
            _dashBar ? _dashBar->getAvailableDashes() : 0);
    }
    else {
        // 如果没有冲刺动画，使用跑步动画但更快
        _currentState = PlayerState::RUNNING;
        this->setFlippedX(false);  // 面向右时不翻转
        setCurrentState(PlayerState::RUNNING);
        // 设置一个标志表示正在冲刺
        _currentState = PlayerState::DASHING;
    }
}

// Player.cpp - 修改 updateDash 函数
void Player::updateDash(float delta) {
    if (_currentState == PlayerState::DASHING) {
        // 计算冲刺移动
        float dashMove = _dashSpeed * delta;
        _currentDashDistance += dashMove;

        // 根据冲刺方向移动
        if (_isDashLeft) {
            _worldPositionX -= dashMove;
            // 防止移动到世界左侧边界之外
            if (_worldPositionX < 0) {
                _worldPositionX = 0;
                // 碰到边界则结束冲刺
                setCurrentState(PlayerState::IDLE);
            }
        }
        else {
            _worldPositionX += dashMove;
        }

        // 检查是否达到最大冲刺距离
        if (_currentDashDistance >= _dashDistance) {
            // 冲刺结束，根据情况回到跑步或待机状态
            if (_isMovingLeft || _isMovingRight) {
                // 先停止所有动作
                this->stopAllActions();
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
            else {
                // 先停止所有动作
                this->stopAllActions();
                _currentState = PlayerState::IDLE;
                setCurrentState(PlayerState::IDLE);
            }

            // 重置冲刺变量
            _currentDashDistance = 0.0f;
            _isDashLeft = false;
        }
    }
}

// Player.cpp - 修改onKeyPressed函数中的冲刺部分
void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode) {
    // 记录按键状态
    _keyStates[keyCode] = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = true;
        _facingRight = false;

        // 如果不是正在攻击或冲刺，可以移动
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING) {
            // 如果已经在跳跃中，保持跳跃状态
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = true;
        _facingRight = true;

        // 如果不是正在攻击或冲刺，可以移动
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING) {
            // 如果已经在跳跃中，保持跳跃状态
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_K) {
        // K键：普通攻击
        // 只有在非攻击状态且在地面时才能触发攻击
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING && _isGrounded) {
            _currentState = PlayerState::ATTACKING;
            setCurrentState(PlayerState::ATTACKING);
            AudioManager::getInstance()->playEffect("sfx_attack");//新增音效
            // 攻击时停止移动
            _isMovingLeft = false;
            _isMovingRight = false;
            _velocity.x = 0;

            log("Player performs normal attack");
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        // W键：跳跃 - 可以与移动同时进行
        jump();
    }
    // Player.cpp - 修改 onKeyPressed 函数中的冲刺部分
    else if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
        // Q键：技能1
        skill1();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_E) {
        // E键：技能2
        skill2();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        // 空格键：冲刺
        if (canDash()) {
            AudioManager::getInstance()->playEffect("sfx_dash");//新增
            // 使用按键状态表来判断方向
            bool aPressed = (_keyStates.find(EventKeyboard::KeyCode::KEY_A) != _keyStates.end() &&
                _keyStates[EventKeyboard::KeyCode::KEY_A]);
            bool dPressed = (_keyStates.find(EventKeyboard::KeyCode::KEY_D) != _keyStates.end() &&
                _keyStates[EventKeyboard::KeyCode::KEY_D]);

            // 明确的方向优先级
            if (aPressed && !dPressed) {
                // A键按下：向左冲刺
                dashLeft();
                log("Player dashes left (A pressed)");
            }
            else if (dPressed && !aPressed) {
                // D键按下：向右冲刺
                dashRight();
                log("Player dashes right (D pressed)");
            }
            else if (aPressed && dPressed) {
                // A和D同时按下：根据当前面向方向冲刺
                if (_facingRight) {
                    dashRight();
                    log("Player dashes right (A+D, facing right)");
                }
                else {
                    dashLeft();
                    log("Player dashes left (A+D, facing left)");
                }
            }
            else {
                // 只按空格：根据当前面向方向冲刺
                if (_facingRight) {
                    dashRight();
                    log("Player dashes right (space only, default)");
                }
                else {
                    dashLeft();
                    log("Player dashes left (space only, default)");
                }
            }
        }
        else {
            log("Cannot dash: no dash available");
        }
    }

    // 处理同时按下的情况
    if (_currentState == PlayerState::JUMPING && (_isMovingLeft || _isMovingRight)) {
        // 保持跳跃状态，但允许水平移动
        // 这已经在 updateWorldPosition 中处理了
    }
}

// Player.cpp - 修改 onKeyReleased 函数
void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode) {
    // 更新按键状态
    _keyStates[keyCode] = false;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = false;
    }

    // 如果两个移动键都释放了
    if (!_isMovingLeft && !_isMovingRight) {
        // 跳跃中保持跳跃状态
        if (_currentState != PlayerState::JUMPING && _currentState != PlayerState::ATTACKING) {
            _currentState = PlayerState::IDLE;
            setCurrentState(PlayerState::IDLE);
        }
    }
    else {
        // 如果还有一个移动键按下，且不在跳跃或攻击状态
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
    info += ", CanDash: " + std::string(canDash() ? "Yes" : "No");
    info += ", DashCooldown: " + std::to_string(_dashCooldown);
    info += ", WorldPos: (" + std::to_string(_worldPositionX) + ", " + std::to_string(_worldPositionY) + ")";
    return info;
}

void Player::setupAnimations() {
    // 此函数暂时留空，动画在loadAnimations中加载
}

void Player::takeDamage(float damage) {
    if (isDead()) {
        return;
    }
    
    // 首先消耗护盾
    if (_currentShield > 0) {
        _currentShield -= 1;
        log("Player shield reduced by 1. Shield: %d", _currentShield);
        
        // 更新HUD显示
        if (_mainGameScene && _mainGameScene->getHudLayer()) {
            _mainGameScene->getHudLayer()->updateSheld(_currentShield);
        }
    } else {
        // 护盾耗尽后消耗生命值
        _currentHealth -= damage;
        
        // 确保生命值不会小于0
        if (_currentHealth < 0) {
            _currentHealth = 0;
        }
        
        // 更新HUD显示
        if (_mainGameScene && _mainGameScene->getHudLayer()) {
            _mainGameScene->getHudLayer()->updateHealth(_currentHealth);
        }
        
        // 如果生命值为0，设置死亡状态
        if (_currentHealth == 0) {
            setCurrentState(PlayerState::DEAD);
            log("Player died! Health: %.0f", _currentHealth);
            // 触发游戏结束
            if (_mainGameScene) {
                _mainGameScene->showGameOver();
            }
        } else {
            // 否则设置受伤状态
            setCurrentState(PlayerState::HURT);
            log("Player took %.0f damage. Health: %.0f", damage, _currentHealth);
        }
    }
}

// Player.cpp - 改进detectAndDamageEnemies函数
void Player::detectAndDamageEnemies() {
    auto enemyManager = EnemyManager::getInstance();
    if (!enemyManager) {
        log("No EnemyManager found for attack detection");
        return;
    }

    const auto& enemies = enemyManager->getAllEnemies();
    if (enemies.empty()) {
        log("No enemies found for attack detection");
        return;
    }

    float attackRange = 50.0f;
    float damage = 30.0f;

    for (Enemy* enemy : enemies) {
        if (enemy && !enemy->isDead()) {
            float distance = std::abs(enemy->getWorldPositionX() - this->getWorldPositionX());

            if (distance <= attackRange) {
                enemy->takeDamage(damage);
                log("Player attacks enemy! Damage: %.0f, Enemy health: %.0f",
                    damage, enemy->getCurrentHealth());
            }
        }
    }
}

Player::~Player() {
    // 释放所有缓存的动画
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
}