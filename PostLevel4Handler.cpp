#include "PostLevel4Handler.h"
#include "cocos2d.h"
#include "MainGameScene.h"

USING_NS_CC;

PostLevel4Handler::PostLevel4Handler() : 
    _countdownTimer(30.0f),
    _isProcessing(false),
    _countdownLabel(nullptr),
    _promptLabel(nullptr),
    _overlay(nullptr)
{
    CCLOG("PostLevel4Handler constructor called");
}

PostLevel4Handler::~PostLevel4Handler() {
    CCLOG("PostLevel4Handler destructor called");
    cleanupResources();
}

bool PostLevel4Handler::init() {
    if (!Node::init()) {
        CCLOG("PostLevel4Handler init failed");
        return false;
    }
    
    CCLOG("PostLevel4Handler init success");
    return true;
}

PostLevel4Handler* PostLevel4Handler::create() {
    PostLevel4Handler* handler = new (std::nothrow) PostLevel4Handler();
    if (handler && handler->init()) {
        handler->autorelease();
        return handler;
    }
    CC_SAFE_DELETE(handler);
    return nullptr;
}

void PostLevel4Handler::startProcessing() {
    // ���ӷ�ֹ�ظ����õļ��
    if (_isProcessing) {
        CCLOG("PostLevel4Handler::startProcessing() - Already processing, skipping call");
        return;
    }
    
    CCLOG("PostLevel4Handler::startProcessing() called");
    
    _isProcessing = true;
    _countdownTimer = 30.0f;
    
    // Pause the game with safety check
    auto director = Director::getInstance();
    if (director) {
        pauseGame();
    } else {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::startProcessing()");
        _isProcessing = false;
        return;
    }
    
    // Create UI elements
    createUI();
    
    // Schedule update for countdown
    scheduleUpdate();
    
    // Enable keyboard input
    enableKeyboardInput();
    
    CCLOG("PostLevel4Handler processing started with %f seconds countdown", _countdownTimer);
}

void PostLevel4Handler::stopProcessing() {
    // ���ӷ�ֹ�ظ����õļ��
    if (!_isProcessing) {
        CCLOG("PostLevel4Handler::stopProcessing() - Not processing, skipping call");
        return;
    }
    
    CCLOG("PostLevel4Handler::stopProcessing() called");
    
    _isProcessing = false;
    
    // Unpause the game with safety check
    auto director = Director::getInstance();
    if (director) {
        resumeGame();
    } else {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::stopProcessing()");
    }
    
    // Clean up resources
    cleanupResources();
    
    CCLOG("PostLevel4Handler processing stopped");
}

void PostLevel4Handler::update(float delta) {
    if (!_isProcessing) return;
    
    // ����deltaֵ��Ч�Լ��
    if (delta < 0.0f || std::isnan(delta) || std::isinf(delta)) {
        CCLOG("WARNING: Invalid delta value in PostLevel4Handler::update(): %f", delta);
        return;
    }
    
    // Update countdown timer with clamping to prevent negative values
    _countdownTimer = std::max(0.0f, _countdownTimer - delta);
    
    // Update UI
    updateUI();
    
    // Check if countdown has ended
    if (_countdownTimer <= 0.0f) {
        CCLOG("PostLevel4Handler countdown ended");
        handleTimeout();
    }
}

void PostLevel4Handler::pauseGame() {
    auto director = Director::getInstance();
    if (director) {
        director->pause();
        CCLOG("Game paused");
    } else {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::pauseGame()");
    }
}

void PostLevel4Handler::resumeGame() {
    auto director = Director::getInstance();
    if (director) {
        director->resume();
        CCLOG("Game resumed");
    } else {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::resumeGame()");
    }
}

