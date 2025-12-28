
// Player.cpp
#include "Player.h"
#include "GameConfig.h"
#include"AudioManager.h"
#include "MainGameScene.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "LevelManager.h"

USING_NS_CC;

// Initialize static instance
Player* Player::_instance = nullptr;

Player* Player::getInstance() {
    return _instance;
}

Player* Player::create(const std::string& spriteFile) {
    Player* player = new (std::nothrow) Player();
    if (player && player->init(spriteFile)) {
        player->autorelease();
        _instance = player;
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

// Player.cpp - Fixed warning in optimizeAttackFrames function
std::vector<std::string> Player::optimizeAttackFrames(const std::vector<std::string>& originalFrames, int targetFrameCount) {
    if ((int)originalFrames.size() <= targetFrameCount) {  // Convert originalFrames.size() to int
        return originalFrames;
    }

    std::vector<std::string> optimizedFrames;
    float step = (float)originalFrames.size() / targetFrameCount;

    for (int i = 0; i < targetFrameCount; i++) {
        int index = (int)(i * step);
        if (index < (int)originalFrames.size()) {  // Convert originalFrames.size() to int
            optimizedFrames.push_back(originalFrames[index]);
        }
    }

    log("Optimized attack frames from %d to %d",
        (int)originalFrames.size(), (int)optimizedFrames.size());

    return optimizedFrames;
}

// Player.cpp - Added attack animation loading in loadAnimations function
void Player::loadAnimations() {
    log("=== Player loadAnimations ===");

    // Clear existing animations
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();

    auto fileUtils = FileUtils::getInstance();

    // 1. Load idle animation (27 frames)
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

    // 2. Load right run animation (1-18 frames)
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

    // 3. Load left run animation (1-19 frames)
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

    // 4. Load right normal attack animation (1-43 frames)
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
    // 5. Load jump animation (1-17 frames) - Using already defined fileUtils
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
    // 8. Load right dash animation (1-20 frames) - Using dedicated dash animation
    std::vector<std::string> dashRightFrames;
    for (int i = 1; i <= 20; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-dash-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-dash-%d.png", i),
            StringUtils::format("C:/test3/Resources/images/characters/player/icey-dash-%d.png", i),
            StringUtils::format("icey-dash-%d.png", i),
            StringUtils::format("characters/player/icey-dash-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                dashRightFrames.push_back(path);
                log("Found dash frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find dash frame %d", i);
        }
    }

    // 9. Load hurt animation (frames 1-30) - no left/right distinction
    std::vector<std::string> hurtFrames;
    for (int i = 1; i <= 30; i++) {
        std::vector<std::string> possiblePaths = {
            StringUtils::format("images/characters/player/icey-hurt-%d.png", i),
            StringUtils::format("Resources/images/characters/player/icey-hurt-%d.png", i),
            StringUtils::format("C:/test3/Resources/images/characters/player/icey-hurt-%d.png", i),
            StringUtils::format("icey-hurt-%d.png", i),
            StringUtils::format("characters/player/icey-hurt-%d.png", i),
        };

        bool found = false;
        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                hurtFrames.push_back(path);
                log("Found hurt frame %d at: %s", i, path.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find hurt frame %d", i);
        }
    }
    // Check if there are animation frames
    if (idleFrames.empty() && rightRunFrames.empty() && leftRunFrames.empty() &&
        normalAttackRightFrames.empty() && jumpFrames.empty()) {
        log("ERROR: No animation frames found at all!");
        return;
    }

    // Create idle animation
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

    // Create right running animation
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

    // Create left running animation
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

    // Create right normal attack animation
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

    // Create jump animation
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

    // 7. Load skill 1 animation
    std::vector<std::string> skill1Frames;
    for (int i = 1; i <= 40; i++) { // Skill 1 animation has 40 frames
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

    // Create skill 1 animation (no left/right distinction)
    if (!skill1Frames.empty()) {
        cocos2d::Animation* skill1Anim = createAnimationFromFiles(skill1Frames, 0.05f); // Use faster frame delay
        if (skill1Anim) {
            _animations["skill1"] = skill1Anim;
            skill1Anim->retain();
            log("Created skill1 animation with %d frames", (int)skill1Frames.size());
        }
        else {
            log("ERROR: Failed to create skill1 animation");
        }
    }

    // 8. Load skill 2 animation
    std::vector<std::string> skill2Frames;
    for (int i = 1; i <= 25; i++) { // Skill 2 animation has 25 frames
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

    // Create skill 2 animation (no left/right distinction)
    if (!skill2Frames.empty()) {
        cocos2d::Animation* skill2Anim = createAnimationFromFiles(skill2Frames, 0.05f); // Use faster frame delay
        if (skill2Anim) {
            _animations["skill2"] = skill2Anim;
            skill2Anim->retain();
            log("Created skill2 animation with %d frames", (int)skill2Frames.size());
        }
        else {
            log("ERROR: Failed to create skill2 animation");
        }
    }
    // Create right dash animation
    if (!dashRightFrames.empty()) {
        cocos2d::Animation* dashRightAnim = createAnimationFromFiles(dashRightFrames, 0.03f);
        if (dashRightAnim) {
            dashRightAnim->setRestoreOriginalFrame(false); // Don't restore original frame
            _animations["dash_right"] = dashRightAnim;
            dashRightAnim->retain();
            log("Created dash right animation with %d frames", (int)dashRightFrames.size());
        }
    }

    // Create hurt animation
    if (!hurtFrames.empty()) {
        cocos2d::Animation* hurtAnim = createAnimationFromFiles(hurtFrames, 0.05f);
        if (hurtAnim) {
            _animations["hurt"] = hurtAnim;
            hurtAnim->retain();
            log("Created hurt animation with %d frames", (int)hurtFrames.size());
        }
    }
}

void Player::jump() {
    // Get current time
    auto currentTime = cocos2d::utils::getTimeInMilliseconds() / 1000.0f;

    // Reset jump count condition: if on ground or more than 0.5 seconds since last jump
    if (_isGrounded || (currentTime - _lastJumpTime > 0.5f)) {
        _jumpCount = 0;
    }

    // Check if can jump (haven't exceeded max jump count)
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
        // Check if file exists
        if (!FileUtils::getInstance()->isFileExist(frameName)) {
            log("File not found: %s", frameName.c_str());
            continue;
        }

        // Load sprite frame using SpriteFrameCache
        SpriteFrame* spriteFrame = nullptr; // Declare variable here

        // First try to get from cache
        spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

        // If not in cache, load from file
        if (!spriteFrame) {
            // Use correct texture loading method
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                // Get actual size of texture
                auto textureSize = texture->getContentSize();
                // Create sprite frame containing entire texture
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, textureSize.width, textureSize.height));

                // Add sprite frame to cache
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
    animation->setRestoreOriginalFrame(false); // Set to false to avoid restoring first frame after animation ends

    return animation;
}

bool Player::init(const std::string& spriteFile) {

    log("=== Player init ===");

    _keyStates.clear();
    // First initialize all member variables
    _currentState = PlayerState::IDLE;
    _velocity = Vec2::ZERO;
    _isMovingLeft = false;
    _isMovingRight = false;
    _facingRight = true;
    _moveSpeed = GameConfig::PLAYER_SPEED;
    _currentAnimationKey = "";
    _worldPositionX = 0;

    // Initialize jump-related variables
    _isGrounded = true;
    _jumpForce = 600.0f;      // Reduced jump force
    _gravity = 800.0f;        // Reduced gravity acceleration
    _worldPositionY = 0.0f;   // Initial Y position
    _jumpCount = 0;           // Initialize jump count
    _maxJumpCount = 2;        // Max double jump
    _lastJumpTime = 0.0f;     // Initialize last jump time

    // Initialize combo-related variables
    _canCombo = false;
    _comboCount = 0;
    _comboTimer = 0.0f;
    
    // Initialize attack cooldown variables
    _attackCooldown = 0.5f; // Attack cooldown is 0.5 seconds
    _currentAttackCooldown = 0.0f;
    
    // Initialize health-related variables
    _maxHealth = 100.0f;
    _currentHealth = _maxHealth;
    
    // Initialize shield-related variables
    _maxShield = 3;
    _currentShield = _maxShield;
    
    // Initialize MainGameScene pointer
    _mainGameScene = nullptr;

    // Initialize dash-related variables
    _dashSpeed = 1500.0f;
    _dashDistance = 300.0f;
    _dashDuration = 0.2f;
    // _dashCooldown = 0.0f; // Removed, now managed by DashBar
    _currentDashDistance = 0.0f;
    _isDashLeft = false;

    // Dash animation keys
    _dashLeftAnimKey = "dash_left";
    _dashRightAnimKey = "dash_right";

    // Initialize DashBar reference to nullptr
    _dashBar = nullptr;

    // Add hurt-related variable initialization
    _isInvincible = false;           // Invincible state
    _invincibleTime = 0.0f;          // Invincible time
    _hurtFlashCount = 0;             // Hurt flash count
    
    // Initialize block-related variables
    _isBlocking = false;
    _blockDuration = 1.5f;           // Block duration 1.5 seconds
    _blockCooldown = 0.5f;           // Block cooldown 0.5 seconds
    _blockReduction = 0.7f;          // Block damage reduction 70%
    _currentBlockDuration = 0.0f;
    
    // Initialize dodge-related variables
    _isDodging = false;
    _dodgeDuration = 0.3f;           // Dodge duration 0.3 seconds
    _dodgeCooldown = 1.0f;           // Dodge cooldown 1 second
    _dodgeInvincibility = 0.3f;      // Dodge invincibility time 0.3 seconds
    _currentDodgeDuration = 0.0f;

    // Initialize skill 1 variables
    _skill1Damage = 50.0f;         // Skill 1 damage
    _skill1Range = 100.0f;         // Skill 1 range
    _skill1Cooldown = 2.0f;        // Skill 1 cooldown time (seconds)
    _skill1CooldownTimer = 0.0f;   // Skill 1 cooldown timer
    
    // Initialize skill 2 variables
    _skill2Damage = 80.0f;         // Skill 2 damage
    _skill2Range = 150.0f;         // Skill 2 range
    _skill2Cooldown = 3.0f;        // Skill 2 cooldown time (seconds)
    _skill2CooldownTimer = 0.0f;   // Skill 2 cooldown timer
    
    // Skill animation keys
    _skill1AnimKey = "skill1";
    _skill2AnimKey = "skill2";

    auto fileUtils = FileUtils::getInstance();
    std::string playerFile = "icey-idle-1.png";  // Use first frame of idle animation as initial image

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

            // Add physics body
            auto physicsBody = PhysicsBody::createBox(this->getContentSize());
            if (physicsBody) {
                physicsBody->setDynamic(true);
                physicsBody->setGravityEnable(false); // Disable physics engine gravity
                physicsBody->setCategoryBitmask(0x01); // Player category
                physicsBody->setCollisionBitmask(0x00); // Don't collide physically with any objects
                physicsBody->setContactTestBitmask(0x02 | 0x04); // Detect collisions with items and enemies
                this->setPhysicsBody(physicsBody);
            }

            this->setAnchorPoint(Vec2(0.5f, 0.5f));
            loadAnimations();
            setCurrentState(PlayerState::IDLE);
            this->scheduleUpdate();

            return true;
        }
    }

    // If file is found, initialize with it
    if (!Sprite::initWithFile(playerFile)) {
        log("ERROR: initWithFile failed for: %s", playerFile.c_str());
        // Create placeholder
        if (!Sprite::init()) {
            return false;
        }
        this->setTextureRect(Rect(0, 0, 50, 100));
        this->setColor(Color3B::GREEN);
    }

    log("Player sprite loaded successfully, size: %f x %f",
        this->getContentSize().width, this->getContentSize().height);

    // Add physics body
    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    if (physicsBody) {
        physicsBody->setDynamic(true);
        physicsBody->setGravityEnable(false); // Disable physics engine gravity
        physicsBody->setCategoryBitmask(0x01); // Player category
        physicsBody->setCollisionBitmask(0x00); // Don't collide physically with any objects
        physicsBody->setContactTestBitmask(0x02 | 0x04); // Detect collisions with items and enemies
        this->setPhysicsBody(physicsBody);
    }

    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadAnimations();
    setCurrentState(PlayerState::IDLE);
    this->scheduleUpdate();

    return true;
}

// Player.cpp - Add helper method in class definition
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

// Skill 1 implementation
void Player::skill1() {
    // Basic safety checks
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

    // Stop all current actions and movement
    this->stopAllActions();
    stopMoving();
    _velocity.x = 0;

    // Set skill state
    _currentState = PlayerState::SKILL1;

    // Play skill sound effect
    auto audioManager = AudioManager::getInstance();
    if (audioManager) {
        audioManager->playEffect("sfx_skill1");
    }

    // Check if there's skill animation
    if (_animations.find("skill1") == _animations.end() ||
        !_animations["skill1"] ||
        _animations["skill1"]->getFrames().empty()) {
        log("ERROR: Skill1 animation not found or empty!");

        // If no animation, directly apply skill effect and return
        applySkill1Effect();
        return;
    }

    // Get skill animation
    auto skillAnim = _animations["skill1"];
    float totalDuration = skillAnim->getDelayPerUnit() * skillAnim->getFrames().size();

    log("Skill1 animation duration: %.2f seconds", totalDuration);

    // Play skill animation
    auto animate = Animate::create(skillAnim);

    // Apply skill effect at middle frame of animation
    float effectDelay = totalDuration * 0.5f;

    // Skill effect callback
    auto applyEffect = CallFunc::create([this]() {
        applySkill1Effect();
        });

    // Skill completion callback
    auto skillComplete = CallFunc::create([this]() {
        if (!this || !this->getParent()) {
            log("Skill1 complete: Player node no longer exists");
            return;
        }

        log("Skill1 animation complete, returning to appropriate state");

        // Restore state
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

    // Create animation sequence
    auto sequence = Sequence::create(
        animate,
        skillComplete,
        nullptr
    );

    // Create effect sequence
    auto effectSequence = Sequence::create(
        DelayTime::create(effectDelay),
        applyEffect,
        nullptr
    );

    // Run action
    this->runAction(sequence);
    this->runAction(effectSequence);

    // Set cooldown time
    _skill1CooldownTimer = _skill1Cooldown;
}

// Skill 1 effect application function
void Player::applySkill1Effect() {
    log("Applying Skill1 effect");

    auto enemyManager = getSafeEnemyManager();
    if (!enemyManager) {
        log("ERROR: Cannot get EnemyManager instance");
        return;
    }

    auto enemies = enemyManager->getAllEnemies();
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

// Skill 2 implementation
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

// Skill 2 effect application function
void Player::applySkill2Effect() {
    log("Applying Skill2 effect");

    auto enemyManager = getSafeEnemyManager();
    if (!enemyManager) {
        log("ERROR: Cannot get EnemyManager instance");
        return;
    }

    auto enemies = enemyManager->getAllEnemies();
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

// Player.cpp - Modify update function
void Player::update(float delta) {
    // Update combo timer
    if (_comboTimer > 0) {
        _comboTimer -= delta;
        if (_comboTimer <= 0) {
            _comboCount = 0;
            _canCombo = false;
        }
    }

    // Update skill cooldown time
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
    
    // Update attack interval
    if (_currentAttackCooldown > 0) {
        _currentAttackCooldown -= delta;
        if (_currentAttackCooldown < 0) {
            _currentAttackCooldown = 0;
        }
    }

    // Update invincible state
    if (_isInvincible) {
        _invincibleTime -= delta;
        if (_invincibleTime <= 0) {
            _isInvincible = false;
            this->setVisible(true); // Restore visibility
            this->stopAllActions(); // Stop blink action
        }
    }

    // Update blocking state
    if (_currentState == PlayerState::BLOCKING) {
        _currentBlockDuration -= delta;
        if (_currentBlockDuration <= 0) {
            endBlocking();
        }
    }

    // Update dodge state
    if (_currentState == PlayerState::DODGING) {
        _currentDodgeDuration -= delta;
        if (_currentDodgeDuration <= 0) {
            endDodging();
        }
    }

    // Prevent infinite falling
    if (_worldPositionY < -1000.0f) {
        _worldPositionY = 0;
        _velocity.y = 0;
        _isGrounded = true;
        log("WARNING: Player fell too far, resetting position!");
    }

    // Update dash
    updateDash(delta);

    // Update physics (including gravity)
    updatePhysics(delta);

    // Update world position
    updateWorldPosition(delta);
}

// Player.cpp - Modify updatePhysics function
void Player::updatePhysics(float delta) {
    // Apply gravity
    _velocity.y -= _gravity * delta;

    // Update Y direction world position
    float newWorldY = _worldPositionY + _velocity.y * delta;

    // Check if landed - add stricter boundary checks
    if (newWorldY <= 0) {
        _worldPositionY = 0;
        _velocity.y = 0;
        _isGrounded = true;
        _jumpCount = 0; // Reset jump count when landing

        // If was jumping, restore previous movement state after landing
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

// Player.cpp - Modify updateWorldPosition function
void Player::updateWorldPosition(float delta) {
    // If attacking, dashing, blocking, or dodging, don't update X position
    if (_currentState == PlayerState::ATTACKING || 
        _currentState == PlayerState::DASHING ||
        _currentState == PlayerState::BLOCKING ||
        _currentState == PlayerState::DODGING) {
        return;
    }

    // Movement speed halved when jumping
    float moveSpeedMultiplier = (_currentState == PlayerState::JUMPING) ? 0.5f : 1.0f;

    // Update world position based on movement state
    if (_isMovingLeft) {
        _worldPositionX -= _moveSpeed * moveSpeedMultiplier * delta;
        _velocity.x = -_moveSpeed * moveSpeedMultiplier;

        // Prevent player from moving outside left world boundary
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

// Modify movement function, no longer set sprite position
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
// Player.cpp - Add function to force reset animation state
void Player::resetAnimationState() {
    if (_currentState == PlayerState::DASHING) {
        // If currently dashing, force end dash
        endDash();
    }
    else if (_currentState == PlayerState::IDLE) {
        // If idle, ensure idle animation is playing
        if (_currentAnimationKey != "idle") {
            _currentAnimationKey = "idle";
            this->stopAllActions();
            auto it = _animations.find("idle");
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                auto animate = Animate::create(it->second);
                this->runAction(RepeatForever::create(animate));
            }
        }
    }
}
void Player::setCurrentState(PlayerState state) {
    bool stateChanged = (_currentState != state);
    bool needUpdateAnimation = false;
    
    // Cannot switch to other animations during jump
    if (_currentState == PlayerState::JUMPING && state != PlayerState::JUMPING) {
        // Can only switch state after landing
        if (_isGrounded) {
            stateChanged = true;
        }
        else {
            return; // Maintain jump state in air
        }
    }

    // If currently dashing, allow switching to other states
    if (_currentState == PlayerState::DASHING && state != PlayerState::DASHING) {
        // First stop dash animation
        this->stopAllActions();

        // Reset dash variables
        _currentDashDistance = 0.0f;
        _isDashLeft = false;

        // Set new state
        _currentState = state;

        // Set animation based on new state
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

        // Play corresponding animation
        auto it = _animations.find(_currentAnimationKey);
        if (it != _animations.end() && it->second->getFrames().size() > 0) {
            // Ensure correct facing direction
            if (_currentAnimationKey == "run_left") {
                this->setFlippedX(true);
            }
            else if (_currentAnimationKey == "run_right") {
                this->setFlippedX(false);
            }

            auto animate = Animate::create(it->second);
            this->runAction(RepeatForever::create(animate));
        }

        log("Switched from DASHING to state %d", (int)state);
        return;
    }

    // Special handling if currently attacking and not switching to attack state
    if (_currentState == PlayerState::ATTACKING && state != PlayerState::ATTACKING) {
        // Stop attack animation
        this->stopAllActions();
        stateChanged = true;
    }

    // Determine animation key to play
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
        // Select attack animation based on facing direction
        if (_facingRight) {
            targetAnimationKey = "normal_attack_right";
        }
        else {
            // If no left attack animation yet, temporarily use right attack animation flipped
            targetAnimationKey = "normal_attack_right";
        }
        needUpdateAnimation = true; // Attack animation always needs updating
    }
    else if (state == PlayerState::JUMPING) {
        targetAnimationKey = "jump";
        needUpdateAnimation = true;
    }
    else if (state == PlayerState::DASHING) {
        // Dash state already handled in dash method, no need to repeat here
        return;
    }
    else {
        needUpdateAnimation = stateChanged;
    }

    if (needUpdateAnimation) {
        _currentState = state;

        // Stop all current actions
        this->stopAllActions();

        // Play animation based on state
        switch (_currentState) {
        case PlayerState::IDLE:
        {
            // Play idle animation
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
            // Play running animation
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
            // Play attack animation - no loop, play once only
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
                // Stop all current actions
                this->stopAllActions();

                // Create attack animation - don't restore original frame, start immediately
                auto animate = Animate::create(it->second);
                float frameDelay = it->second->getDelayPerUnit();
                int totalFrames = (int)it->second->getFrames().size();
                float animationDuration = frameDelay * totalFrames;
                
                // Perform damage detection at middle frame (about 3/4) of attack animation
                int damageFrame = std::max(1, std::min(totalFrames - 2, totalFrames * 3 / 4));
                float damageDelay = frameDelay * damageFrame;
                
                // Create damage detection callback
                auto damageCallback = CallFunc::create([this]() {
                    if (_currentState == PlayerState::ATTACKING) {
                        // Detect enemies in attack range and deal damage
                        detectAndDamageEnemies();
                        // Detect items in attack range and process
                        detectAndDamageItems();
                    }
                });

                // Callback to ensure state reset after attack animation completes
                auto attackCompleteCallback = CallFunc::create([this]() {
                    // Ensure attack state is reset to prevent player from getting stuck
                    if (_currentState == PlayerState::ATTACKING) {
                        // Immediately reset to IDLE state
                        _currentState = PlayerState::IDLE;
                        
                        // Play idle animation
                        if (_animations.count("idle") > 0) {
                            this->stopAllActions();
                            auto idleAnim = _animations["idle"];
                            auto idleAction = RepeatForever::create(Animate::create(idleAnim));
                            this->runAction(idleAction);
                            _currentAnimationKey = "idle";
                        }
                    }
                });

                // Callback for combo processing after attack completes
                auto comboCallback = CallFunc::create([this]() {
                    // Allow combo
                    _canCombo = true;

                    // If still in combo time window, wait for next hit
                    if (_comboTimer > 0 && _comboCount < 3) {
                        // Maintain attack posture for a short time
                        auto delay = DelayTime::create(0.1f);
                        auto checkReset = CallFunc::create([this]() {
                            if (_currentState == PlayerState::ATTACKING) {
                                _currentState = PlayerState::IDLE;
                                
                                // Play idle animation
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
                        // Directly reset to IDLE state
                        _currentState = PlayerState::IDLE;
                        
                        // Play idle animation
                        if (_animations.count("idle") > 0) {
                            this->stopAllActions();
                            auto idleAnim = _animations["idle"];
                            auto idleAction = RepeatForever::create(Animate::create(idleAnim));
                            this->runAction(idleAction);
                            _currentAnimationKey = "idle";
                        }
                    }
                });

                // Create main sequence: attack animation -> attack complete callback -> combo processing
                auto mainSequence = Sequence::create(
                    animate,
                    attackCompleteCallback,
                    comboCallback,
                    nullptr);
                this->runAction(mainSequence);
                
                // Run damage detection sequence separately
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
            // Play jump animation
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
                auto audioManager = AudioManager::getInstance();
                if (audioManager) {
                    audioManager->playEffect("sfx_jump");// Added
                }
                log("Playing jump animation with %d frames", (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Jump animation not found or empty");
            }
        }
        break;
        
        // Player.cpp - Modify skill state handling in setCurrentState function
        case PlayerState::SKILL1:
        case PlayerState::SKILL2:
        {
            // Skill states already handle animations in skill1() and skill2() functions
            // Only set state here, don't play animation to avoid duplicate playback
            log("Setting skill state %d", (int)state);

            // Stop all current actions (but keep skill actions)
            // Note: Don't call stopAllActions() here to avoid interrupting playing skill animations
            _currentState = state;

            // Set animation key based on skill type
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
            
            // Stop all actions
            this->stopAllActions();
            
            // Play death narration audio
            auto audioManager = AudioManager::getInstance();
            if (audioManager) {
                audioManager->playVoice("narrator_die");
            }
            
            // Set death state
            _currentState = PlayerState::DEAD;
            _currentAnimationKey = "dead";
            
            // Disable and remove physics collision body
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->setEnabled(false);
                this->removeComponent(this->getPhysicsBody());
            }
            
            // Clean up animation resources to avoid memory leaks
            for (auto& pair : _animations) {
                CC_SAFE_RELEASE(pair.second);
            }
            _animations.clear();
            
            // Reset state variables
            _isMovingLeft = false;
            _isMovingRight = false;
            _velocity = Vec2::ZERO;
            _isGrounded = false;
            
            log("Player DEAD state processed: animations cleared, physics disabled");
        }
        break;
        case PlayerState::HURT:
        {
            // Play hurt animation
            _currentAnimationKey = "hurt";
            auto it = _animations.find("hurt");
            if (it != _animations.end() && it->second->getFrames().size() > 0) {
                // Stop all current actions
                this->stopAllActions();

                // Set invincible state
                _isInvincible = true;
                _invincibleTime = 1.0f; // 1 second invincible time

                // Create blink action
                auto blinkAction = Blink::create(1.0f, 10); // Blink 10 times in 1 second
                this->runAction(blinkAction);

                // Play hurt animation (no loop)
                auto animate = Animate::create(it->second);
                auto callback = CallFunc::create([this]() {
                    // After hurt animation ends, return to appropriate state based on situation
                    if (_currentHealth <= 0) {
                        _currentState = PlayerState::DEAD;
                        setCurrentState(PlayerState::DEAD);
                    }
                    else if (_isGrounded) {
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

                auto sequence = Sequence::create(animate, callback, nullptr);
                this->runAction(sequence);

                log("Playing hurt animation with %d frames", (int)it->second->getFrames().size());
            }
            else {
                log("ERROR: Hurt animation not found or empty");
                // If no hurt animation, directly set back to IDLE state
                setCurrentState(PlayerState::IDLE);
            }
        }
        break;
        default:
            break;
        }
    }
}
// Player.cpp - Modify canDash method
bool Player::canDash() const {
    // Use DashBar to manage dash count
    if (_dashBar) {
        return _dashBar->canDash();
    }

    // If no DashBar, use original logic (backward compatible)
    return (_dashCooldown <= 0.0f && _currentState != PlayerState::ATTACKING);
}

// Skill 1 availability check
bool Player::canUseSkill1() const {
    // Skill 1 available condition: not on cooldown, not attacking, dashing, hurt, dead, or other skill states
    log("canUseSkill1: cooldown=%.2f, state=%d, grounded=%d", _skill1CooldownTimer, (int)_currentState, _isGrounded);
    return _skill1CooldownTimer <= 0.0f && 
           _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

// Skill 2 availability check
bool Player::canUseSkill2() const {
    // Skill 2 available condition: not on cooldown, not attacking, dashing, hurt, dead, or other skill states
    log("canUseSkill2: cooldown=%.2f, state=%d, grounded=%d", _skill2CooldownTimer, (int)_currentState, _isGrounded);
    return _skill2CooldownTimer <= 0.0f && 
           _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

// Player.cpp - Add dash method implementation
void Player::dash() {
    // Default dash right
    dashRight();
}
// Player.cpp - Simplify dashLeft and dashRight functions
void Player::dashLeft() {
    if (!canDash() || _currentState == PlayerState::ATTACKING) {
        log("Cannot dash: canDash=%d, state=%d", canDash(), (int)_currentState);
        return;
    }

    if (_dashBar && !_dashBar->useDash()) {
        log("Failed to use dash from DashBar");
        return;
    }

    // Set dash state
    _currentState = PlayerState::DASHING;
    _facingRight = false;
    _isDashLeft = true;
    _currentDashDistance = 0.0f;

    // Stop all current actions
    this->stopAllActions();

    // Play dash animation
    std::string dashKey = "dash_right"; // Use right dash animation
    auto it = _animations.find(dashKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        // Horizontally flip animation when dashing left
        this->setFlippedX(true);

        // Play dash animation (using Repeat::create so animation stops after ending instead of looping)
        auto animate = Animate::create(it->second);
        this->runAction(Repeat::create(animate, 1));

        log("Playing dash left animation, remaining dashes: %d",
            _dashBar ? _dashBar->getAvailableDashes() : 0);
    }
    else {
        log("ERROR: Dash animation not found");
        // Maintain dash state even without animation
    }
}

void Player::dashRight() {
    if (!canDash() || _currentState == PlayerState::ATTACKING) {
        log("Cannot dash: canDash=%d, state=%d", canDash(), (int)_currentState);
        return;
    }

    if (_dashBar && !_dashBar->useDash()) {
        log("Failed to use dash from DashBar");
        return;
    }

    // Set dash state
    _currentState = PlayerState::DASHING;
    _facingRight = true;
    _isDashLeft = false;
    _currentDashDistance = 0.0f;

    // Stop all current actions
    this->stopAllActions();

    // Play dash animation
    std::string dashKey = "dash_right";
    auto it = _animations.find(dashKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        // Don't flip when dashing right
        this->setFlippedX(false);

        // Play dash animation (using Repeat::create so animation stops after ending instead of looping)
        auto animate = Animate::create(it->second);
        this->runAction(Repeat::create(animate, 1));

        log("Playing dash right animation, remaining dashes: %d",
            _dashBar ? _dashBar->getAvailableDashes() : 0);
    }
    else {
        log("ERROR: Dash animation not found");
        // Maintain dash state even without animation
    }
}
// Add dash completion handling function
void Player::onDashComplete() {
    log("Player::onDashComplete called");

    // Stop all actions
    this->stopAllActions();

    // Reset dash variables
    _currentDashDistance = 0.0f;
    _isDashLeft = false;

    // Set appropriate state based on current input state
    if (_isMovingLeft || _isMovingRight) {
        // If movement input, switch to running state
        setCurrentState(PlayerState::RUNNING);
    }
    else {
        // No movement input, switch to idle state
        setCurrentState(PlayerState::IDLE);
    }
}
// Player.cpp - Modify updateDash function
void Player::updateDash(float delta) {
    if (_currentState == PlayerState::DASHING) {
        // Calculate dash movement
        float dashMove = _dashSpeed * delta;
        _currentDashDistance += dashMove;

        // Move based on dash direction
        if (_isDashLeft) {
            _worldPositionX -= dashMove;
            // Prevent moving outside left world boundary
            if (_worldPositionX < 0) {
                _worldPositionX = 0;
            }
        }
        else {
            _worldPositionX += dashMove;
        }

        // Check if reached max dash distance OR animation has ended
        if (_currentDashDistance >= _dashDistance) {
            // Dash distance reached, end dash
            endDash();
        }
        else {
            // Check if animation has naturally ended (by checking if there are running actions)
            if (this->getNumberOfRunningActions() == 0) {
                // If no running actions, animation has ended, force end dash
                log("No running actions detected, forcing dash end");
                endDash();
            }
        }
    }
}
// Modify endDash function
void Player::endDash() {
    if (_currentState != PlayerState::DASHING) {
        return;
    }

    log("Dash ended, resetting state");

    // Stop all actions
    this->stopAllActions();

    // Reset dash variables
    _currentDashDistance = 0.0f;
    _isDashLeft = false;

    // Force set back to idle state
    _currentState = PlayerState::IDLE;

    // Play idle animation
    auto it = _animations.find("idle");
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        // Ensure correct facing direction
        if (_facingRight) {
            this->setFlippedX(false);
        }
        else {
            this->setFlippedX(true);
        }

        // Play idle animation
        auto animate = Animate::create(it->second);
        this->runAction(RepeatForever::create(animate));

        log("Player returned to idle state after dash");
    }
    else {
        log("ERROR: Idle animation not found after dash");
    }
}
// Player.cpp - Modify dash section in onKeyPressed function
void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode) {
    // Record key press state
    _keyStates[keyCode] = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = true;
        _facingRight = false;

        // If not attacking or dashing, can move
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING) {
            // If already jumping, maintain jump state
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = true;
        _facingRight = true;

        // If not attacking or dashing, can move
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING) {
            // If already jumping, maintain jump state
            if (_currentState != PlayerState::JUMPING) {
                _currentState = PlayerState::RUNNING;
                setCurrentState(PlayerState::RUNNING);
            }
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_K) {
        // K key: normal attack
        // Can only trigger attack when not attacking, on ground, and attack interval has ended
        if (_currentState != PlayerState::ATTACKING && _currentState != PlayerState::DASHING && _isGrounded && _currentAttackCooldown <= 0) {
            _currentState = PlayerState::ATTACKING;
            setCurrentState(PlayerState::ATTACKING);
            auto audioManager = AudioManager::getInstance();
            if (audioManager) {
                audioManager->playEffect("sfx_attack");// Added sound effect
            }
            // Stop movement when attacking
            _isMovingLeft = false;
            _isMovingRight = false;
            _velocity.x = 0;
            
            // Reset attack interval
            _currentAttackCooldown = _attackCooldown;

            log("Player performs normal attack");
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        // W key: jump - can be used simultaneously with movement
        jump();
    }
    // Player.cpp - Modify dash section in onKeyPressed function
    else if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
        // Q key: skill 1
        skill1();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_E) {
        // E key: skill 2
        skill2();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        // Space key: dash
        if (canDash()) {
            auto audioManager = AudioManager::getInstance();
            if (audioManager) {
                audioManager->playEffect("sfx_dash");// Added
            }
            // Use key state table to determine direction
            bool aPressed = (_keyStates.find(EventKeyboard::KeyCode::KEY_A) != _keyStates.end() &&
                _keyStates[EventKeyboard::KeyCode::KEY_A]);
            bool dPressed = (_keyStates.find(EventKeyboard::KeyCode::KEY_D) != _keyStates.end() &&
                _keyStates[EventKeyboard::KeyCode::KEY_D]);

            // Clear direction priority
            if (aPressed && !dPressed) {
                // A key pressed: dash left
                dashLeft();
                log("Player dashes left (A pressed)");
            }
            else if (dPressed && !aPressed) {
                // D key pressed: dash right
                dashRight();
                log("Player dashes right (D pressed)");
            }
            else if (aPressed && dPressed) {
                // A and D keys pressed simultaneously: dash based on current facing direction
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
                // Only space pressed: dash based on current facing direction
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

    // Handle simultaneous press cases
    if (_currentState == PlayerState::JUMPING && (_isMovingLeft || _isMovingRight)) {
        // Maintain jump state, but allow horizontal movement
        // This is already handled in updateWorldPosition
    }
}

// Player.cpp - Modify onKeyReleased function
void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode) {
    // Update key state
    _keyStates[keyCode] = false;
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _isMovingLeft = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _isMovingRight = false;
    }

    // If both movement keys are released
    if (!_isMovingLeft && !_isMovingRight) {
        // Maintain jump state while jumping
        if (_currentState != PlayerState::JUMPING && _currentState != PlayerState::ATTACKING) {
            _currentState = PlayerState::IDLE;
            setCurrentState(PlayerState::IDLE);
        }
    }
    else {
        // If one movement key is pressed and not jumping or attacking
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
    // This function is temporarily empty, animations are loaded in loadAnimations
}

// Player.cpp - Add toggleInvincibleMode function implementation
void Player::toggleInvincibleMode() {
    _isInvincible = !_isInvincible;
    log("Invincible mode %s", _isInvincible ? "enabled" : "disabled");
}

// Player.cpp - Modify takeDamage function
void Player::takeDamage(float damage) {
    if (isDead() || _isInvincible) {
        return; // If already dead or invincible, don't process damage
    }

    // Check if in blocking state
    if (_isBlocking) {
        // Block reduces 70% damage
        float reducedDamage = damage * (1.0f - _blockReduction);
        log("Player blocked attack, reduced damage from %.2f to %.2f", damage, reducedDamage);
        damage = reducedDamage;
    }

    // First consume shield
    if (_currentShield > 0) {
        _currentShield -= 1;
        log("Player shield reduced by 1. Shield: %d", _currentShield);

        // Update HUD display
        if (_mainGameScene && _mainGameScene->getHudLayer()) {
            _mainGameScene->getHudLayer()->updateSheld(_currentShield);
        }

        // Play shield hit effect
        _currentState = PlayerState::HURT;
        setCurrentState(PlayerState::HURT);

        // Play shield hit sound effect
        auto audioManager = AudioManager::getInstance();
        if (audioManager) {
            audioManager->playEffect("sfx_shield_hit");
            // Play hit sound effect
            audioManager->playEffect("sfx_hurt");
        }

    }
    else {
        // After shield is exhausted, consume health
        _currentHealth -= damage;

        // Ensure health doesn't go below 0
        if (_currentHealth < 0) {
            _currentHealth = 0;
        }

        // Update HUD display
        if (_mainGameScene && _mainGameScene->getHudLayer()) {
            _mainGameScene->getHudLayer()->updateHealth(_currentHealth);
        }

        // Set hurt state
        _currentState = PlayerState::HURT;
        setCurrentState(PlayerState::HURT);

        // Play hurt sound effect
        auto audioManager = AudioManager::getInstance();
        if (audioManager) {
            audioManager->playEffect("sfx_hurt");
        }

        // If health is 0, set death state
        if (_currentHealth == 0) {
            log("Player died! Health: %.0f", _currentHealth);
            // Death state automatically triggered after hurt animation ends
        }
        else {
            log("Player took %.0f damage. Health: %.0f", damage, _currentHealth);
        }
    }
}

// Player.cpp - Implement precise attack detection system
void Player::detectAndDamageEnemies() {
    auto enemyManager = EnemyManager::getInstance();
    if (!enemyManager) {
        log("No EnemyManager found for attack detection");
        return;
    }

    auto enemies = enemyManager->getAllEnemies();
    if (enemies.empty()) {
        log("No enemies found for attack detection");
        return;
    }

    // Adjust damage based on combo count
    float baseDamage = 30.0f;
    float comboMultiplier = 1.0f + (_comboCount * 0.2f); // Each combo increases damage by 20%
    float damage = baseDamage * comboMultiplier;
    
    // Set attack range and direction based on player facing
    float attackRangeX = 80.0f;  // Horizontal attack range
    float attackRangeY = 40.0f;  // Vertical attack range
    float playerX = this->getWorldPositionX();
    float playerY = this->getWorldPositionY();

    for (Enemy* enemy : enemies) {
        if (enemy && !enemy->isDead()) {
            float enemyX = enemy->getWorldPositionX();
            float enemyY = enemy->getWorldPositionY();
            
            // 1. Check vertical range (Y axis)
            float yDistance = std::abs(enemyY - playerY);
            if (yDistance > attackRangeY) {
                continue;  // Not in vertical attack range
            }
            
            // 2. Check horizontal range (X axis) based on player facing
            if (_facingRight) {
                // Attacking right, only detect enemies to the right of the player
                if (enemyX < playerX || (enemyX - playerX) > attackRangeX) {
                    continue;
                }
            } else {
                // Attacking left, only detect enemies to the left of the player
                if (enemyX > playerX || (playerX - enemyX) > attackRangeX) {
                    continue;
                }
            }
            
            // 3. Precise attack collision detection (considering enemy's actual size)
            float playerHalfWidth = this->getContentSize().width / 2;
            float enemyHalfWidth = enemy->getContentSize().width / 2;
            float actualDistanceX = std::abs(enemyX - playerX) - playerHalfWidth - enemyHalfWidth;
            
            if (actualDistanceX <= 0) {
                // Attack hits enemy
                if (enemy->getEnemyType() == "hidden_level_trigger") {
                    // Special handling: trigger hidden level
                    log("Player attacked hidden level trigger! Switching to hidden level...");
                    
                    // Get level manager and switch to hidden level
                    auto levelManager = LevelManager::getInstance();
                    if (levelManager) {
                        levelManager->switchToHiddenLevel1();
                    }
                    
                    // Increase combo count
                    _comboCount++;
                    _comboTimer = COMBO_WINDOW;  // Reset combo time window
                } else {
                    // Normal enemy: deal damage
                    enemy->takeDamage(damage);
                    log("Player attacks enemy! Damage: %.0f (combo: x%.1f), Enemy health: %.0f",
                        damage, comboMultiplier, enemy->getCurrentHealth());
                    
                    // Increase combo count
                    _comboCount++;
                    _comboTimer = COMBO_WINDOW;  // Reset combo time window
                }
            }
        }
    }
}

void Player::detectAndDamageItems() {
    // Get current scene
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    if (!scene) {
        log("No scene found for item attack detection");
        return;
    }
    
    // Convert to MainGameScene
    auto gameScene = dynamic_cast<MainGameScene*>(scene);
    if (!gameScene) {
        log("Scene is not MainGameScene for item attack detection");
        return;
    }
    
    // Get all items
    auto items = gameScene->getAllItems();
    if (items.empty()) {
        log("No items found for attack detection");
        return;
    }
    
    // Set attack range and direction based on player facing
    float attackRangeX = 80.0f;  // Horizontal attack range
    float attackRangeY = 40.0f;  // Vertical attack range
    float playerX = this->getWorldPositionX();
    float playerY = this->getWorldPositionY();
    
    // Get level manager
    auto levelManager = LevelManager::getInstance();
    if (!levelManager) {
        log("No LevelManager found for level switching");
        return;
    }
    
    for (Item* item : items) {
        if (item) {
            float itemX = item->getWorldPositionX();
            float itemY = item->getWorldPositionY();
            
            // 1. Check vertical range (Y axis)
            float yDistance = std::abs(itemY - playerY);
            if (yDistance > attackRangeY) {
                continue;  // Not in vertical attack range
            }
            
            // 2. Check horizontal range (X axis) based on player facing
            if (_facingRight) {
                // Attacking right, only detect items to the right of the player
                if (itemX < playerX || (itemX - playerX) > attackRangeX) {
                    continue;
                }
            } else {
                // Attacking left, only detect items to the left of the player
                if (itemX > playerX || (playerX - itemX) > attackRangeX) {
                    continue;
                }
            }
            
            // 3. Precise attack collision detection (considering actual item size)
            float playerHalfWidth = this->getContentSize().width / 2;
            float itemHalfWidth = item->getContentSize().width / 2;
            float actualDistanceX = std::abs(itemX - playerX) - playerHalfWidth - itemHalfWidth;
            
            if (actualDistanceX <= 0) {
                // Attack hits item
                log("Player attacks item: %s", item->getItemId().c_str());
                
                // Check if it's the hidden level trigger item
                if (item->getItemId() == "what_the_hell") {
                    log("Player attacked what_the_hell item! Switching to hidden level...");
                    
                    // Switch to hidden level
                    levelManager->switchToHiddenLevel1();
                    
                    // Remove the item from scene
                    gameScene->removeItem(item);
                    break;
                }
            }
        }
    }
}

// Blocking related methods implementation
bool Player::canBlock() const {
    return _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

void Player::startBlocking() {
    if (!canBlock()) {
        log("Cannot block: current state is %d", (int)_currentState);
        return;
    }

    // Set blocking state
    _currentState = PlayerState::BLOCKING;
    _isBlocking = true;
    _currentBlockDuration = _blockDuration;

    // Stop all current actions
    this->stopAllActions();

    // Play blocking animation
    std::string blockKey = "block"; // Assuming blocking animation already exists
    auto it = _animations.find(blockKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        auto animate = Animate::create(it->second);
        this->runAction(RepeatForever::create(animate));
        log("Player starts blocking");
    }
    else {
        log("WARNING: Block animation not found, using default blocking state");
        // Maintain blocking state even without animation
    }
}

void Player::endBlocking() {
    if (_currentState != PlayerState::BLOCKING) {
        return;
    }

    // Reset blocking state
    _isBlocking = false;
    _currentBlockDuration = 0.0f;

    this->stopAllActions();

    // Set appropriate state based on current input
    if (_isMovingLeft || _isMovingRight) {
        setCurrentState(PlayerState::RUNNING);
    }
    else {
        setCurrentState(PlayerState::IDLE);
    }
    
    log("Player ends blocking");
}

// Dodging related methods implementation
bool Player::canDodge() const {
    return _currentState != PlayerState::ATTACKING && 
           _currentState != PlayerState::DASHING &&
           _currentState != PlayerState::HURT &&
           _currentState != PlayerState::DEAD &&
           _currentState != PlayerState::SKILL1 &&
           _currentState != PlayerState::SKILL2;
}

void Player::startDodging(float direction) {
    if (!canDodge()) {
        log("Cannot dodge: current state is %d", (int)_currentState);
        return;
    }

    // Set dodging state
    _currentState = PlayerState::DODGING;
    _isDodging = true;
    _currentDodgeDuration = _dodgeDuration;
    
    // Set invincible state
    _isInvincible = true;
    _invincibleTime = _dodgeInvincibility;

    // Stop all current actions
    this->stopAllActions();

    // Play dodging animation
    std::string dodgeKey = "dodge";
    auto it = _animations.find(dodgeKey);
    if (it != _animations.end() && it->second->getFrames().size() > 0) {
        auto animate = Animate::create(it->second);
        this->runAction(Repeat::create(animate, 1));
        log("Player starts dodging");
    }
    else {
        log("WARNING: Dodge animation not found, using default dodging state");
        // Maintain dodging state even without animation
    }
    
    // Set facing direction based on dodge direction
    if (direction < 0) {
        _facingRight = false;
    } else {
        _facingRight = true;
    }
}

void Player::endDodging() {
    if (_currentState != PlayerState::DODGING) {
        return;
    }

    // Reset dodging state
    _isDodging = false;
    _currentDodgeDuration = 0.0f;

    this->stopAllActions();

    // Set appropriate state based on current input
    if (_isMovingLeft || _isMovingRight) {
        setCurrentState(PlayerState::RUNNING);
    }
    else {
        setCurrentState(PlayerState::IDLE);
    }
    
    log("Player ends dodging");
}

Player::~Player() {
    // Release all cached animations
    for (auto& pair : _animations) {
        CC_SAFE_RELEASE(pair.second);
    }
    _animations.clear();
    
    // Clear singleton instance
    if (_instance == this) {
        _instance = nullptr;
    }
}