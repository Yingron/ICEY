#include "AnimationManager.h"
#include "cocos2d.h"

USING_NS_CC;

AnimationManager* AnimationManager::_instance = nullptr;

AnimationManager::AnimationManager() {
    // 鏋勯€犲嚱鏁板垵濮嬪寲
}

AnimationManager* AnimationManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new AnimationManager();
        _instance->preloadEnemyAnimations();
        _instance->initEnemyConfigs();
    }
    return _instance;
}

void AnimationManager::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

AnimationManager::~AnimationManager() {
    // Release all animations
    for (auto& enemyEntry : _animations) {
        for (auto& animEntry : enemyEntry.second) {
            if (animEntry.second) {
                animEntry.second->release();
            }
        }
    }
    _animations.clear();
}

cocos2d::Animation* AnimationManager::getAnimation(const std::string& enemyType, const std::string& state) {
    auto enemyIt = _animations.find(enemyType);
    if (enemyIt != _animations.end()) {
        auto stateIt = enemyIt->second.find(state);
        if (stateIt != enemyIt->second.end()) {
            return stateIt->second;
        }
    }
    return nullptr;
}

float AnimationManager::getAnimationDelay(const std::string& enemyType, const std::string& state) {
    Animation* animation = getAnimation(enemyType, state);
    if (animation) {
        return animation->getDelayPerUnit();
    }
    return 0.1f; // 榛樿寤惰繜
}

const AnimationManager::EnemyConfig& AnimationManager::getEnemyConfig(const std::string& enemyType) {
    static EnemyConfig defaultConfig;
    auto it = _enemyConfigs.find(enemyType);
    if (it != _enemyConfigs.end()) {
        return it->second;
    }
    return defaultConfig;
}

void AnimationManager::preloadEnemyAnimations() {
    loadBoss1Animations();
    loadBoss2Animations();
    loadBoss3Animations();
    loadCloseCombat1Animations();
    loadCloseCombat2Animations();
    loadEliteEnemyAnimations();
    loadRemoteEnemyAnimations();

    log("All enemy animations preloaded");
}

// 锟斤拷锟斤拷BOSS1锟斤拷锟斤拷
void AnimationManager::loadBoss1Animations() {
    std::string bossDir = "images/characters/enemies/BOSS1-CAIXUKUN/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (34帧)
    Animation* idleAnim = Animation::create();
    for (int i = 1; i <= 34; i++) {
        std::string frameName = bossDir + "cxk-idle" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            idleAnim->addSpriteFrame(spriteFrame);
        }
    }
    idleAnim->setDelayPerUnit(0.08f);
    idleAnim->setRestoreOriginalFrame(true);

    idleAnim->retain();
    _animations["boss1"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "cxk-walk" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.1f);
    walkAnim->setRestoreOriginalFrame(true);

    walkAnim->retain();
    _animations["boss1"]["walk"] = walkAnim;
    _animations["boss1"]["run"] = walkAnim; // 使锟斤拷锟斤拷同锟斤拷锟斤拷锟斤拷锟斤拷锟劫度革拷锟斤拷

    // 锟斤拷锟斤拷准锟斤拷锟斤拷锟斤拷 (20帧)
    Animation* prepareAnim = Animation::create();
    for (int i = 1; i <= 20; i++) {
        std::string frameName = bossDir + "cxk-ready" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            prepareAnim->addSpriteFrame(spriteFrame);
        }
    }
    prepareAnim->setDelayPerUnit(0.05f);
    prepareAnim->setRestoreOriginalFrame(true);

    prepareAnim->retain();
    _animations["boss1"]["prepare"] = prepareAnim;

    // 远锟教癸拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* rangedAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "cxk-remote attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            rangedAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    rangedAttackAnim->setDelayPerUnit(0.08f);
    rangedAttackAnim->setRestoreOriginalFrame(true);

    rangedAttackAnim->retain();
    _animations["boss1"]["ranged_attack"] = rangedAttackAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* meleeAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "cxk-Close-range attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            meleeAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    meleeAttackAnim->setDelayPerUnit(0.08f);
    meleeAttackAnim->setRestoreOriginalFrame(true);

    meleeAttackAnim->retain();
    _animations["boss1"]["melee_attack"] = meleeAttackAnim;

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "cxk-hit" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);

    hurtAnim->retain();
    _animations["boss1"]["hurt"] = hurtAnim;
    
    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* deadAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "cxk-dead" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            deadAnim->addSpriteFrame(spriteFrame);
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);

    deadAnim->retain();
    _animations["boss1"]["dead"] = deadAnim;

    log("BOSS1 animations loaded");
}

