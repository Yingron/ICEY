#include "ItemBagLayer.h"
#include "ItemManager.h"
#include "AudioManager.h"
#include "cocos2d.h"

USING_NS_CC;

bool ItemBagLayer::init()
{
    if (!Layer::init())
        return false;

    _isVisible = false;
    _bagScale = 0.8f;
    _showItems = true; // 默认显示物品

    createUI();

    // 初始隐藏背包
    this->setVisible(false);

    return true;
}

void ItemBagLayer::createUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建半透明背景
    _background = LayerColor::create(Color4B(0, 0, 0, 200));
    _background->setContentSize(visibleSize);
    _background->setPosition(origin);
    this->addChild(_background, 0);

    // 创建标题（根据显示模式动态更新）
    _titleLabel = Label::createWithTTF(u8"收 藏 品", "fonts/forui2.ttf", 36);
    if (!_titleLabel) {
        _titleLabel = Label::createWithSystemFont("COLLECTIONS", "", 36);
    }
    _titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.85f));
    _titleLabel->setTextColor(Color4B::YELLOW);
    _titleLabel->enableShadow(Color4B::BLACK, Size(2, -2));
    this->addChild(_titleLabel, 1);

    // 创建滚动视图（居中显示）
    _scrollView = ui::ScrollView::create();
    _scrollView->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    _scrollView->setPosition(Vec2(
        origin.x + visibleSize.width * 0.1f,
        origin.y + visibleSize.height * 0.15f
    ));
    _scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _scrollView->setBounceEnabled(true);
    _scrollView->setScrollBarEnabled(true);
    _scrollView->setScrollBarWidth(8);
    _scrollView->setScrollBarColor(Color3B::GRAY);
    this->addChild(_scrollView, 1);

    // 创建关闭提示
    auto closeHint = Label::createWithTTF(u8"按 Tab 键关闭背包", "fonts/forui2.ttf", 20);
    if (!closeHint) {
        closeHint = Label::createWithSystemFont("Press TAB to close bag", "", 20);
    }
    closeHint->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.08f));
    closeHint->setTextColor(Color4B::WHITE);
    closeHint->setOpacity(180);
    this->addChild(closeHint, 1);


    // 创建模式切换按钮（新增）
    _switchButton = ui::Button::create();
    _switchButton->setTitleFontName("fonts/forui2.ttf");
    _switchButton->setTitleFontSize(20);
    _switchButton->setTitleColor(Color3B::WHITE);
    _switchButton->setContentSize(Size(150, 40));
    _switchButton->setPosition(Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.08f));

    // 设置按钮背景颜色
    _switchButton->setColor(Color3B::ORANGE);

    _switchButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioManager::getInstance()->playUISound("ui_click");
            toggleDisplayMode();
        }
        });
    this->addChild(_switchButton, 1);


    // 创建收集进度标签
    auto progressLabel = Label::createWithTTF("", "fonts/forui2.ttf", 24);
    if (!progressLabel) {
        progressLabel = Label::createWithSystemFont("", "", 24);
    }
    progressLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.92f));
    progressLabel->setTextColor(Color4B::GREEN);
    progressLabel->setTag(100);
    this->addChild(progressLabel, 1);

    // 初始创建内容
    if (_showItems) {
        createItemGrid();
    }
    else {
        createAchievementGrid();
    }
    updateProgressLabel();
    updateSwitchButtonText(); // 初始更新按钮文本
}

// 添加切换显示模式的函数
void ItemBagLayer::toggleDisplayMode()
{
    _showItems = !_showItems;
    updateItemDisplay();
    updateProgressLabel();
    updateSwitchButtonText(); // 切换后更新按钮文本

    // 播放音效
    AudioManager::getInstance()->playUISound("ui_tab_switch");
}

// 添加更新切换按钮文本的函数
void ItemBagLayer::updateSwitchButtonText()
{
    if (_switchButton) {
        if (_showItems) {
            _switchButton->setTitleText(u8"查看成就");
        }
        else {
            _switchButton->setTitleText(u8"查看物品");
        }
    }
}


