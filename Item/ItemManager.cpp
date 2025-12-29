// ItemManager.cpp
#include "ItemManager.h"
#include "cocos2d.h"
#include <fstream>
#include"AudioManager.h"

USING_NS_CC;

ItemManager* ItemManager::_instance = nullptr;

ItemManager* ItemManager::getInstance() {
    if (!_instance) {
        _instance = new ItemManager();
    }
    return _instance;
}

ItemManager::ItemManager() {
    _itemDatabase.clear();
    initAllItems();
    // 强制设置所有物品为未收集
    for (auto& pair : _itemDatabase) {
        pair.second.isCollected = false;
    }

    // 保存这个初始状态
    saveItemStates();
}

void ItemManager::initAllItems() {
    // 注意：这里的物品ID必须与 Item.cpp 中的物品ID完全一致

    // 物品一：学习堵
    registerItem("study_du", u8"学习堵",  // 改为 "study_du" 与 Item.cpp 一致
        u8"警惕新型骗局：近期美国中情局发明了一款软件，它伪装成学习软件隐藏在大学生手机里，外形为红色圣诞帽，帽顶有一颗星星。该软件会影响学生身心健康，窃取秘密资料，影响学生睡眠质量和日常生活，而且还会让中情局特工发布定位信息签到码，窃取你的隐私安全，或者要输入指定的4位数摩尔斯密码，或者还要拍摄相关隐私图片上传才能成功签到，极大的影响了中国大学生的身心健康。如有发现，立刻对此软件进行举报。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/学习堵.png",
        "");

    // 物品二：知不到
    registerItem("do_not_know", u8"知不到",  // 改为 "do_not_know"
        u8"求16倍速播放网课教程。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/知不到.png",
        "");

    // 物品三：一张海报
    registerItem("poster", u8"一张海报",
        u8"记载了众多骇人听闻的恶魔软件。我衷心祈祷看到这张海报的人还没有被毒害。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/一张海报.png",
        "");

    // 物品四：乌龙茶
    registerItem("oolong", u8"乌龙茶",  // 改为 "oolong"
        u8"爽口的饮料，据说对选课很有帮助(三得利打钱)。",
        ItemType::CONSUMABLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/乌龙茶.png",
        "");

    // 物品五：65472
    registerItem("65472", u8"65472",  // 改为 "65472"
        u8"同学说的不错\n校方很认可，\n留下学号\n给你加创新学分\n真的",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/65472.png",
        "");

    // 物品六：捐款证明
    registerItem("donation_certificate", u8"捐款证明",  // 改为 "donation_certificate"
        u8"一年两度的慈善盛宴。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/捐款证明.png",
        "");

    // 物品七：安全帽
    registerItem("safety_helmet", u8"安全帽",
        u8"土木学长的圣遗物，依稀可见曾经的辉煌。",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/安全帽.png",
        "");

    // 物品八：校园卡
    registerItem("campus_card", u8"校园卡",
        u8"看起来很眼熟，直觉告诉你：经常有人弄丢它。",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/校园卡.png",
        "");

    // 物品九：外卖
    registerItem("take_out", u8"外卖",  // 改为 "take_out"
        u8"讨贼檄文\n--与贼义子决裂书\n朗朗乾坤天日下，初入寒冬心已凉!\n方点外卖楼下放，大雨闻歇脚步忙。\n上寻门口下望桌，两处茫茫皆不见。\n外卖小哥明置此，为何不入我眼帘?\n昔我偶闻盗食者，泯然一笑未以然。\n同为学院师兄弟，何以一餐作奸郎，\n今有竖子窃外卖，不觉替汝父母伤，\n汝之道义值几钱，忍心任狗食心良?\n汝之忠义价几两，同为一校却逆善?\n汝之孝义有几厘，敢私义父之钱粮?\n枉费学府之栽培，羞尽双亲之脸面\n为父常时怎训你，何以甘心作贼状。\n想此不禁双泪流，我儿骂名万人扬。\n不知真贫或真贱，贼字伴汝终不散\n天下谁人有汝行，他年去后无处葬\n贼不知吾乃你义父XXX，定叫竖子绿长存!\n然今后莫要唤吾为义父，不忍有此贼义子!",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/外卖.png",
        "令人汗颜！------发自我的手机");

    // 物品十：汉诺塔
    registerItem("hanoi_tower", u8"汉诺塔",
        u8"我想起一些不好的回忆...",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/汉诺塔.png",
        "");

    // 物品十一：SJ
    registerItem("sj_larry", u8"SJ",
        u8"时间？世界？数据？手机？睡觉？设计？暑假？实际？司机？三角？上交？世纪？洒家？随机？瞬间？神经？四季？商家？审计？三级？上架？视觉？书籍？升级？收集？Larry?!",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/Larry.png",
        "");

    // 物品十二：男人
    registerItem("man", u8"男人",
        u8"我想你了...",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/男人.png",
        "");

    // 物品十三：冰红茶
    registerItem("iced_black_tea", u8"冰红茶",
        u8"现在是6月13号晚上的8:63......",
        ItemType::CONSUMABLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/冰红茶.png",
        "");

    log("ItemManager: Initialized %d items", _itemDatabase.size());
}

