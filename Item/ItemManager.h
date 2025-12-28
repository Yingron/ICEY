// ItemManager.h
#pragma once
#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "cocos2d.h"
#include "Item.h"
#include <vector>
#include <unordered_map>

class ItemManager {
public:
    // 锟斤拷锟饺讹拷锟斤拷ItemData锟结构锟斤拷
    struct ItemData {
        std::string name;
        std::string description;
        ItemType type;
        std::string imagePath;
        std::string achievement;
        bool isCollected;
    };

    // 锟斤拷取锟斤拷品锟斤拷息
    std::string getItemName(const std::string& itemId) const;
    std::string getItemDescription(const std::string& itemId) const;
    ItemType getItemType(const std::string& itemId) const;
    std::string getItemImagePath(const std::string& itemId) const;
    std::string getItemAchievement(const std::string& itemId) const;

    // 锟斤拷取锟斤拷品锟斤拷锟捷ｏ拷锟斤拷锟斤拷锟斤拷细锟斤拷示锟斤拷 - 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    ItemData* getItemData(const std::string& itemId);

    static ItemManager* getInstance();

    // 锟斤拷品锟斤拷锟斤拷
    void registerItem(const std::string& itemId, const std::string& name,
        const std::string& description, ItemType type,
        const std::string& imagePath, const std::string& achievement = "");

    Item* createItem(const std::string& itemId);
    void collectItem(const std::string& itemId);

    // 鏌ヨ鏂规硶
    bool isItemCollected(const std::string& itemId) const;
    int getCollectedCount() const;
    int getTotalItemCount() const;
    
    // 璁剧疆鐗╁搧鏀堕泦鐘舵€?
    void setItemCollected(const std::string& itemId, bool collected);

    // 锟斤拷取锟斤拷锟斤拷锟斤拷锟秸硷拷锟斤拷品
    std::vector<std::string> getCollectedItems() const;

    // 锟斤拷锟斤拷/锟斤拷锟斤拷锟斤拷品状态
    void saveItemStates();
    void loadItemStates();

    // 锟斤拷锟缴撅拷锟斤拷锟斤拷
    void checkAchievementConditions();

private:
    ItemManager();
    ~ItemManager() = default;

    static ItemManager* _instance;

    std::unordered_map<std::string, ItemData> _itemDatabase;

    // 锟斤拷始锟斤拷锟斤拷锟斤拷锟斤拷品锟斤拷锟斤拷
    void initAllItems();
};

#endif // ITEM_MANAGER_H