void ItemBagLayer::updateProgressLabel()
{
    auto progressLabel = dynamic_cast<Label*>(this->getChildByTag(100));
    if (!progressLabel) return;

    if (_showItems) {
        // 更新物品收集进度
        auto itemManager = ItemManager::getInstance();
        int collected = itemManager->getCollectedCount();
        int total = itemManager->getTotalItemCount();
        std::string progress = StringUtils::format(u8"物品: %d/%d", collected, total);

        // 更新标题
        _titleLabel->setString(u8"收 藏 品");

        auto chineseLabel = Label::createWithTTF(progress, "fonts/forui2.ttf", 24);
        if (!chineseLabel) {
            progressLabel->setString("Items: " + StringUtils::format("%d/%d", collected, total));
        }
        else {
            progressLabel->setString(chineseLabel->getString());
        }
        progressLabel->setTextColor(Color4B::GREEN);
    }
    else {
        // 更新成就进度
        auto achievementSystem = AchievementSystem::getInstance();
        int unlocked = achievementSystem->getUnlockedCount();
        int total = achievementSystem->getAchievements().size();
        std::string progress = StringUtils::format(u8"成就: %d/%d", unlocked, total);

        // 更新标题
        _titleLabel->setString(u8"成 就");

        auto chineseLabel = Label::createWithTTF(progress, "fonts/forui2.ttf", 24);
        if (!chineseLabel) {
            progressLabel->setString("Achievements: " + StringUtils::format("%d/%d", unlocked, total));
        }
        else {
            progressLabel->setString(chineseLabel->getString());
        }
        progressLabel->setTextColor(Color4B::ORANGE);
    }

    // 更新切换按钮文本
    updateSwitchButtonText();
}