void PostLevel4Handler::createUI() {
    auto director = Director::getInstance();
    if (!director) {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::createUI()");
        return;
    }
    
    auto visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    
    // ȷ��UI����ǰ��Դ������
    if (_overlay || _countdownLabel || _promptLabel) {
        CCLOG("WARNING: UI elements already exist when calling createUI(), cleaning up first");
        cleanupResources();
    }
    
    // Create semi-transparent black overlay with safety checks
    _overlay = LayerColor::create(Color4B(0, 0, 0, 150), visibleSize.width, visibleSize.height);
    if (_overlay) {
        _overlay->setPosition(origin);
        this->addChild(_overlay, 10);
    } else {
        CCLOG("ERROR: Failed to create overlay in PostLevel4Handler::createUI()");
    }
    
    // Create countdown label with safety checks
    _countdownLabel = Label::createWithSystemFont(
        StringUtils::format("����ʱ: %.1f ��", _countdownTimer), 
        "Arial", 
        36
    );
    if (_countdownLabel) {
        _countdownLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 50));
        _countdownLabel->setColor(Color3B::YELLOW);
        this->addChild(_countdownLabel, 11);
    } else {
        CCLOG("ERROR: Failed to create countdown label in PostLevel4Handler::createUI()");
    }
    
    // Create prompt label with safety checks
    _promptLabel = Label::createWithSystemFont(
        "��ϲ���Level 4��\n\n���������ս���չؿ���\n\n(Y: ���� | N: �˳���Ϸ)", 
        "Arial", 
        32
    );
    if (_promptLabel) {
        _promptLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 50));
        _promptLabel->setColor(Color3B::WHITE);
        _promptLabel->setAlignment(TextHAlignment::CENTER);
        _promptLabel->setDimensions(visibleSize.width - 100, 200);
        this->addChild(_promptLabel, 11);
    } else {
        CCLOG("ERROR: Failed to create prompt label in PostLevel4Handler::createUI()");
    }
    
    CCLOG("PostLevel4Handler UI created");
}

void PostLevel4Handler::updateUI() {
    if (_countdownLabel) {
        // ���Ӱ�ȫ��飬ȷ����ʱ��ֵ��Ч
        float displayTime = std::isnan(_countdownTimer) ? 30.0f : _countdownTimer;
        _countdownLabel->setString(StringUtils::format("����ʱ: %.1f ��", displayTime));
        
        // Change color as time runs out
        if (displayTime <= 10.0f) {
            _countdownLabel->setColor(Color3B::RED);
        } else if (displayTime <= 20.0f) {
            _countdownLabel->setColor(Color3B::ORANGE);
        } else {
            _countdownLabel->setColor(Color3B::YELLOW);
        }
    }
}

void PostLevel4Handler::enableKeyboardInput() {
    // ���������еļ��̼�����
    disableKeyboardInput();
    
    auto director = Director::getInstance();
    if (!director) {
        CCLOG("ERROR: Director instance is null in PostLevel4Handler::enableKeyboardInput()");
        return;
    }
    
    // Enable keyboard input
    auto listener = EventListenerKeyboard::create();
    if (listener) {
        listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
            if (!_isProcessing || !event) return;
            
            CCLOG("PostLevel4Handler received key press with code: %d", (int)keyCode);
            
            // Check for Y key
            if (keyCode == EventKeyboard::KeyCode::KEY_Y) {
                CCLOG("PostLevel4Handler detected Y key - proceeding to final level");
                this->handleUserInputY();
            }
            // Check for N key
            else if (keyCode == EventKeyboard::KeyCode::KEY_N) {
                CCLOG("PostLevel4Handler detected N key - showing game over");
                this->handleUserInputN();
            }
            else {
                CCLOG("PostLevel4Handler received unsupported key: %d", (int)keyCode);
            }
            
            // Prevent event propagation to other handlers
            event->stopPropagation();
        };
        
        _keyboardListener = listener;
        director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        CCLOG("PostLevel4Handler keyboard input enabled");
    } else {
        CCLOG("ERROR: Failed to create keyboard listener in PostLevel4Handler::enableKeyboardInput()");
    }
}

void PostLevel4Handler::disableKeyboardInput() {
    auto director = Director::getInstance();
    if (_keyboardListener && director) {
        director->getEventDispatcher()->removeEventListener(_keyboardListener);
        _keyboardListener = nullptr;
        CCLOG("PostLevel4Handler keyboard input disabled");
    }
}

void PostLevel4Handler::handleUserInputY() {
    CCLOG("PostLevel4Handler handling 'Y' input - transitioning to final level");
    
    // ����״̬���
    if (!_isProcessing) {
        CCLOG("WARNING: handleUserInputY called when not processing");
        return;
    }
    
    // Clean up and transition to final level
    cleanupResources();
    loadFinalLevel();
}

