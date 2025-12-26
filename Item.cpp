// Item.cpp
#include "Item.h"
#include "ItemManager.h"
#include "cocos2d.h"
USING_NS_CC;

Item* Item::create(const std::string& itemId) {
    Item* item = new (std::nothrow) Item();
    if (item && item->init(itemId)) {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

// Item.cpp - fixed init method with corrected image paths
bool Item::init(const std::string& itemId) {
    _itemId = itemId;
    _isCollected = false;

    // Initialize world position
    _worldPositionX = 0.0f;
    _worldPositionY = 0.0f;

    // First initialize item data
    initItemData();

    // Load the sprite
    auto fileUtils = FileUtils::getInstance();
    bool loaded = false;

    // Try multiple possible paths - these are new paths
    std::vector<std::string> possiblePaths = {
        itemId + ".png",
        "images/environment/items/" + itemId + ".png",
        "Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + _itemName + ".png"
    };

    // Special handling for some items
    if (_itemId == "sj_larry") {
        possiblePaths.insert(possiblePaths.begin(), {
            "Larry.png",
            "images/environment/items/Larry.png",
            "Resources/images/environment/items/Larry.png",
            "C:/aishi/test2/Resources/images/environment/items/Larry.png"
            });
    }
    else if (_itemId == "study_du") {
        possiblePaths.insert(possiblePaths.begin(), {
            "study_du.png",
            "images/environment/items/study_du.png"
            });
    }

    for (const auto& path : possiblePaths) {
        if (fileUtils->isFileExist(path)) {
            if (Sprite::initWithFile(path)) {
                loaded = true;
                log("Successfully loaded item image: %s from %s", itemId.c_str(), path.c_str());
                break;
            }
        }
    }

    if (!loaded) {
        log("WARNING: Could not find item image for: %s, using placeholder", itemId.c_str());

        // Use placeholder
        if (!Sprite::init()) {
            return false;
        }
        this->setTextureRect(Rect(0, 0, 40, 40));
        this->setColor(Color3B::YELLOW);
    }

    // Add physics body only if size is valid
    auto contentSize = this->getContentSize();
    if (contentSize.width > 0 && contentSize.height > 0) {
        auto physicsBody = PhysicsBody::createBox(contentSize);
        if (physicsBody) {
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setCategoryBitmask(0x02); // Item category
            physicsBody->setCollisionBitmask(0x01); // Collide with player
            physicsBody->setContactTestBitmask(0x01);
            this->setPhysicsBody(physicsBody);
        }
    }
    else {
        log("WARNING: Item has invalid size, not adding physics body");
    }

    // Add floating animation
    auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
    auto moveDown = MoveBy::create(1.0f, Vec2(0, -10));
    auto sequence = Sequence::create(moveUp, moveDown, nullptr);
    this->runAction(RepeatForever::create(sequence));

    // Add rotation animation
    auto rotate = RotateBy::create(2.0f, 360);
    this->runAction(RepeatForever::create(rotate));

    return true;
}

// Item.cpp - fixed initItemData method
void Item::initItemData() {
    // Initialize item data
    if (_itemId == "study_du") {
        _itemName = "study du";
        _description = "A study guide document.";
        _itemType = ItemType::DOCUMENT;
        _linkedAchievement = "";
    }
    else if (_itemId == "do_not_know") {
        _itemName = "do_not_know";
        _description = "Class 16 student information.";
        _itemType = ItemType::DOCUMENT;
        _linkedAchievement = "";
    }
    else if (_itemId == "poster") {
        _itemName = "poster";
        _description = "A colorful poster.";
        _itemType = ItemType::DOCUMENT;
        _linkedAchievement = "";
    }
    else if (_itemId == "oolong") {
        _itemName = "oolong";
        _description = "A tea item.";
        _itemType = ItemType::CONSUMABLE;
        _linkedAchievement = "";
    }
    else if (_itemId == "65472") {
        _itemName = "65472";
        _description = "Student notes.";
        _itemType = ItemType::KEY_ITEM;
        _linkedAchievement = "";
    }
    else if (_itemId == "donation_certificate") {
        _itemName = "donation certificate";
        _description = "A donation certificate.";
        _itemType = ItemType::DOCUMENT;
        _linkedAchievement = "";
    }
    else if (_itemId == "safety_helmet") {
        _itemName = "safety helmet";
        _description = "A safety helmet.";
        _itemType = ItemType::COLLECTIBLE;
        _linkedAchievement = "";
    }
    else if (_itemId == "campus_card") {
        _itemName = "campus card";
        _description = "A campus card.";
        _itemType = ItemType::COLLECTIBLE;
        _linkedAchievement = "";
    }
    else if (_itemId == "take_out") {
        _itemName = "take out";
        _description = "A take out order.";
        _itemType = ItemType::KEY_ITEM;
        _linkedAchievement = "";
    }
    else if (_itemId == "hanoi_tower") {
        _itemName = "hanoi tower";
        _description = "A puzzle game.";
        _itemType = ItemType::COLLECTIBLE;
        _linkedAchievement = "";
    }
    else if (_itemId == "sj_larry") {
        _itemName = "SJ";
        _description = "A collectible item.";
        _itemType = ItemType::KEY_ITEM;
        _linkedAchievement = "";
    }
    else {
        // Default item data - 修复：使用成员变量 _itemId
        _itemName = _itemId;
        _description = "An item.";
        _itemType = ItemType::COLLECTIBLE;
        _linkedAchievement = "";
    }
}

void Item::setWorldPosition(float x, float y) {
    _worldPositionX = x;
    _worldPositionY = y;
}

// 添加 collect 函数实现
void Item::collect() {
    _isCollected = true;
    // Handle collection logic
    this->setVisible(false);
    auto physicsBody = this->getPhysicsBody();
    if (physicsBody) {
        physicsBody->setEnabled(false);
    }
}