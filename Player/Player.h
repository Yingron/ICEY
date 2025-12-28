// Player.h
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "PlayerState.h"
#include "DashBar.h"

// 鍓嶅悜澹版槑MainGameScene绫?
class MainGameScene;
// 鍓嶅悜澹版槑EnemyManager绫?
class EnemyManager;

class Player : public cocos2d::Sprite {
public:
    static Player* create(const std::string& spriteFile);
    static Player* getInstance();
    std::vector<std::string> optimizeAttackFrames(const std::vector<std::string>& originalFrames, int targetFrameCount);
    virtual bool init(const std::string& spriteFile);
    virtual ~Player();

    void update(float delta);

    // 绉诲姩鍑芥暟
    void moveLeft(float delta);
    void moveRight(float delta);
    void stopMoving();

    // 璺宠穬鍑芥暟
    void jump();

    // 鐘舵€佽幏鍙?
    PlayerState getCurrentState() const { return _currentState; }
    void resetAnimationState();
    void setCurrentState(PlayerState state);

    // 杈撳叆澶勭悊
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode);

    // 璋冭瘯鏂规硶
    std::string getDebugInfo() const;

    // 娴嬭瘯鏂规硶
    cocos2d::Vec2 getCurrentVelocity() const { return _velocity; }

    // 涓栫晫鍧愭爣鐩稿叧
    float getWorldPositionX() const { return _worldPositionX; }
    float getWorldPositionY() const { return _worldPositionY; }
    void setWorldPositionX(float x) { _worldPositionX = x; }
    void setWorldPositionY(float y) { _worldPositionY = y; }

    // 璺宠穬鐘舵€佹煡璇?
    bool isGrounded() const { return _isGrounded; }
    bool isJumping() const { return _currentState == PlayerState::JUMPING; }
    
    // 绉诲姩鐩稿叧鏌ヨ
    float getMoveSpeed() const { return _moveSpeed; }
    bool isFacingRight() const { return _facingRight; }
    // 娣诲姞DashBar寮曠敤
    DashBar* _dashBar;

    // 淇敼鍐插埡鐩稿叧鏂规硶澹版槑
    bool canDash() const;  // 鏀逛负鍏湁鏂规硶浠ヤ究璁块棶
    void setDashBar(DashBar* dashBar) { _dashBar = dashBar; }
    // 鍐插埡鐩稿叧鏂规硶
    void dashLeft();
    void dashRight();
    void dash();  // 榛樿鍚戝彸鍐插埡
    bool isDashing() const { return _currentState == PlayerState::DASHING; }
    
    // 鏍兼尅鐩稿叧鏂规硶
    void startBlocking();
    void endBlocking();
    bool canBlock() const;
    bool isBlocking() const { return _currentState == PlayerState::BLOCKING; }
    
    // 闂伩鐩稿叧鏂规硶
    void startDodging(float direction);
    void endDodging();
    bool canDodge() const;
    bool isDodging() const { return _currentState == PlayerState::DODGING; }
    
    // 鐢熷懡鍊肩浉鍏虫柟娉曞拰灞炴€?
    float getMaxHealth() const { return _maxHealth; }
    float getCurrentHealth() const { return _currentHealth; }
    
    // 鎶ょ浘鐩稿叧鏂规硶鍜屽睘鎬?
    int getMaxShield() const { return _maxShield; }
    int getCurrentShield() const { return _currentShield; }
    
    void takeDamage(float damage);
    bool isDead() const { return _currentHealth <= 0; }
    
    // 鏀诲嚮鐩稿叧鏂规硶
    void detectAndDamageEnemies();
    void detectAndDamageItems();
    bool isAttacking() const { return _currentState == PlayerState::ATTACKING; }
    
    // 鎶€鑳界浉鍏虫柟娉?
    void skill1();
    void skill2();
    bool canUseSkill1() const;
    bool canUseSkill2() const;

    // 娣诲姞鎶€鑳芥晥鏋滃簲鐢ㄥ嚱鏁扮殑澹版槑
    void applySkill1Effect();
    void applySkill2Effect();
    
    // 娓告垙缁撴潫鐩稿叧鏂规硶
    void setMainGameScene(MainGameScene* scene) { _mainGameScene = scene; }
    MainGameScene* getMainGameScene() const { return _mainGameScene; }
    
    // 鏃犳晫妯″紡鐩稿叧鏂规硶
    // 鏃犳晫鐘舵€佺浉鍏?
    bool _invincibleModeEnabled = false;      // 鏃犳晫妯″紡寮€鍏?
    void toggleInvincibleMode();
    float getDisplayHealth() const; // 淇鍑芥暟澹版槑璇硶閿欒
    bool isInvincibleModeEnabled() const; // 淇鍑芥暟澹版槑璇硶閿欒
    cocos2d::ParticleSystem* _invincibleParticle = nullptr; // 鏃犳晫鐘舵€佺矑瀛愭晥鏋?
    float _originalMaxHealth = 100.0f;  // 瀛樺偍鍘熷鏈€澶х敓鍛藉€?
    float _originalCurrentHealth = 100.0f; // 瀛樺偍鍘熷褰撳墠鐢熷懡鍊?
    float _displayHealth = 100.0f;      // 鏄剧ず鐢ㄧ殑鐢熷懡鍊?
    void setInvincibleModeVisualFeedback(bool enabled); // 璁剧疆鏃犳晫妯″紡瑙嗚鍙嶉
    
    // 娴嬭瘯鏃犳晫妯″紡鍔熻兘锛堝彲閫夛紝鐢ㄤ簬璋冭瘯锛?
    float getHealthPercentage() const;
    void testInvincibleMode(); // 娴嬭瘯鏂规硶锛屽紑鍙戦樁娈典娇鐢?

