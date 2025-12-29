// Item.cpp
#include "Item.h"
#include "ItemManager.h"
#include "cocos2d.h"
#include"ItemBagLayer.h"
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

// Item.cpp - 修复init函数中的图片路径
bool Item::init(const std::string& itemId) {
    _itemId = itemId;
    _isCollected = false;

    // 初始化世界坐标
    _worldPositionX = 0.0f;
    _worldPositionY = 0.0f;

    auto itemManager = ItemManager::getInstance();

    // 先调用initItemData设置物品信息
    initItemData();

    // 尝试加载图片
    auto fileUtils = FileUtils::getInstance();
    bool loaded = false;

    // 尝试多个可能的路径 - 更新为新的路径
    std::vector<std::string> possiblePaths = {
        itemId + ".png",
        "images/environment/items/" + itemId + ".png",
        "Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + _itemName + ".png"
    };

    // 特殊处理一些文件名不同的物品
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

        // 使用占位符
        if (!Sprite::init()) {
            return false;
        }
        this->setTextureRect(Rect(0, 0, 40, 40));
        this->setColor(Color3B::YELLOW);
    }

    // 设置物理体用于碰撞检测
    if (this->getContentSize().width > 0 && this->getContentSize().height > 0) {
        auto physicsBody = PhysicsBody::createBox(this->getContentSize());
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(0x02); // 物品类别
        physicsBody->setCollisionBitmask(0x01); // 与玩家碰撞
        physicsBody->setContactTestBitmask(0x01);
        this->setPhysicsBody(physicsBody);
    }
    else {
        log("WARNING: Item has invalid size, not adding physics body");
    }

    // 添加浮动动画
    auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
    auto moveDown = MoveBy::create(1.0f, Vec2(0, -10));
    auto sequence = Sequence::create(moveUp, moveDown, nullptr);
    this->runAction(RepeatForever::create(sequence));

    // 添加旋转动画
    auto rotate = RotateBy::create(2.0f, 360);
    this->runAction(RepeatForever::create(rotate));

    return true;
}