// BOSS2-MAODIE锟斤拷锟斤拷
void AnimationManager::loadBoss2Animations() {
    std::string bossDir = "images/characters/enemies/BOSS2-MAODIE/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* idleAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-idle" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            idleAnim->addSpriteFrame(spriteFrame);
        }
    }
    idleAnim->setDelayPerUnit(0.08f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["boss2"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-walk" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.1f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["boss2"]["walk"] = walkAnim;
    _animations["boss2"]["run"] = walkAnim;

    // 锟斤拷锟斤拷准锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* prepareAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-ready" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            prepareAnim->addSpriteFrame(spriteFrame);
        }
    }
    prepareAnim->setDelayPerUnit(0.05f);
    prepareAnim->setRestoreOriginalFrame(true);
    prepareAnim->retain();
    _animations["boss2"]["prepare"] = prepareAnim;

    // 远锟教癸拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* rangedAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-remote attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            rangedAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    rangedAttackAnim->setDelayPerUnit(0.08f);
    rangedAttackAnim->setRestoreOriginalFrame(true);
    rangedAttackAnim->retain();
    _animations["boss2"]["ranged_attack"] = rangedAttackAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* meleeAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-Close-range attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            meleeAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    meleeAttackAnim->setDelayPerUnit(0.08f);
    meleeAttackAnim->setRestoreOriginalFrame(true);
    meleeAttackAnim->retain();
    _animations["boss2"]["melee_attack"] = meleeAttackAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-hurt" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["boss2"]["hurt"] = hurtAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* deadAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "md-dead" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            deadAnim->addSpriteFrame(spriteFrame);
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["boss2"]["dead"] = deadAnim;

    log("BOSS2 animations loaded");
}

// BOSS3-NAILONG锟斤拷锟斤拷
void AnimationManager::loadBoss3Animations() {
    std::string bossDir = "images/characters/enemies/BOSS3-NAILONG/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* idleAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-idle" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            idleAnim->addSpriteFrame(spriteFrame);
        }
    }
    idleAnim->setDelayPerUnit(0.08f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["boss3"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-walk" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.1f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["boss3"]["walk"] = walkAnim;
    _animations["boss3"]["run"] = walkAnim;

    // 锟斤拷锟斤拷准锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* prepareAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-ready" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            prepareAnim->addSpriteFrame(spriteFrame);
        }
    }
    prepareAnim->setDelayPerUnit(0.05f);
    prepareAnim->setRestoreOriginalFrame(true);
    prepareAnim->retain();
    _animations["boss3"]["prepare"] = prepareAnim;

    // 远锟教癸拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* rangedAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-remote attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            rangedAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    rangedAttackAnim->setDelayPerUnit(0.08f);
    rangedAttackAnim->setRestoreOriginalFrame(true);
    rangedAttackAnim->retain();
    _animations["boss3"]["ranged_attack"] = rangedAttackAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* meleeAttackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-Close-range attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            meleeAttackAnim->addSpriteFrame(spriteFrame);
        }
    }
    meleeAttackAnim->setDelayPerUnit(0.08f);
    meleeAttackAnim->setRestoreOriginalFrame(true);
    meleeAttackAnim->retain();
    _animations["boss3"]["melee_attack"] = meleeAttackAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-hurt" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["boss3"]["hurt"] = hurtAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* deadAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = bossDir + "nl-dead" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            deadAnim->addSpriteFrame(spriteFrame);
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["boss3"]["dead"] = deadAnim;

    log("BOSS3 animations loaded");
}

