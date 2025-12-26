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
    initAllItems();
    loadItemStates();
}

void ItemManager::initAllItems() {
    // 物品一：学习堵
    registerItem("study_block", "学习堵",
        "警惕新型骗局：近期美国中情局发明了一款软件，它伪装成学习软件隐藏在大学生手机里，"
        "外形为红色圣诞帽，帽顶有一颗星星。该软件会影响学生身心健康，窃取秘密资料...",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/学习堵.png",
        "");

    // 物品二：知不到
    registerItem("know_not", "知不到",
        "求16倍速播放网课教程。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/知不到.png",
        "");

    // 物品三：一张海报
    registerItem("poster", "一张海报",
        "记载了众多骇人听闻的恶魔软件。我衷心祈祷看到这张海报的人还没有被毒害。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/一张海报.png",
        "");

    // 物品四：乌龙茶
    registerItem("oolong_tea", "乌龙茶",
        "爽口的饮料，据说对选课很有帮助(三得利打钱)。",
        ItemType::CONSUMABLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/乌龙茶.png",
        "");

    // 物品五：65472
    registerItem("code_65472", "65472",
        "同学说的不错\n校方很认可，\n留下学号\n给你加创新学分\n真的",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/65472.png",
        "");

    // 物品六：捐款证明
    registerItem("donation_proof", "捐款证明",
        "一年两度的慈善盛宴。",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/捐款证明.png",
        "");

    // 物品七：安全帽
    registerItem("safety_helmet", "安全帽",
        "土木学长的圣遗物，依稀可见曾经的辉煌。",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/安全帽.png",
        "");

    // 物品八：不知道属于谁的校园卡
    registerItem("campus_card", "校园一卡通",
        "看起来很眼熟，直觉告诉你：经常有人弄丢它。",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/校园一卡通.png",
        "");

    // 物品九：放在路边的外卖
    registerItem("takeout", "外卖",
        "讨贼檄文\n--与贼义子决裂书\n朗朗乾坤天日下，初入寒冬心已凉!...",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/外卖.png",
        "令人汗颜！------发自我的手机");

    // 物品十：汉诺塔
    registerItem("hanoi_tower", "汉诺塔",
        "我想起一些不好的回忆...",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/汉诺塔.png",
        "");

    // 物品十一：SJ
    registerItem("sj_larry", "SJ",
        "时间？世界？数据？手机？睡觉？设计？暑假？实际？司机？三角？上交？世纪？洒家？随机？瞬间？神经？四季？商家？审计？三级？上架？视觉？书籍？升级？收集？Larry?!",
        ItemType::KEY_ITEM,
        "C:/aishi/test2/Resources/images/environment/interactive items/Larry.png",
        "");

    // 额外物品：告示栏
    registerItem("bulletin_board", "告示栏",
        "校园信息公告板",
        ItemType::DOCUMENT,
        "C:/aishi/test2/Resources/images/environment/interactive items/告示栏.png",
        "");

    // 额外物品：垃圾桶一号
    registerItem("trash_bin_1", "垃圾桶一号",
        "校园垃圾桶，里面可能会有意想不到的东西",
        ItemType::COLLECTIBLE,
        "C:/aishi/test2/Resources/images/environment/interactive items/垃圾桶一号.png",
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