void ItemBagLayer::createItemGrid()
{
    _scrollView->removeAllChildren();
    _itemIcons.clear();
    _itemLabels.clear();

    auto itemManager = ItemManager::getInstance();
    auto collectedItems = itemManager->getCollectedItems();

    // 计算内容宽度
    float containerWidth = _scrollView->getContentSize().width;
    float gridWidth = GRID_COLS * (ITEM_SIZE + ITEM_MARGIN) + ITEM_MARGIN;
    float startX = (containerWidth - gridWidth) / 2 + ITEM_SIZE / 2 + ITEM_MARGIN;

    // 如果没有任何物品被收集
    if (collectedItems.empty()) {
        auto emptyLabel = Label::createWithTTF(u8"背包空空如也，快去探索收集物品吧！", "fonts/forui2.ttf", 28);
        if (!emptyLabel) {
            emptyLabel = Label::createWithSystemFont("Bag is empty. Go explore and collect items!", "", 28);
        }
        emptyLabel->setPosition(Vec2(containerWidth / 2, _scrollView->getContentSize().height / 2));
        emptyLabel->setTextColor(Color4B::GRAY);
        _scrollView->addChild(emptyLabel);
        _scrollView->setInnerContainerSize(_scrollView->getContentSize());
        updateProgressLabel();
        return;
    }

    // 计算网格布局
    int rows = (int)std::ceil(collectedItems.size() / (float)GRID_COLS);
    float containerHeight = std::max(
        _scrollView->getContentSize().height,
        (ITEM_SIZE + ITEM_MARGIN) * rows + ITEM_MARGIN * 2
    );

    _scrollView->setInnerContainerSize(Size(containerWidth, containerHeight));

    // 创建每个收集到的物品的显示
    for (size_t i = 0; i < collectedItems.size(); i++) {
        const std::string& itemId = collectedItems[i];

        // 计算网格位置
        int row = i / GRID_COLS;
        int col = i % GRID_COLS;

        float x = startX + col * (ITEM_SIZE + ITEM_MARGIN);
        float y = containerHeight - ITEM_MARGIN - row * (ITEM_SIZE + ITEM_MARGIN) - ITEM_SIZE / 2;

        // 创建物品图标背景
        auto iconBg = LayerColor::create(Color4B(50, 50, 50, 200), ITEM_SIZE, ITEM_SIZE);
        iconBg->setPosition(Vec2(x - ITEM_SIZE / 2, y - ITEM_SIZE / 2));
        iconBg->setTag(static_cast<int>(i));
        _scrollView->addChild(iconBg);

        // 从ItemManager获取物品数据
        auto itemData = itemManager->getItemData(itemId);
        std::string itemName = itemData ? itemData->name : itemId;

        // 尝试加载物品图像
        Sprite* itemIcon = nullptr;
        auto fileUtils = FileUtils::getInstance();
        std::vector<std::string> possiblePaths = {
            itemId + ".png",
            "images/environment/items/" + itemId + ".png",
            "Resources/images/environment/items/" + itemId + ".png",
            "C:/aishi/test2/Resources/images/environment/items/" + itemId + ".png"
        };

        // 特殊处理某些物品
        if (itemId == "sj_larry") {
            possiblePaths.insert(possiblePaths.begin(), {
                "Larry.png",
                "images/environment/items/Larry.png",
                "Resources/images/environment/items/Larry.png"
                });
        }

        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                itemIcon = Sprite::create(path);
                if (itemIcon) {
                    break;
                }
            }
        }

        if (!itemIcon && itemData && !itemData->imagePath.empty()) {
            if (fileUtils->isFileExist(itemData->imagePath)) {
                itemIcon = Sprite::create(itemData->imagePath);
            }
        }

        if (itemIcon) {
            itemIcon->setPosition(Vec2(x, y));
            itemIcon->setScale(std::min(ITEM_SIZE / itemIcon->getContentSize().width,
                ITEM_SIZE / itemIcon->getContentSize().height) * 0.8f);
            _scrollView->addChild(itemIcon);
            _itemIcons.push_back(itemIcon);
        }
        else {
            auto placeholder = Sprite::create();
            placeholder->setTextureRect(Rect(0, 0, ITEM_SIZE * 0.7f, ITEM_SIZE * 0.7f));
            placeholder->setColor(Color3B::YELLOW);
            placeholder->setPosition(Vec2(x, y));
            _scrollView->addChild(placeholder);
            _itemIcons.push_back(placeholder);
            itemIcon = placeholder;
        }

        // 为物品图标添加触摸事件
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = [itemId, iconBg, itemIcon](Touch* touch, Event* event) -> bool {
            auto target = event->getCurrentTarget();
            Rect rect = target->getBoundingBox();
            auto parent = target->getParent();
            if (parent) {
                rect.origin = parent->convertToWorldSpace(rect.origin);
            }

            if (rect.containsPoint(touch->getLocation())) {
                iconBg->setColor(Color3B(100, 100, 100));
                if (itemIcon) {
                    itemIcon->setScale(itemIcon->getScale() * 1.05f);
                }
                return true;
            }
            return false;
            };

        touchListener->onTouchEnded = [itemId, iconBg, itemIcon, this](Touch* touch, Event* event) {
            iconBg->setColor(Color3B(50, 50, 50));
            if (itemIcon) {
                itemIcon->setScale(itemIcon->getScale() / 1.05f);
            }

            showItemDetail(itemId);
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, iconBg);

        // 创建物品名称标签
        auto nameLabel = Label::createWithTTF(itemName, "fonts/forui2.ttf", 16);
        if (!nameLabel) {
            nameLabel = Label::createWithSystemFont(itemName, "", 16);
        }
        nameLabel->setPosition(Vec2(x, y - ITEM_SIZE / 2 - 15));
        nameLabel->setTextColor(Color4B::WHITE);
        nameLabel->setAlignment(TextHAlignment::CENTER);
        nameLabel->setWidth(ITEM_SIZE + 20);
        _scrollView->addChild(nameLabel);
        _itemLabels.push_back(nameLabel);
    }

    updateProgressLabel();
}