// 锟斤拷战锟斤拷锟斤拷1锟斤拷锟斤拷
void AnimationManager::loadCloseCombat1Animations() {
    std::string enemyDir = "images/characters/enemies/Close combat enemy1/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧) - 浣跨敤walk鍔ㄧ敾浣滀负fallback
    Animation* idleAnim = Animation::create();
    // 妫€鏌ユ槸鍚︽湁idle鍔ㄧ敾锛屽鏋滄病鏈夊垯浣跨敤walk鍔ㄧ敾
    bool hasIdleFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy1-idle" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            idleAnim->addSpriteFrame(spriteFrame);
            hasIdleFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁idle鍔ㄧ敾锛屽垯浣跨敤walk鍔ㄧ敾浣滀负idle鍔ㄧ敾
    if (!hasIdleFrames) {
        idleAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Close combat enemy1-walk" + std::to_string(i) + "_resized.png";
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                auto spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
                idleAnim->addSpriteFrame(spriteFrame);
            }
        }
    }
    idleAnim->setDelayPerUnit(0.1f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["close_combat1"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy1-walk" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.08f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["close_combat1"]["walk"] = walkAnim;
    _animations["close_combat1"]["run"] = walkAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* attackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy1-attack" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            attackAnim->addSpriteFrame(spriteFrame);
        }
    }
    attackAnim->setDelayPerUnit(0.08f);
    attackAnim->setRestoreOriginalFrame(true);
    attackAnim->retain();
    _animations["close_combat1"]["attack"] = attackAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷 (浣跨敤hit鏇夸唬hurt)
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy1-hit" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["close_combat1"]["hurt"] = hurtAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷 (濡傛灉娌℃湁dead鍔ㄧ敾锛屼娇鐢╤it鍔ㄧ敾浣滀负fallback)
    Animation* deadAnim = Animation::create();
    bool hasDeadFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy1-dead" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            deadAnim->addSpriteFrame(spriteFrame);
            hasDeadFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁dead鍔ㄧ敾锛屽垯浣跨敤hit鍔ㄧ敾浣滀负dead鍔ㄧ敾
    if (!hasDeadFrames) {
        deadAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Close combat enemy1-hit" + std::to_string(i) + "_resized.png";
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
                if (spriteFrame) {
                    deadAnim->addSpriteFrame(spriteFrame);
                }
            }
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["close_combat1"]["dead"] = deadAnim;

    log("CloseCombat1 animations loaded");
}

// 锟斤拷战锟斤拷锟斤拷2锟斤拷锟斤拷
void AnimationManager::loadCloseCombat2Animations() {
    std::string enemyDir = "images/characters/enemies/Close combat enemy2/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* idleAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy2-walk" + std::to_string(i) + "_resized.png"; // 浣跨敤walk鍔ㄧ敾浣滀负idle fallback
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            idleAnim->addSpriteFrame(spriteFrame);
        }
    }
    idleAnim->setDelayPerUnit(0.1f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["close_combat2"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy2-walk" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.08f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["close_combat2"]["walk"] = walkAnim;
    _animations["close_combat2"]["run"] = walkAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* attackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy2-attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            attackAnim->addSpriteFrame(spriteFrame);
        }
    }
    attackAnim->setDelayPerUnit(0.08f);
    attackAnim->setRestoreOriginalFrame(true);
    attackAnim->retain();
    _animations["close_combat2"]["attack"] = attackAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy2-hit" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["close_combat2"]["hurt"] = hurtAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* deadAnim = Animation::create();
    bool hasDeadFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Close combat enemy2-dead" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            deadAnim->addSpriteFrame(spriteFrame);
            hasDeadFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁dead鍔ㄧ敾锛屽垯浣跨敤hit鍔ㄧ敾浣滀负fallback
    if (!hasDeadFrames) {
        deadAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Close combat enemy2-hit" + std::to_string(i) + "_resized.png";
            auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            if (spriteFrame) {
                deadAnim->addSpriteFrame(spriteFrame);
            }
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["close_combat2"]["dead"] = deadAnim;

    log("CloseCombat2 animations loaded");
}

