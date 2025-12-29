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
    // 首先定义ItemData结构体
    struct ItemData {
        std::string name;
        std::string description;
        ItemType type;
        std::string imagePath;
        std::string achievement;
        bool isCollected;
    };

    // 获取物品信息
    std::string getItemName(const std::string& itemId) const;
    std::string getItemDescription(const std::string& itemId) const;
    ItemType getItemType(const std::string& itemId) const;

    // 获取物品数据（用于详细显示） - 这里声明！
    ItemData* getItemData(const std::string& itemId);

    static ItemManager* getInstance();

    // 物品管理
    void registerItem(const std::string& itemId, const std::string& name,
        const std::string& description, ItemType type,
        const std::string& imagePath, const std::string& achievement = "");

    Item* createItem(const std::string& itemId);
    void collectItem(const std::string& itemId);

    // 查询功能
    bool isItemCollected(const std::string& itemId) const;
    int getCollectedCount() const;
    int getTotalItemCount() const;

    // 获取所有已收集物品
    std::vector<std::string> getCollectedItems() const;

    // 保存/加载物品状态
    void saveItemStates();
    void loadItemStates();

    // 检查成就条件
    void checkAchievementConditions();

private:
    ItemManager();
    ~ItemManager() = default;

    static ItemManager* _instance;

    std::unordered_map<std::string, ItemData> _itemDatabase;

    // 初始化所有物品数据
    void initAllItems();
};

#endif // ITEM_MANAGER_H