void ItemBagLayer::createAchievementGrid()
{
    _scrollView->removeAllChildren();
    _itemIcons.clear();
    _itemLabels.clear();

    auto achievementSystem = AchievementSystem::getInstance();
    const auto& achievements = achievementSystem->getAchievements();

    float containerWidth = _scrollView->getContentSize().width;

    // 如果没有成就
    if (achievements.empty()) {
        auto emptyLabel = Label::createWithTTF(u8"尚无成就数据", "fonts/forui2.ttf", 28);
        if (!emptyLabel) {
            emptyLabel = Label::createWithSystemFont("No achievements yet", "", 28);
        }
        emptyLabel->setPosition(Vec2(containerWidth / 2, _scrollView->getContentSize().height / 2));
        emptyLabel->setTextColor(Color4B::GRAY);
        _scrollView->addChild(emptyLabel);
        _scrollView->setInnerContainerSize(_scrollView->getContentSize());
        return;
    }

    // 简单固定的行高和间距
    const float ROW_HEIGHT = 80.0f;
    const float ROW_MARGIN = 5.0f;
    const float PADDING_X = 15.0f;

    // 容器高度 = 每行高度 * 行数 + 边距
    // 关键修改：使用实际的成就数量来计算高度
    int totalRows = achievements.size();
    float containerHeight = ROW_MARGIN + (ROW_HEIGHT + ROW_MARGIN) * totalRows;

    // 确保容器高度至少为滚动视图的高度，这样如果成就少也能正常显示
    containerHeight = std::max(containerHeight, _scrollView->getContentSize().height * 1.2f);

    _scrollView->setInnerContainerSize(Size(containerWidth, containerHeight));

    // 直接从上往下创建，不搞复杂计算
    float currentY = containerHeight - ROW_MARGIN;

    for (size_t i = 0; i < achievements.size(); i++) {
        const auto& achievement = achievements[i];

        // 移动到下一行
        currentY -= ROW_HEIGHT;

        // 创建行背景
        Color4B bgColor = (achievement.status == AchievementSystem::Status::UNLOCKED) ?
            Color4B(50, 70, 90, 200) : Color4B(70, 70, 70, 200);

        auto rowBg = LayerColor::create(bgColor, containerWidth - PADDING_X * 2, ROW_HEIGHT);
        rowBg->setPosition(Vec2(PADDING_X, currentY));
        _scrollView->addChild(rowBg);

        // 准备显示的文本
        std::string displayText;
        Color4B textColor;

        if (achievement.status == AchievementSystem::Status::UNLOCKED) {
            if (achievement.isSecret) {
                displayText = u8"[隐藏] " + achievement.title + ": " + achievement.description;
            }
            else {
                displayText = achievement.title + ": " + achievement.description;
            }
            textColor = Color4B::YELLOW;
        }
        else {
            if (achievement.isSecret) {
                displayText = u8"？？？？？: ？？？？？？？？？？？";
            }
            else {
                displayText = achievement.title + u8": 未解锁";
            }
            textColor = Color4B::GRAY;
        }

        // 创建两个标签：一个显示成就名，一个显示描述
        // 1. 成就名标签
        auto titleText = (achievement.status == AchievementSystem::Status::UNLOCKED) ?
            achievement.title : (achievement.isSecret ? u8"？？？？？" : achievement.title + u8": 未解锁");

        auto titleLabel = Label::createWithTTF(titleText, "fonts/forui2.ttf", 22);
        if (!titleLabel) {
            titleLabel = Label::createWithSystemFont(titleText, "", 22);
        }
        titleLabel->setAnchorPoint(Vec2(0, 1));
        titleLabel->setPosition(Vec2(PADDING_X + 10, currentY + ROW_HEIGHT - 10));
        titleLabel->setTextColor(textColor);
        titleLabel->setWidth(containerWidth - PADDING_X * 2 - 20);
        titleLabel->setLineBreakWithoutSpace(true);
        _scrollView->addChild(titleLabel);
        _itemLabels.push_back(titleLabel);

        // 2. 成就描述标签（仅解锁时显示）
        if (achievement.status == AchievementSystem::Status::UNLOCKED) {
            auto descLabel = Label::createWithTTF(achievement.description, "fonts/forui2.ttf", 18);
            if (!descLabel) {
                descLabel = Label::createWithSystemFont(achievement.description, "", 18);
            }
            descLabel->setAnchorPoint(Vec2(0, 1));
            descLabel->setPosition(Vec2(PADDING_X + 10, currentY + ROW_HEIGHT - 50));
            descLabel->setTextColor(Color4B(220, 220, 220, 255));
            descLabel->setWidth(containerWidth - PADDING_X * 2 - 20);
            descLabel->setLineBreakWithoutSpace(true);
            _scrollView->addChild(descLabel);
            _itemLabels.push_back(descLabel);
        }

        // 分隔线
        currentY -= ROW_MARGIN;
        if (i < achievements.size() - 1) {
            auto separator = LayerColor::create(Color4B(100, 100, 100, 100), containerWidth - PADDING_X * 2, 1);
            separator->setPosition(Vec2(PADDING_X, currentY));
            _scrollView->addChild(separator);
        }
    }

    // 确保滚动到底部能看到所有内容
    if (containerHeight > _scrollView->getContentSize().height) {
        _scrollView->scrollToTop(0.1f, false);
    }

    updateProgressLabel();
}