// 锟斤拷英锟斤拷锟斤拷锟斤拷锟斤拷
void AnimationManager::loadEliteEnemyAnimations() {
    std::string enemyDir = "images/characters/enemies/Elite enemy/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* idleAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Elite enemy-walk" + std::to_string(i) + "_resized.png"; // 浣跨敤walk鍔ㄧ敾浣滀负idle fallback
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            idleAnim->addSpriteFrame(spriteFrame);
        }
    }
    idleAnim->setDelayPerUnit(0.1f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["elite_enemy"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Elite enemy-walk" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.08f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["elite_enemy"]["walk"] = walkAnim;
    _animations["elite_enemy"]["run"] = walkAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* attackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Elite enemy-attack" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            attackAnim->addSpriteFrame(spriteFrame);
        }
    }
    attackAnim->setDelayPerUnit(0.08f);
    attackAnim->setRestoreOriginalFrame(true);
    attackAnim->retain();
    _animations["elite_enemy"]["attack"] = attackAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* hurtAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Elite enemy-hit" + std::to_string(i) + "_resized.png";
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            hurtAnim->addSpriteFrame(spriteFrame);
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["elite_enemy"]["hurt"] = hurtAnim;

    // 锟斤拷锟铰革拷同锟斤拷员锟斤拷锟斤拷锟斤拷
    Animation* deadAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Elite enemy-hit" + std::to_string(i) + "_resized.png"; // 浣跨敤hit鍔ㄧ敾浣滀负dead fallback
        auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (!spriteFrame) {
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            }
        }
        if (spriteFrame) {
            deadAnim->addSpriteFrame(spriteFrame);
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["elite_enemy"]["dead"] = deadAnim;

    log("EliteEnemy animations loaded");
}

