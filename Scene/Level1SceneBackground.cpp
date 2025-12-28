#include "Level1SceneBackground.h"
#include "GameConfig.h"
#include "LevelManager.h"

USING_NS_CC;

bool SceneBackground::init() {
    if (!Node::init()) return false;

    // 锟斤拷锟矫碉拷前锟截匡拷锟侥憋拷锟斤拷
    setupLevel1Background();

    scheduleUpdate();

    return true;
}

void SceneBackground::setupLevel1Background() {
    log("=== Setting up Scene Background ===");

    // 锟斤拷取锟斤拷前锟截匡拷锟侥憋拷锟斤拷图片锟叫憋拷
    std::vector<std::string> bgImages = getCurrentLevelBackgroundImages();

    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    _continuousBackground = ContinuousBackground::createWithImageSequence(bgImages);
    if (_continuousBackground) {
        this->addChild(_continuousBackground);
        log("Scene background created successfully");
    }
    else {
        log("ERROR: Failed to create scene background");
    }
}

// Level1SceneBackground.cpp - 锟睫革拷 getCurrentLevelBackgroundImages 锟斤拷锟斤拷
std::vector<std::string> SceneBackground::getCurrentLevelBackgroundImages() {
    std::vector<std::string> images;

    // 锟斤拷取锟斤拷前锟截匡拷
    auto levelManager = LevelManager::getInstance();
    auto currentLevel = levelManager->getCurrentLevel();

    // 锟斤拷锟捷关匡拷枚锟劫凤拷锟截讹拷应锟侥憋拷锟斤拷图片
    if (currentLevel == LevelManager::LevelState::LEVEL1) {
        images = {
            "background-level1-1.png",
            "background-level1-2.png",
            "background-level1-3.png",
            "background-level1-4.png",
            "background-level1-5.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_1) {
        images = { "background-level2-1.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_2) {
        images = { "background-level2-2.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_3) {
        images = { "background-level2-3.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_4) {
        images = { "background-level2-4.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_5) {
        images = { "background-level2-5.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL2_6) {
        images = { "background-level2-6.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_1) {
        // 锟斤拷展为锟斤拷锟斤拷锟结：5锟斤拷图片
        images = {
            "background-level3-1-1.png",
            "background-level3-1-2.png",
            "background-level3-1-3.png",
            "background-level3-1-4.png",
            "background-level3-1-5.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_2) {
        // 锟斤拷展为锟斤拷锟斤拷锟结：3锟斤拷图片
        images = {
            "background-level3-2-1.png",
            "background-level3-2-2.png",
            "background-level3-2-3.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_3) {
        images = { "background-level3-3.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_4) {
        images = { "background-level3-4.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_5) {
        images = { "background-level3-5.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL3_6) {
        images = { "background-level3-6.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_1) {
        images = { "background-level4-1.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_2) {
        images = { "background-level4-2.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_3) {
        // 锟斤拷展为锟斤拷锟斤拷锟结：3锟斤拷图片
        images = {
            "background-level4-3-1.png",
            "background-level4-3-2.png",
            "background-level4-3-3.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_4) {
        images = { "background-level4-4.png" };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_5) {
        // 锟斤拷展为锟斤拷锟斤拷锟结：3锟斤拷图片
        images = {
            "background-level4-5-1.png",
            "background-level4-5-2.png",
            "background-level4-5-3.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::LEVEL4_6) {
        // 锟斤拷展为锟斤拷锟斤拷锟结：3锟斤拷图片
        images = {
            "background-level4-6-1.png",
            "background-level4-6-2.png",
            "background-level4-6-3.png"
        };
    }
    else if (currentLevel == LevelManager::LevelState::FINAL_LEVEL) {
        // 锟斤拷锟较柏课ｏ拷锟斤拷4锟斤拷图片
        images = {
            "final-level-4.png",
            "final-level-4.png",
            "final-level-4.png",
            "final-level-4.png"
        };
    }

    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> verifiedImages;

    // 锟斤拷锟斤拷募锟斤拷欠锟斤拷锟节ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷路锟斤拷
    for (const auto& filename : images) {
        std::vector<std::string> basePaths = {
            "",
            "Resources/images/environment/background/",
            "images/environment/background/",
            "images/",
            "../Resources/images/environment/background/",
            "../../Resources/images/environment/background/",
            "C:/aishi/test2/Resources/images/environment/background/"  // 锟斤拷锟斤拷路锟斤拷
        };

        bool found = false;
        for (const auto& basePath : basePaths) {
            std::string fullPath = basePath + filename;
            if (fileUtils->isFileExist(fullPath)) {
                verifiedImages.push_back(fullPath);
                log("Found background image: %s", fullPath.c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            log("WARNING: Could not find background image: %s", filename.c_str());
            verifiedImages.push_back(""); // 占位
        }
    }

    return verifiedImages;
}

void SceneBackground::reloadCurrentLevelBackground() {
    // 锟接革拷锟节碉拷锟狡筹拷锟缴的憋拷锟斤拷
    if (_continuousBackground) {
        _continuousBackground->removeFromParent();
        _continuousBackground = nullptr;
    }

    // 锟斤拷锟铰硷拷锟截憋拷锟斤拷
    setupLevel1Background();
}

void SceneBackground::updateWithPlayerSpeed(float delta, float playerSpeed) {
    // 锟斤拷锟斤拷锟揭拷锟斤拷锟斤拷锟斤拷锟劫度革拷锟铰憋拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷实锟斤拷
    if (_continuousBackground) {
        // _continuousBackground->setScrollSpeed(playerSpeed * 0.5f);
    }
}

float SceneBackground::getScrollSpeed() const {
    if (_continuousBackground) {
        return _continuousBackground->getScrollSpeed();
    }
    return 0.0f;
}

void SceneBackground::update(float delta) {
    Node::update(delta);

    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟接碉拷锟斤拷锟斤拷息
    static float debugTimer = 0.0f;
    debugTimer += delta;
    if (debugTimer > 2.0f) {
        debugTimer = 0.0f;
        log("Scene background scroll speed: %.1f", getScrollSpeed());
    }
}

float SceneBackground::getWorldWidth() const {
    if (_continuousBackground) {
        return _continuousBackground->getWorldWidth();
    }
    return 0.0f;
}

void SceneBackground::updateWithCameraOffset(float cameraOffsetX) {
    if (_continuousBackground) {
        _continuousBackground->updateWithCameraOffset(cameraOffsetX);
    }
}

std::string SceneBackground::getImagePath(const std::string& filename) {
    auto fileUtils = FileUtils::getInstance();

    std::vector<std::string> paths = {
        filename,
        "images/environment/background/" + filename,
        "images/" + filename,
        "Resources/images/environment/background/" + filename,
        "Resources/images/" + filename,
        "C:/aishi/test2/Resources/images/environment/background/" + filename
    };

    for (const auto& path : paths) {
        if (fileUtils->isFileExist(path)) {
            return path;
        }
    }

    return "";
}