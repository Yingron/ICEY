// MainGameScene.h
#pragma once
#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "cocos2d.h"
#include "Level1SceneBackground.h"
#include "LevelManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "HudLayer.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "ItemBagLayer.h"
#include"AchievementSystem.h"

// Forward declaration
class Player;

class MainGameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    CREATE_FUNC(MainGameScene);
    void initItemBag();

    // ============ Added: Achievement-related methods ============//12/28/15.19
    void checkFirstLaunch();                     // Check first launch
    void onPlayerDeath();                        // Player death
    void onJumpToTopBoundary();                  // Reach top boundary
    void onAttackAction();                       // Attack action
    void onCollectItem();                        // Collect item
    void onGameCompleted();                      // Game completed
    void onFinalChoice(bool choseReality);       // Final choice
    void onAnnoyNarrator();                      // Annoy narrator
    void onConfrontNarrator(bool choseReality);  // Confront narrator

public:
    void showGameOver();
    HudLayer* getHudLayer() const { return _hudLayer; }
    int getEnemyCount() const;
    float getCameraOffsetX() const; // Get camera offset X
    void removeItem(Item* item);
    std::vector<Item*> getAllItems() const { return _levelItems; }

private:
    // ============ Added: Achievement-related variables ============//12/28/15.19
    int _deathCount = 0;                // Death count tracking
    int _airAttackCount = 0;            // Air attack count tracking
    int _annoyNarratorCount = 0;        // Annoy narrator count tracking




    ItemBagLayer* _itemBagLayer;
    // Enemy-related methods
    void initEnemies();
    void updateEnemies(float delta);
    void checkEnemyCollisions(float delta);
    void createEnemyAt(const std::string& enemyTypeStr, float worldX, float worldY);

    // Added enemy configuration variables
    const int MIN_ENEMY_COUNT = 2;  // Minimum number of enemies
    const int MAX_ENEMY_COUNT = 5;  // Maximum number of enemies
    std::vector<Enemy*> _enemiesList; // Stores all enemies in current map
    Player* _player;
    SceneBackground* _currentBackground;
    EnemyManager* _enemyManager;

    HudLayer* _hudLayer;  // HUD layer pointer hy*****

    // Level management
    LevelManager* _levelManager;

    bool _isGameOver;//hy*****
    void checkPlayerHealth();

    // Camera related
    float _cameraOffsetX;
    float _screenWidth;
    float _worldWidth;
    float _minCameraX;
    float _maxCameraX;

    // Level transition
    bool _isTransitioning;
    float _transitionTimer;
    const float TRANSITION_DURATION = 1.0f;

    // Item system
    std::vector<Item*> _levelItems;
    cocos2d::Label* _collectionLabel;

    // Initialization methods
    void initBackground();
    void initPlayer();
    void initInput();
    void initDebugUI();
    void initCamera();
    void initItems();
    void initHud();//hy*****
    void showCollectionUI();

    // Update methods
    void updateCamera(float delta);
    void checkLevelTransition(float delta);
    void switchToNextLevel();
    void checkItemCollisions(float delta);
    void updateCollectionUI();

    // Helper methods
    bool isPlayerAtRightBoundary();
    void placeItemAt(const std::string& itemId, float worldX, float worldY);

    // Keyboard related
    cocos2d::EventListenerKeyboard* _keyboardListener;
    cocos2d::Label* _debugLabel;
    cocos2d::Label* _levelLabel;
    
    // Physics related
    cocos2d::EventListenerPhysicsContact* _contactListener;

    // Keyboard event callbacks
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Collision event callback
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    // Scene lifecycle methods
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void removeEnemy(Enemy* enemy);
    void cleanupDeadEnemies();
};

#endif // MAINGAMESCENE_H