// 远锟教碉拷锟斤拷锟斤拷锟斤拷
void AnimationManager::loadRemoteEnemyAnimations() {
    std::string enemyDir = "images/characters/enemies/Remote combat enemy/";

    // 锟斤拷锟斤拷锟斤拷锟斤拷 (10帧) - 浣跨敤walk鍔ㄧ敾浣滀负fallback
    Animation* idleAnim = Animation::create();
    // 妫€鏌ユ槸鍚︽湁idle鍔ㄧ敾锛屽鏋滄病鏈夊垯浣跨敤walk鍔ㄧ敾
    bool hasIdleFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Remote combat enemy-idle" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            idleAnim->addSpriteFrame(spriteFrame);
            hasIdleFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁idle鍔ㄧ敾锛屽垯浣跨敤walk鍔ㄧ敾浣滀负idle鍔ㄧ敾
    if (!hasIdleFrames) {
        idleAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Remote combat enemy-walk" + std::to_string(i) + "_resized.png";
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                auto spriteFrame = SpriteFrame::createWithTexture(texture,
                    Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
                idleAnim->addSpriteFrame(spriteFrame);
            }
        }
    }
    idleAnim->setDelayPerUnit(0.1f);
    idleAnim->setRestoreOriginalFrame(true);
    idleAnim->retain();
    _animations["remote_enemy"]["idle"] = idleAnim;

    // 锟斤拷锟竭讹拷锟斤拷 (10帧)
    Animation* walkAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Remote combat enemy-walk" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            walkAnim->addSpriteFrame(spriteFrame);
        }
    }
    walkAnim->setDelayPerUnit(0.08f);
    walkAnim->setRestoreOriginalFrame(true);
    walkAnim->retain();
    _animations["remote_enemy"]["walk"] = walkAnim;
    _animations["remote_enemy"]["run"] = walkAnim;

    // 锟斤拷战锟斤拷锟斤拷锟斤拷锟斤拷 (10帧)
    Animation* attackAnim = Animation::create();
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Remote combat enemy-attack" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            attackAnim->addSpriteFrame(spriteFrame);
        }
    }
    attackAnim->setDelayPerUnit(0.08f);
    attackAnim->setRestoreOriginalFrame(true);
    attackAnim->retain();
    _animations["remote_enemy"]["attack"] = attackAnim;

    // 杩滅▼鏁屼汉鍙椾激鍔ㄧ敾 - 浣跨敤attack鍔ㄧ敾浣滀负fallback
    Animation* hurtAnim = Animation::create();
    bool hasHurtFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Remote combat enemy-hurt" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            hurtAnim->addSpriteFrame(spriteFrame);
            hasHurtFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁hurt鍔ㄧ敾锛屽垯浣跨敤attack鍔ㄧ敾浣滀负hurt鍔ㄧ敾
    if (!hasHurtFrames) {
        hurtAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Remote combat enemy-attack" + std::to_string(i) + "_resized.png";
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
                if (spriteFrame) {
                    hurtAnim->addSpriteFrame(spriteFrame);
                }
            }
        }
    }
    hurtAnim->setDelayPerUnit(0.1f);
    hurtAnim->setRestoreOriginalFrame(true);
    hurtAnim->retain();
    _animations["remote_enemy"]["hurt"] = hurtAnim;
    
    // 杩滅▼鏁屼汉姝讳骸鍔ㄧ敾 - 浣跨敤walk鍔ㄧ敾浣滀负fallback
    Animation* deadAnim = Animation::create();
    bool hasDeadFrames = false;
    for (int i = 1; i <= 10; i++) {
        std::string frameName = enemyDir + "Remote combat enemy-dead" + std::to_string(i) + "_resized.png";
        auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
        if (texture) {
            auto spriteFrame = SpriteFrame::createWithTexture(texture,
                Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, frameName);
            deadAnim->addSpriteFrame(spriteFrame);
            hasDeadFrames = true;
        }
    }
    
    // 濡傛灉娌℃湁dead鍔ㄧ敾锛屽垯浣跨敤walk鍔ㄧ敾浣滀负dead鍔ㄧ敾
    if (!hasDeadFrames) {
        deadAnim = Animation::create();
        for (int i = 1; i <= 10; i++) {
            std::string frameName = enemyDir + "Remote combat enemy-walk" + std::to_string(i) + "_resized.png";
            auto texture = Director::getInstance()->getTextureCache()->addImage(frameName);
            if (texture) {
                auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
                if (spriteFrame) {
                    deadAnim->addSpriteFrame(spriteFrame);
                }
            }
        }
    }
    deadAnim->setDelayPerUnit(0.1f);
    deadAnim->setRestoreOriginalFrame(true);
    deadAnim->retain();
    _animations["remote_enemy"]["dead"] = deadAnim;

    log("RemoteEnemy animations loaded");
}