private:
    static Player* _instance;
    PlayerState _currentState;
    cocos2d::Vec2 _velocity;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _facingRight;
    float _moveSpeed;
    std::string _currentAnimationKey;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, bool> _keyStates;
    // 涓栫晫鍧愭爣
    float _worldPositionX;
    float _worldPositionY;

    // 璺宠穬鐩稿叧鍙橀噺
    bool _isGrounded;
    float _jumpForce;
    float _gravity;
    int _jumpCount;           // 璺宠穬娆℃暟璁℃暟
    int _maxJumpCount;        // 鏈€澶ц烦璺冩鏁帮紙2琛ㄧず浜岃繛璺筹級
    float _lastJumpTime;      // 涓婃璺宠穬鏃堕棿锛岀敤浜庢娴嬭繛璺?

    // 杩炲嚮鐩稿叧鍙橀噺
    bool _canCombo;
    int _comboCount;
    float _comboTimer;
    const float COMBO_WINDOW = 1.0f; // 杩炲嚮鏃堕棿绐楀彛锛?绉掞級
    
    // 鏀诲嚮闂撮殧鐩稿叧鍙橀噺
    float _attackCooldown;
    float _currentAttackCooldown;
    
    // 鐢熷懡鍊肩浉鍏冲彉閲?
    float _maxHealth;
    float _currentHealth;
    
    // 鎶ょ浘鐩稿叧鍙橀噺
    int _maxShield;
    int _currentShield;

    // 鍔ㄧ敾绠＄悊
    void setupAnimations();
    void loadAnimations();
    cocos2d::Animation* createAnimationFromFiles(const std::vector<std::string>& frames, float delay);

    // 鐗╃悊鏇存柊
    void updatePhysics(float delta);
    void updateWorldPosition(float delta);

    std::unordered_map<std::string, cocos2d::Animation*> _animations;
    // 鍐插埡鐩稿叧鍙橀噺
    bool _canDash;               // 鏄惁鍙互鍐插埡
    float _dashSpeed;            // 鍐插埡閫熷害
    float _dashDistance;         // 鍐插埡璺濈
    float _dashDuration;         // 鍐插埡鎸佺画鏃堕棿
    float _dashCooldown;         // 鍐插埡鍐峰嵈鏃堕棿
    float _currentDashDistance;  // 褰撳墠宸插啿鍒鸿窛绂?
    bool _isDashLeft;            // 鏄惁鍚戝乏鍐插埡
    
    // 鏍兼尅鐩稿叧鍙橀噺
    bool _isBlocking;            // 鏄惁鍦ㄦ牸鎸＄姸鎬?
    float _blockDuration;        // 鏍兼尅鎸佺画鏃堕棿
    float _blockCooldown;        // 鏍兼尅鍐峰嵈鏃堕棿
    float _blockReduction;       // 鏍兼尅鍑忎激鐧惧垎姣?
    float _currentBlockDuration; // 褰撳墠鏍兼尅鎸佺画鏃堕棿
    
    // 闂伩鐩稿叧鍙橀噺
    bool _isDodging;             // 鏄惁鍦ㄩ棯閬跨姸鎬?
    float _dodgeDuration;        // 闂伩鎸佺画鏃堕棿
    float _dodgeCooldown;        // 闂伩鍐峰嵈鏃堕棿
    float _dodgeInvincibility;   // 闂伩鏃犳晫鏃堕棿
    float _currentDodgeDuration; // 褰撳墠闂伩鎸佺画鏃堕棿

    // 鍐插埡鍔ㄧ敾閿?
    std::string _dashLeftAnimKey;
    std::string _dashRightAnimKey;

    // 鍐插埡鏇存柊鏂规硶
    void updateDash(float delta);
    // 鍐插埡瀹屾垚澶勭悊鍑芥暟
    void onDashComplete();
    void endDash();
    
    // 鎶€鑳界浉鍏冲彉閲?
    float _skill1Damage;         // 鎶€鑳?浼ゅ
    float _skill1Range;          // 鎶€鑳?鑼冨洿
    float _skill1Cooldown;       // 鎶€鑳?鍐峰嵈鏃堕棿
    float _skill1CooldownTimer;  // 鎶€鑳?鍐峰嵈璁℃椂鍣?
    
    float _skill2Damage;         // 鎶€鑳?浼ゅ
    float _skill2Range;          // 鎶€鑳?鑼冨洿
    float _skill2Cooldown;       // 鎶€鑳?鍐峰嵈鏃堕棿
    float _skill2CooldownTimer;  // 鎶€鑳?鍐峰嵈璁℃椂鍣?
    
    // 鎶€鑳藉姩鐢婚敭
    std::string _skill1AnimKey;
    std::string _skill2AnimKey;
    
    // 鎸囧悜MainGameScene鐨勬寚閽堬紝鐢ㄤ簬瑙﹀彂娓告垙缁撴潫
    MainGameScene* _mainGameScene;
    // 鎶€鑳借緟鍔╂柟娉?
    EnemyManager* getSafeEnemyManager();

    // 鏃犳晫鐩稿叧鍙橀噺
    float _invincibleTime;    // 鏃犳晫鏃堕棿
    bool _isInvincible;       // 鏃犳晫鐘舵€?
    int _hurtFlashCount;      // 鍙椾激闂儊璁℃暟

};
#endif // PLAYER_H