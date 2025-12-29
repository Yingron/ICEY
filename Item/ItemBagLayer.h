#pragma once
#ifndef ITEM_BAG_LAYER_H
#define ITEM_BAG_LAYER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Item.h"
#include "AchievementSystem.h"
#include <vector>
static const float ROW_HEIGHT = 60.0f;      // 每行高度
static const float ROW_MARGIN = 10.0f;      // 行间距
static const float PADDING_X = 20.0f;       // 左右边距

class ItemBagLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ItemBagLayer);
    virtual bool init() override;

    // 显示/隐藏背包
    void showBag();
    void hideBag();
    void toggleBag();
    bool isBagVisible() const { return _isVisible; }

    // 刷新背包显示
    void refreshBag();

    // 更新进度标签
    void updateProgressLabel();

    // 键盘事件处理
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 设置显示模式：物品或成就
    void setDisplayMode(bool showItems) { _showItems = showItems; }

    // 新增：切换显示模式
    void switchToItems();
    void switchToAchievements();
    void toggleDisplayMode();

private:
    // 物品详情相关
    void showItemDetail(const std::string& itemId);
    void createItemDetailUI(const std::string& itemId);
    void hideItemDetail();

    // UI创建
    void createUI();
    void createItemGrid();
    void createAchievementGrid();
    void updateItemDisplay();

    // 新增：更新切换按钮文本
    void updateSwitchButtonText();

    // 成员变量
    cocos2d::ui::ScrollView* _scrollView;
    cocos2d::LayerColor* _background;

    std::vector<cocos2d::Sprite*> _itemIcons;
    std::vector<cocos2d::Label*> _itemLabels;

    cocos2d::Label* _titleLabel; // 当前显示的标题
    cocos2d::ui::Button* _switchButton; // 新增：切换按钮

    bool _isVisible;
    bool _showItems; // true: 显示物品, false: 显示成就
    float _bagScale;

    // 布局参数
    const int GRID_COLS = 4;
    const float ITEM_SIZE = 80.0f;
    const float ITEM_MARGIN = 20.0f;
    const float ACHIEVEMENT_SIZE = 70.0f;
    const float ACHIEVEMENT_MARGIN = 15.0f;
};

#endif // ITEM_BAG_LAYER_H