void AnimationManager::initEnemyConfigs() {
    // BOSS1锟斤拷锟斤拷
    EnemyConfig boss1Config;
    boss1Config.maxHealth = 1000.0f;
    boss1Config.attackDamage = 50.0f;
    boss1Config.moveSpeed = 120.0f;
    boss1Config.attackRange = 80.0f;
    boss1Config.detectionRange = 500.0f;
    boss1Config.attackCooldown = 2.0f;
    boss1Config.name = "BOSS1-CAIXUNKUN";
    boss1Config.isBoss = true;
    boss1Config.phaseCount = 3;
    _enemyConfigs["boss1"] = boss1Config;

    // BOSS2锟斤拷锟斤拷
    EnemyConfig boss2Config;
    boss2Config.maxHealth = 800.0f;
    boss2Config.attackDamage = 40.0f;
    boss2Config.moveSpeed = 100.0f;
    boss2Config.attackRange = 70.0f;
    boss2Config.detectionRange = 400.0f;
    boss2Config.attackCooldown = 1.8f;
    boss2Config.name = "BOSS2-MAODIE";
    boss2Config.isBoss = true;
    boss2Config.phaseCount = 2;
    _enemyConfigs["boss2"] = boss2Config;

    // BOSS3锟斤拷锟斤拷
    EnemyConfig boss3Config;
    boss3Config.maxHealth = 1200.0f;
    boss3Config.attackDamage = 60.0f;
    boss3Config.moveSpeed = 90.0f;
    boss3Config.attackRange = 90.0f;
    boss3Config.detectionRange = 450.0f;
    boss3Config.attackCooldown = 2.2f;
    boss3Config.name = "BOSS3-NAILONG";
    boss3Config.isBoss = true;
    boss3Config.phaseCount = 3;
    _enemyConfigs["boss3"] = boss3Config;

    // 锟斤拷战锟斤拷锟斤拷1锟斤拷锟斤拷
    EnemyConfig closeCombat1Config;
    closeCombat1Config.maxHealth = 100.0f;
    closeCombat1Config.attackDamage = 20.0f;
    closeCombat1Config.moveSpeed = 80.0f;
    closeCombat1Config.attackRange = 50.0f;
    closeCombat1Config.detectionRange = 150.0f;
    closeCombat1Config.attackCooldown = 1.0f;
    closeCombat1Config.name = "锟斤拷战锟斤拷锟斤拷1";
    closeCombat1Config.isBoss = false;
    closeCombat1Config.phaseCount = 1;
    _enemyConfigs["close_combat1"] = closeCombat1Config;

    // 锟斤拷战锟斤拷锟斤拷2锟斤拷锟斤拷
    EnemyConfig closeCombat2Config;
    closeCombat2Config.maxHealth = 120.0f;
    closeCombat2Config.attackDamage = 25.0f;
    closeCombat2Config.moveSpeed = 70.0f;
    closeCombat2Config.attackRange = 55.0f;
    closeCombat2Config.detectionRange = 160.0f;
    closeCombat2Config.attackCooldown = 1.2f;
    closeCombat2Config.name = "锟斤拷战锟斤拷锟斤拷2";
    closeCombat2Config.isBoss = false;
    closeCombat2Config.phaseCount = 1;
    _enemyConfigs["close_combat2"] = closeCombat2Config;

    // 锟斤拷英锟斤拷锟斤拷锟斤拷锟斤拷
    EnemyConfig eliteEnemyConfig;
    eliteEnemyConfig.maxHealth = 200.0f;
    eliteEnemyConfig.attackDamage = 30.0f;
    eliteEnemyConfig.moveSpeed = 60.0f;
    eliteEnemyConfig.attackRange = 40.0f;
    eliteEnemyConfig.detectionRange = 180.0f;
    eliteEnemyConfig.attackCooldown = 1.5f;
    eliteEnemyConfig.name = "锟斤拷英锟斤拷锟斤拷";
    eliteEnemyConfig.isBoss = false;
    eliteEnemyConfig.phaseCount = 1;
    _enemyConfigs["elite_enemy"] = eliteEnemyConfig;

    // 远锟教碉拷锟斤拷锟斤拷锟斤拷
    EnemyConfig remoteEnemyConfig;
    remoteEnemyConfig.maxHealth = 80.0f;
    remoteEnemyConfig.attackDamage = 15.0f;
    remoteEnemyConfig.moveSpeed = 50.0f;
    remoteEnemyConfig.attackRange = 200.0f;
    remoteEnemyConfig.detectionRange = 300.0f;
    remoteEnemyConfig.attackCooldown = 1.8f;
    remoteEnemyConfig.name = "远锟教碉拷锟斤拷";
    remoteEnemyConfig.isBoss = false;
    remoteEnemyConfig.phaseCount = 1;
    _enemyConfigs["remote_enemy"] = remoteEnemyConfig;
}
