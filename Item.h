// Item.h
#pragma once
#ifndef ITEM_H
#define ITEM_H

#include "cocos2d.h"
#include <string>

enum class ItemType {
    COLLECTIBLE,    // 收集物品
    KEY_ITEM,       // 关键物品
    CONSUMABLE,     // 消耗品
    DOCUMENT        // 文档/道具
};

class Item : public cocos2d::Sprite {
public:
    static Item* create(const std::string& itemId);
    virtual bool init(const std::string& itemId);

    // 获取物品信息
    std::string getItemId() const { return _itemId; }
    std::string getItemName() const { return _itemName; }
    std::string getDescription() const { return _description; }
    ItemType getItemType() const { return _itemType; }
    bool isCollected() const { return _isCollected; }

    // 收集物品
    void collect();

    // 获取关联成就（如果有）
    std::string getLinkedAchievement() const { return _linkedAchievement; }

    // 世界位置设置
    void setWorldPosition(float worldX, float worldY);
    float getWorldPositionX() const { return _worldPositionX; }
    float getWorldPositionY() const { return _worldPositionY; }

private:
    std::string _itemId;
    std::string _itemName;
    std::string _description;
    ItemType _itemType;
    bool _isCollected;
    std::string _linkedAchievement;

    // 世界位置
    float _worldPositionX;
    float _worldPositionY;

    // 初始化物品数据
    void initItemData();
};

#endif // ITEM_H