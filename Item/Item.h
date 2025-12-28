// Item.h
#pragma once
#ifndef ITEM_H
#define ITEM_H

#include "cocos2d.h"
#include <string>

enum class ItemType {
    COLLECTIBLE,    // 锟斤拷锟秸硷拷锟斤拷品
    KEY_ITEM,       // 锟截硷拷锟斤拷品
    CONSUMABLE,     // 锟斤拷锟斤拷品
    DOCUMENT,       // 锟侥硷拷/锟斤拷锟斤拷锟斤拷
    NONE            // 未知锟斤拷锟酵★拷锟斤拷状态
};

class Item : public cocos2d::Sprite {
public:
    static Item* create(const std::string& itemId);
    virtual bool init(const std::string& itemId);

    // 锟斤拷取锟斤拷品锟斤拷息
    std::string getItemId() const { return _itemId; }
    std::string getItemName() const { return _itemName; }
    std::string getDescription() const { return _description; }
    ItemType getItemType() const { return _itemType; }
    bool isCollected() const { return _isCollected; }

    // 锟秸硷拷锟斤拷品
    void collect();

    // 锟斤拷取锟缴就癸拷锟斤拷锟斤拷锟斤拷锟斤拷校锟?
    std::string getLinkedAchievement() const { return _linkedAchievement; }

    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
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

    // 锟斤拷锟斤拷锟斤拷锟斤拷
    float _worldPositionX;
    float _worldPositionY;

    // 锟斤拷始锟斤拷锟斤拷品锟斤拷锟斤拷
    void initItemData();
};

#endif // ITEM_H