// Item.cpp - 完善initItemData函数
void Item::initItemData() {
    auto itemManager = ItemManager::getInstance();
    auto itemData = itemManager->getItemData(_itemId);

    if (itemData) {
        _itemName = itemData->name;
        _description = itemData->description;
        _itemType = itemData->type;
        _linkedAchievement = itemData->achievement;
    }
    else {
        // 设置物品数据
        if (_itemId == "study_du") {
            _itemName = "study du";
            _description = "警惕新型骗局：近期美国中情局发明了一款软件，它伪装成学习软件隐藏在大学生手机里，外形为红色圣诞帽，帽顶有一颗星星。该软件会影响学生身心健康，窃取秘密资料，影响学生睡眠质量和日常生活，而且还会让中情局特工发布定位信息签到码，窃取你的隐私安全，或者要输入指定的4位数摩尔斯密码，或者还要拍摄相关隐私图片上传才能成功签到，极大的影响了中国大学生的身心健康。如有发现，立刻对此软件进行举报。";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "do_not_know") {
            _itemName = "do_not_know";
            _description = "求16倍速播放网课教程。";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "poster") {
            _itemName = "poster";
            _description = "记载了众多骇人听闻的恶魔软件。我衷心祈祷看到这张海报的人还没有被毒害。";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "oolong") {
            _itemName = "oolong";
            _description = "爽口的饮料，据说对选课很有帮助(三得利打钱)。";
            _itemType = ItemType::CONSUMABLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "65472") {
            _itemName = "65472";
            _description = "同学说的不错\n校方很认可，\n留下学号\n给你加创新学分\n真的";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "";
        }
        else if (_itemId == "donation_certificate") {
            _itemName = "donation certificate";
            _description = "一年两度的慈善盛宴。";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "safety_helmet") {
            _itemName = "safety helmet";
            _description = "土木学长的圣遗物，依稀可见曾经的辉煌。";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "campus_card") {
            _itemName = "campus card";
            _description = "看起来很眼熟，直觉告诉你：经常有人弄丢它。";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "take_out") {
            _itemName = "take out";
            _description = "讨贼檄文\n--与贼义子决裂书\n朗朗乾坤天日下，初入寒冬心已凉!\n方点外卖楼下放，大雨闻歇脚步忙。\n上寻门口下望桌，两处茫茫皆不见。\n外卖小哥明置此，为何不入我眼帘?\n昔我偶闻盗食者，泯然一笑未以然。\n同为学院师兄弟，何以一餐作奸郎，\n今有竖子窃外卖，不觉替汝父母伤，\n汝之道义值几钱，忍心任狗食心良?\n汝之忠义价几两，同为一校却逆善?\n汝之孝义有几厘，敢私义父之钱粮?\n枉费学府之栽培，羞尽双亲之脸面\n为父常时怎训你，何以甘心作贼状。\n想此不禁双泪流，我儿骂名万人扬。\n不知真贫或真贱，贼字伴汝终不散\n天下谁人有汝行，他年去后无处葬\n贼不知吾乃你义父XXX，定叫竖子绿长存!\n然今后莫要唤吾为义父，不忍有此贼义子!";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "令人汗颜！------发自我的手机";
        }
        else if (_itemId == "hanoi_tower") {
            _itemName = "hanoi tower";
            _description = "我想起一些不好的回忆...";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "sj_larry") {
            _itemName = "SJ";
            _description = "时间？世界？数据？手机？睡觉？设计？暑假？实际？司机？三角？上交？世纪？洒家？随机？瞬间？神经？四季？商家？审计？三级？上架？视觉？书籍？升级？收集？Larry?!";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "";
        }
        else if (_itemId == "man") {
            _itemName = "man";
            _description = "我想你了...";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "iced_black_tea") {
            _itemName = "iced black tea";
            _description = "现在是6月13号晚上的8:63......";
            _itemType = ItemType::CONSUMABLE;
            _linkedAchievement = "";
        }
        else {
            // 默认值
            _itemName = _itemId;
            _description = "未知物品";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
    }
}

void Item::setWorldPosition(float worldX, float worldY) {
    _worldPositionX = worldX;
    _worldPositionY = worldY;
    // 立即设置精灵位置（需要在MainGameScene中根据摄像机偏移调整）
    this->setPosition(Vec2(worldX, worldY));
}

void Item::collect() {
    if (_isCollected) return;

    _isCollected = true;

    // 播放收集动画
    auto scaleUp = ScaleTo::create(0.2f, 1.5f);
    auto scaleDown = ScaleTo::create(0.2f, 0.1f);
    auto fadeOut = FadeOut::create(0.2f);
    auto remove = RemoveSelf::create();

    auto sequence = Sequence::create(scaleUp, scaleDown, fadeOut, remove, nullptr);
    this->runAction(sequence);

    // 通知ItemManager
    ItemManager::getInstance()->collectItem(_itemId);

    // 刷新背包显示（如果背包已打开）
    auto scene = Director::getInstance()->getRunningScene();
    if (scene) {
        // 寻找背包层并刷新
        scene->enumerateChildren("ItemBagLayer", [](Node* node) -> bool {
            auto bagLayer = dynamic_cast<ItemBagLayer*>(node);
            if (bagLayer && bagLayer->isBagVisible()) {
                bagLayer->refreshBag();
            }
            return false;
            });
    }

    // 显示收集提示
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithSystemFont("收集到: " + _itemName, "Arial", 24);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.8f));
    label->setColor(Color3B::YELLOW);

    scene->addChild(label, 100);

    // 提示文字动画
    auto fadeIn = FadeIn::create(0.5f);
    auto delay = DelayTime::create(1.5f);
    auto fadeOutLabel = FadeOut::create(0.5f);
    auto removeLabel = RemoveSelf::create();
    auto labelSequence = Sequence::create(fadeIn, delay, fadeOutLabel, removeLabel, nullptr);
    label->runAction(labelSequence);
}