void PostLevel4Handler::handleUserInputN() {
    CCLOG("PostLevel4Handler handling 'N' input - showing game over");
    
    // ����״̬���
    if (!_isProcessing) {
        CCLOG("WARNING: handleUserInputN called when not processing");
        return;
    }
    
    // Clean up and show game over
    cleanupResources();
    showGameOver();
}

void PostLevel4Handler::handleTimeout() {
    CCLOG("PostLevel4Handler handling timeout");
    
    // ����״̬���
    if (!_isProcessing) {
        CCLOG("WARNING: handleTimeout called when not processing");
        return;
    }
    
    // By default, if time runs out, show game over
    cleanupResources();
    showGameOver();
}

void PostLevel4Handler::loadFinalLevel() {
    CCLOG("PostLevel4Handler::loadFinalLevel() called - transitioning to final level");
    
    // Resume game before scene transition
    Director::getInstance()->resume();
    
    // Create a fade transition effect
    TransitionFade* transition = TransitionFade::create(1.0f, nullptr); // Replace nullptr with actual final level scene
    
    // Use a slight delay to ensure proper cleanup before transition
    auto delay = DelayTime::create(0.1f);
    auto performTransition = CallFunc::create([transition]() {
        CCLOG("Performing scene transition to final level");
        // In a real implementation, you would replace nullptr with:
        // Director::getInstance()->replaceScene(transition);
        
        // For now, we'll just log that we would perform the transition
        CCLOG("NOTE: In a complete implementation, this would transition to FinalLevelScene");
        // Example code for actual implementation:
        // Director::getInstance()->replaceScene(TransitionFade::create(1.0f, FinalLevelScene::createScene()));
    });
    
    this->runAction(Sequence::create(delay, performTransition, nullptr));
    
    CCLOG("Final level transition prepared");
}

void PostLevel4Handler::showGameOver() {
    CCLOG("PostLevel4Handler::showGameOver() called - showing game over screen");
    
    // Resume game before scene transition
    Director::getInstance()->resume();
    
    // Create a transition effect
    TransitionFade* transition = TransitionFade::create(1.0f, nullptr); // Replace nullptr with actual game over scene
    
    // Use a slight delay to ensure proper cleanup before transition
    auto delay = DelayTime::create(0.1f);
    auto performTransition = CallFunc::create([transition]() {
        CCLOG("Performing scene transition to game over screen");
        // In a real implementation, you would replace nullptr with:
        // Director::getInstance()->replaceScene(transition);
        
        // For now, we'll just log that we would perform the transition
        CCLOG("NOTE: In a complete implementation, this would transition to GameOverScene");
        // Example code for actual implementation:
        // Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOverScene::createScene()));
    });
    
    this->runAction(Sequence::create(delay, performTransition, nullptr));
    
    CCLOG("Game over transition prepared");
}

void PostLevel4Handler::cleanupResources() {
    CCLOG("PostLevel4Handler::cleanupResources() called");
    
    // Stop update schedule
    unscheduleUpdate();
    unscheduleAllCallbacks(); // Use newer method instead of unscheduleAllSelectors
    
    // Disable keyboard input
    disableKeyboardInput();
    
    // Stop all running actions
    this->stopAllActions();
    
    // Remove UI elements with safety checks
    if (_countdownLabel) {
        _countdownLabel->removeFromParentAndCleanup(true);
        _countdownLabel = nullptr;
        CCLOG("PostLevel4Handler: countdown label cleaned up");
    }
    
    if (_promptLabel) {
        _promptLabel->removeFromParentAndCleanup(true);
        _promptLabel = nullptr;
        CCLOG("PostLevel4Handler: prompt label cleaned up");
    }
    
    if (_overlay) {
        _overlay->removeFromParentAndCleanup(true);
        _overlay = nullptr;
        CCLOG("PostLevel4Handler: overlay cleaned up");
    }
    
    // Reset processing state
    _isProcessing = false;
    _countdownTimer = 30.0f; // Reset timer to default
    
    CCLOG("PostLevel4Handler resources fully cleaned up");
}

// Remove duplicate destructor definition