// 修改updateItemDisplay函数：
void ItemBagLayer::updateItemDisplay()
{
    if (_showItems) {
        createItemGrid();
    }
    else {
        createAchievementGrid();
    }
}

void ItemBagLayer::refreshBag()
{
    updateItemDisplay();
    updateSwitchButtonText(); // 刷新时也更新按钮文本
}

// 添加外部切换函数（方便从其他地方调用）：
void ItemBagLayer::switchToItems()
{
    if (_showItems) return;

    _showItems = true;
    updateItemDisplay();
    updateProgressLabel();
    updateSwitchButtonText();

    // 播放音效
    AudioManager::getInstance()->playUISound("ui_tab_switch");
}

void ItemBagLayer::switchToAchievements()
{
    if (!_showItems) return;

    _showItems = false;
    updateItemDisplay();
    updateProgressLabel();
    updateSwitchButtonText();

    // 播放音效
    AudioManager::getInstance()->playUISound("ui_tab_switch");
}

void ItemBagLayer::showBag()
{
    if (_isVisible) return;

    refreshBag();
    this->setVisible(true);
    _isVisible = true;

    // 更新切换按钮文本
    updateSwitchButtonText();

    // 播放音效
    AudioManager::getInstance()->playUISound("ui_bag_open");

    // 动画效果
    this->setScale(0.5f);
    auto scaleAction = ScaleTo::create(0.2f, _bagScale);
    auto easeAction = EaseBackOut::create(scaleAction);
    this->runAction(easeAction);
}

void ItemBagLayer::hideBag()
{
    if (!_isVisible) return;

    // 动画效果
    auto scaleAction = ScaleTo::create(0.15f, 0.5f);
    auto fadeAction = FadeOut::create(0.15f);
    auto spawnAction = Spawn::create(scaleAction, fadeAction, nullptr);
    auto callback = CallFunc::create([this]() {
        this->setVisible(false);
        _isVisible = false;
        });

    auto sequence = Sequence::create(spawnAction, callback, nullptr);
    this->runAction(sequence);

    // 播放音效
    AudioManager::getInstance()->playUISound("ui_bag_close");
}

void ItemBagLayer::toggleBag()
{
    if (_isVisible) {
        hideBag();
    }
    else {
        showBag();
    }
}

void ItemBagLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_TAB) {
        toggleBag();
        event->stopPropagation();
    }
}

// 物品详情相关函数（保持原有实现，仅作微小调整）
void ItemBagLayer::showItemDetail(const std::string& itemId) {
    AudioManager::getInstance()->playUISound("ui_click");
    createItemDetailUI(itemId);
}