void ItemManager::registerItem(const std::string& itemId, const std::string& name,
    const std::string& description, ItemType type,
    const std::string& imagePath, const std::string& achievement) {
    ItemData data;
    data.name = name;
    data.description = description;
    data.type = type;
    data.imagePath = imagePath;
    data.achievement = achievement;
    data.isCollected = false;

    _itemDatabase[itemId] = data;
}

std::string ItemManager::getItemName(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.name;
    }
    return "未知物品";
}

std::string ItemManager::getItemDescription(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.description;
    }
    return "没有描述";
}

ItemType ItemManager::getItemType(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.type;
    }
    return ItemType::COLLECTIBLE;
}

ItemManager::ItemData* ItemManager::getItemData(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        // 返回容器中元素的地址（指针）
        return &(it->second);
    }
    // 找不到时返回空指针，调用方需判断是否为空
    return nullptr;
}

Item* ItemManager::createItem(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it == _itemDatabase.end()) {
        log("ItemManager: Item not found: %s", itemId.c_str());
        return nullptr;
    }

    Item* item = Item::create(itemId);
    return item;
}

void ItemManager::collectItem(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end() && !it->second.isCollected) {
        it->second.isCollected = true;
        log("ItemManager: Collected item: %s", itemId.c_str());

        // 检查成就
        checkAchievementConditions();

        // 保存状态
        saveItemStates();
    }
}

bool ItemManager::isItemCollected(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.isCollected;
    }
    return false;
}

int ItemManager::getCollectedCount() const {
    int count = 0;
    for (const auto& pair : _itemDatabase) {
        if (pair.second.isCollected) {
            count++;
        }
    }
    return count;
}

int ItemManager::getTotalItemCount() const {
    return _itemDatabase.size();
}

std::vector<std::string> ItemManager::getCollectedItems() const {
    std::vector<std::string> collected;
    for (const auto& pair : _itemDatabase) {
        if (pair.second.isCollected) {
            collected.push_back(pair.first);
        }
    }
    return collected;
}

void ItemManager::saveItemStates() {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "items.dat";
    std::ofstream file(filePath, std::ios::binary);

    if (file.is_open()) {
        size_t count = _itemDatabase.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& pair : _itemDatabase) {
            // 写入物品ID
            size_t idLength = pair.first.length();
            file.write(reinterpret_cast<const char*>(&idLength), sizeof(idLength));
            file.write(pair.first.c_str(), idLength);

            // 写入收集状态
            file.write(reinterpret_cast<const char*>(&pair.second.isCollected), sizeof(bool));
        }

        file.close();
        log("ItemManager: Saved item states to %s", filePath.c_str());
    }
}

void ItemManager::loadItemStates() {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "items.dat";
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        log("ItemManager: No saved item states found");
        return;
    }

    size_t count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; i++) {
        // 读取物品ID
        size_t idLength = 0;
        file.read(reinterpret_cast<char*>(&idLength), sizeof(idLength));

        std::string itemId(idLength, '\0');
        file.read(&itemId[0], idLength);

        // 读取收集状态
        bool isCollected = false;
        file.read(reinterpret_cast<char*>(&isCollected), sizeof(bool));

        // 更新数据库
        auto it = _itemDatabase.find(itemId);
        if (it != _itemDatabase.end()) {
            it->second.isCollected = isCollected;
        }
    }

    file.close();
    log("ItemManager: Loaded item states from %s", filePath.c_str());
}

void ItemManager::checkAchievementConditions() {
    int collected = getCollectedCount();
    int total = getTotalItemCount();

    // 检查"它们都是为了忘却的纪念"成就（找到所有物品）
    if (collected == total) {
        log("ACHIEVEMENT UNLOCKED: 它们都是为了忘却的纪念");
        // 这里可以触发成就系统
    }

    // 检查特定物品的成就
    for (const auto& pair : _itemDatabase) {
        if (pair.second.isCollected && !pair.second.achievement.empty()) {
            log("ACHIEVEMENT UNLOCKED: %s", pair.second.achievement.c_str());
        }
    }
}