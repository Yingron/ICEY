// ItemManager.cpp
#include "ItemManager.h"
#include "cocos2d.h"
#include <fstream>
#include "AudioManager.h"

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

    // 保存初始状态
    saveItemStates();
}

void ItemManager::initAllItems() {
    // 注意：所有物品ID需要与 Item.cpp 中的物品ID完全一致

    // 物品一：学习单
    registerItem("study_du", u8"ѧϰ��", u8"һ���ѧϰ��", ItemType::DOCUMENT, "C:/test3/Resources/images/environment/interactive items/study_du.png", "");

    // 物品二：通知书
    registerItem("do_not_know", u8"֪����", u8"һֽ֪����", ItemType::DOCUMENT, "C:/test3/Resources/images/environment/interactive items/do_not_know.png", "");

    // 物品三：一张海报
    registerItem("poster", u8"һ�ź���", u8"һ���ź���", ItemType::DOCUMENT, "C:/test3/Resources/images/environment/interactive items/poster.png", "");

    // 物品四：乌龙茶
    registerItem("oolong", u8"������", u8"һƿ������", ItemType::CONSUMABLE, "C:/test3/Resources/images/environment/interactive items/oolong.png", "");

    // 物品五：65472
    registerItem("65472", u8"65472", u8"һ���֤��", ItemType::KEY_ITEM, "C:/test3/Resources/images/environment/interactive items/65472.png", "");

    // 物品六：捐赠证书
    registerItem("donation_certificate", u8"���֤��", u8"һ������ʢ��", ItemType::DOCUMENT, "C:/test3/Resources/images/environment/interactive items/donation_certificate.png", "");

    // 物品七：安全帽
    registerItem("safety_helmet", u8"��ȫñ", u8"һ��ȫñ", ItemType::COLLECTIBLE, "C:/test3/Resources/images/environment/interactive items/safety_helmet.png", "");

    // 物品八：校园卡
    registerItem("campus_card", u8"У԰��", u8"һ��У԰��", ItemType::COLLECTIBLE, "C:/test3/Resources/images/environment/interactive items/campus_card.png", "");

    // 物品九：外卖
    registerItem("take_out", u8"����", u8"һ��ϴ����", ItemType::KEY_ITEM, "C:/test3/Resources/images/environment/interactive items/take_out.png", "");

    // 物品十：汉诺塔
    registerItem("hanoi_tower", u8"��ŵ��", u8"һ���ŵ��", ItemType::COLLECTIBLE, "C:/test3/Resources/images/environment/interactive items/hanoi_tower.png", "");

    // 物品十一：SJ
    registerItem("sj_larry", u8"SJ", u8"һ����", ItemType::KEY_ITEM, "C:/test3/Resources/images/environment/interactive items/sj_larry.png", "");

    // 物品十二：男人
    registerItem("man", u8"����", u8"һ���ͼƬ", ItemType::COLLECTIBLE, "C:/test3/Resources/images/environment/interactive items/man.png", "");

    // 物品十三：冰红茶
    registerItem("iced_black_tea", u8"�����", u8"һƿ������", ItemType::CONSUMABLE, "C:/test3/Resources/images/environment/interactive items/iced_black_tea.png", "");

    // 物品: what_the_hell (隐藏关卡触发物品)
    registerItem("what_the_hell", u8"what_the_hell", u8"一个神秘的物品", ItemType::KEY_ITEM, "C:/test3/Resources/images/environment/items/what_the_hell.png", "");

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
    return "Unknown Item";
}

std::string ItemManager::getItemDescription(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.description;
    }
    return "No description available";
}

ItemType ItemManager::getItemType(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.type;
    }
    return ItemType::NONE;
}

std::string ItemManager::getItemImagePath(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.imagePath;
    }
    return "";
}

std::string ItemManager::getItemAchievement(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.achievement;
    }
    return "";
}

bool ItemManager::isItemCollected(const std::string& itemId) const {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return it->second.isCollected;
    }
    return false;
}

void ItemManager::setItemCollected(const std::string& itemId, bool collected) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        it->second.isCollected = collected;
        saveItemStates();
    }
}

void ItemManager::saveItemStates() {
    // 保存物品收集状态到文件
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "item_states.dat";
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        for (const auto& pair : _itemDatabase) {
            outFile << pair.first << ":" << (pair.second.isCollected ? "1" : "0") << std::endl;
        }
        outFile.close();
    }
}

void ItemManager::loadItemStates() {
    // 从文件加载物品收集状态
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "item_states.dat";
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            size_t colonPos = line.find(":");
            if (colonPos != std::string::npos) {
                std::string itemId = line.substr(0, colonPos);
                bool isCollected = (line.substr(colonPos + 1) == "1");
                
                auto it = _itemDatabase.find(itemId);
                if (it != _itemDatabase.end()) {
                    it->second.isCollected = isCollected;
                }
            }
        }
        inFile.close();
    }
}

Item* ItemManager::createItem(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        Item* item = Item::create(itemId);
        return item;
    }
    return nullptr;
}

void ItemManager::collectItem(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end() && !it->second.isCollected) {
        it->second.isCollected = true;
        saveItemStates();
        
        // 播放收集音效
        AudioManager::getInstance()->playEffect("collect_item");
        
        // 检查成就条件
        checkAchievementConditions();
    }
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
    std::vector<std::string> collectedItems;
    for (const auto& pair : _itemDatabase) {
        if (pair.second.isCollected) {
            collectedItems.push_back(pair.first);
        }
    }
    return collectedItems;
}

ItemManager::ItemData* ItemManager::getItemData(const std::string& itemId) {
    auto it = _itemDatabase.find(itemId);
    if (it != _itemDatabase.end()) {
        return &(it->second);
    }
    return nullptr;
}

void ItemManager::checkAchievementConditions() {
    // 实现成就检查逻辑
    // 暂时留空，后续可以扩展
}