void ItemBagLayer::createItemDetailUI(const std::string& itemId) {
    if (this->getChildByTag(999)) {
        this->removeChildByTag(999);
    }

    auto itemManager = ItemManager::getInstance();
    auto itemData = itemManager->getItemData(itemId);

    if (!itemData) {
        log("ItemBagLayer: Cannot find item data for ID: %s", itemId.c_str());
        return;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto detailBg = LayerColor::create(Color4B(0, 0, 0, 180));
    detailBg->setContentSize(visibleSize);
    detailBg->setPosition(origin);
    detailBg->setTag(999);
    this->addChild(detailBg, 10);

    const float panelWidth = visibleSize.width * 0.7f;
    const float panelHeight = visibleSize.height * 0.8f;

    auto panel = LayerColor::create(Color4B(40, 40, 70, 240), panelWidth, panelHeight);
    panel->setPosition(Vec2(
        visibleSize.width / 2 - panelWidth / 2,
        visibleSize.height / 2 - panelHeight / 2
    ));
    panel->setCascadeOpacityEnabled(true);
    detailBg->addChild(panel);

    // 标题
    auto titleBg = LayerColor::create(Color4B(60, 60, 100, 255), panelWidth, 60);
    titleBg->setPosition(Vec2(0, panelHeight - 60));
    panel->addChild(titleBg);

    std::string itemName = itemData->name;
    auto nameLabel = Label::createWithTTF(itemName, "fonts/forui2.ttf", 36);
    if (!nameLabel) {
        nameLabel = Label::createWithSystemFont(itemName, "Arial", 36);
    }
    nameLabel->setPosition(Vec2(panelWidth / 2, panelHeight - 30));
    nameLabel->setTextColor(Color4B::YELLOW);
    nameLabel->enableShadow(Color4B::BLACK, Size(2, -2));
    nameLabel->setAlignment(TextHAlignment::CENTER);
    panel->addChild(nameLabel);

    // 图标区域
    const float iconAreaHeight = panelHeight * 0.25f;
    auto iconArea = LayerColor::create(Color4B(30, 30, 50, 100), panelWidth - 40, iconAreaHeight);
    iconArea->setPosition(Vec2(20, panelHeight - 100 - iconAreaHeight));
    panel->addChild(iconArea);

    // 加载图标
    Sprite* itemIcon = nullptr;
    auto fileUtils = FileUtils::getInstance();

    if (!itemData->imagePath.empty() && fileUtils->isFileExist(itemData->imagePath)) {
        itemIcon = Sprite::create(itemData->imagePath);
    }

    if (!itemIcon) {
        std::vector<std::string> possiblePaths = {
            "images/environment/items/" + itemId + ".png",
            "Resources/images/environment/items/" + itemId + ".png",
            itemId + ".png"
        };

        for (const auto& path : possiblePaths) {
            if (fileUtils->isFileExist(path)) {
                itemIcon = Sprite::create(path);
                if (itemIcon) break;
            }
        }
    }

    if (itemIcon) {
        float maxSize = std::min(iconAreaHeight * 0.7f, panelWidth * 0.25f);
        float scaleX = maxSize / itemIcon->getContentSize().width;
        float scaleY = maxSize / itemIcon->getContentSize().height;
        float scale = std::min(scaleX, scaleY) * 0.9f;

        itemIcon->setPosition(Vec2(panelWidth / 2, panelHeight - 100 - iconAreaHeight / 2));
        itemIcon->setScale(scale);
        panel->addChild(itemIcon);
    }
    else {
        itemIcon = Sprite::create();
        float placeholderSize = iconAreaHeight * 0.5f;
        itemIcon->setTextureRect(Rect(0, 0, placeholderSize, placeholderSize));
        itemIcon->setColor(Color3B::YELLOW);
        itemIcon->setPosition(Vec2(panelWidth / 2, panelHeight - 100 - iconAreaHeight / 2));
        panel->addChild(itemIcon);
    }

    // 类型信息
    std::string typeStr = "";
    switch (itemData->type) {
        case ItemType::COLLECTIBLE: typeStr = u8"收藏品"; break;
        case ItemType::KEY_ITEM: typeStr = u8"关键物品"; break;
        case ItemType::CONSUMABLE: typeStr = u8"消耗品"; break;
        case ItemType::DOCUMENT: typeStr = u8"文档"; break;
        default: typeStr = u8"未知类型"; break;
    }

    auto typeLabel = Label::createWithTTF(typeStr, "fonts/forui2.ttf", 22);
    if (!typeLabel) {
        typeLabel = Label::createWithSystemFont(typeStr, "Arial", 22);
    }
    typeLabel->setPosition(Vec2(panelWidth / 2, panelHeight - 100 - iconAreaHeight - 30));
    typeLabel->setTextColor(Color4B::GREEN);
    typeLabel->enableOutline(Color4B::BLACK, 1);
    panel->addChild(typeLabel);

    // 描述区域
    const float descAreaHeight = panelHeight * 0.4f;
    auto descArea = LayerColor::create(Color4B(20, 20, 40, 180), panelWidth - 40, descAreaHeight);
    descArea->setPosition(Vec2(20, 60));
    panel->addChild(descArea);

    std::string description = itemData->description;
    if (description.empty()) {
        Item* tempItem = ItemManager::getInstance()->createItem(itemId);
        if (tempItem) {
            description = tempItem->getDescription();
            tempItem->release();
        }
    }

    if (description.empty()) {
        description = "测试描述 - 物品ID: " + itemId;
    }

    auto descLabel = Label::createWithTTF(description, "fonts/forui2.ttf", 20);
    if (!descLabel) {
        descLabel = Label::createWithSystemFont(description, "Arial", 20);
    }

    descLabel->setTextColor(Color4B::WHITE);
    descLabel->setAlignment(TextHAlignment::LEFT);
    descLabel->setVerticalAlignment(TextVAlignment::TOP);
    descLabel->setDimensions(panelWidth - 60, descAreaHeight - 40);
    descLabel->setPosition(Vec2((panelWidth - 40) / 2, descAreaHeight - 20));
    descLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    descArea->addChild(descLabel);

    // 关闭按钮
    auto buttonArea = LayerColor::create(Color4B(60, 60, 100, 200), panelWidth, 50);
    buttonArea->setPosition(Vec2(0, 0));
    panel->addChild(buttonArea);

    auto closeButton = ui::Button::create();
    closeButton->setTitleText(u8"关闭");
    closeButton->setTitleFontName("fonts/forui2.ttf");
    closeButton->setTitleFontSize(24);
    closeButton->setTitleColor(Color3B::WHITE);
    closeButton->setContentSize(Size(120, 40));
    closeButton->setPosition(Vec2(panelWidth / 2, 25));

    closeButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioManager::getInstance()->playUISound("ui_click");
            hideItemDetail();
        }
        });

    panel->addChild(closeButton);

    // 添加触摸监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        return true;
        };
    touchListener->onTouchEnded = [this, panel](Touch* touch, Event* event) {
        auto panelRect = panel->getBoundingBox();
        if (!panelRect.containsPoint(touch->getLocation())) {
            hideItemDetail();
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, detailBg);

    // 动画效果
    detailBg->setOpacity(0);
    panel->setScale(0.5f);

    auto fadeIn = FadeIn::create(0.25f);
    detailBg->runAction(fadeIn);

    auto scaleTo = ScaleTo::create(0.25f, 1.0f);
    auto easeBack = EaseBackOut::create(scaleTo);
    panel->runAction(easeBack);

    AudioManager::getInstance()->playUISound("ui_popup");
}

void ItemBagLayer::hideItemDetail() {
    auto detailBg = this->getChildByTag(999);
    if (detailBg) {
        AudioManager::getInstance()->playUISound("ui_close");
        auto fadeOut = FadeOut::create(0.15f);
        auto remove = RemoveSelf::create();
        auto sequence = Sequence::create(fadeOut, remove, nullptr);
        detailBg->runAction(sequence);
    }
}