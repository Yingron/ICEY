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

// Item.cpp - �޸�init�����е�ͼƬ·��
bool Item::init(const std::string& itemId) {
    _itemId = itemId;
    _isCollected = false;

    // ��ʼ����������
    _worldPositionX = 0.0f;
    _worldPositionY = 0.0f;

    auto itemManager = ItemManager::getInstance();

    // �ȵ���initItemData������Ʒ��Ϣ
    initItemData();

    // ���Լ���ͼƬ
    auto fileUtils = FileUtils::getInstance();
    bool loaded = false;

    // ���Զ�����ܵ�·�� - ����Ϊ�µ�·��
    std::vector<std::string> possiblePaths = {
        itemId + ".png",
        "images/environment/items/" + itemId + ".png",
        "Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + itemId + ".png",
        "C:/aishi/test2/Resources/images/environment/items/" + _itemName + ".png"
    };

    // ���⴦��һЩ�ļ�����ͬ����Ʒ
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
    else if (_itemId == "what_the_hell") {
        possiblePaths.insert(possiblePaths.begin(), {
            "C:/test3/Resources/images/environment/items/what_the_hell.png"
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

        // ʹ��ռλ��
        if (!Sprite::init()) {
            return false;
        }
        this->setTextureRect(Rect(0, 0, 40, 40));
        this->setColor(Color3B::YELLOW);
    }

    // ����������������ײ���
    if (this->getContentSize().width > 0 && this->getContentSize().height > 0) {
        auto physicsBody = PhysicsBody::createBox(this->getContentSize());
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(0x02); // ��Ʒ���
        physicsBody->setCollisionBitmask(0x01); // �������ײ
        physicsBody->setContactTestBitmask(0x01);
        this->setPhysicsBody(physicsBody);
    }
    else {
        log("WARNING: Item has invalid size, not adding physics body");
    }

    // ���Ӹ�������
    auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
    auto moveDown = MoveBy::create(1.0f, Vec2(0, -10));
    auto sequence = Sequence::create(moveUp, moveDown, nullptr);
    this->runAction(RepeatForever::create(sequence));

    // ������ת����
    auto rotate = RotateBy::create(2.0f, 360);
    this->runAction(RepeatForever::create(rotate));

    return true;
}

// Item.cpp - ����initItemData����
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
        // ������Ʒ����
        if (_itemId == "study_du") {
            _itemName = "study du";
            _description = "��������ƭ�֣�������������ַ�����һ����������αװ��ѧϰ���������ڴ�ѧ���ֻ������Ϊ��ɫʥ��ñ��ñ����һ�����ǡ���������Ӱ��ѧ�����Ľ�������ȡ�������ϣ�Ӱ��ѧ��˯���������ճ�������һ�����������ع�������λ��Ϣǩ���룬��ȡ�����˽��ȫ������Ҫ����ָ����4λ��Ħ��˹���룬���߻�Ҫ���������˽ͼƬ�ϴ����ܳɹ�ǩ���������Ӱ�����й���ѧ�������Ľ��������з��֣����̶Դ��������оٱ���";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "do_not_know") {
            _itemName = "do_not_know";
            _description = "��16���ٲ������ν̡̳�";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "poster") {
            _itemName = "poster";
            _description = "�������ڶວ�����ŵĶ�ħ�����������������������ź������˻�û�б�������";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "oolong") {
            _itemName = "oolong";
            _description = "ˬ�ڵ����ϣ���˵��ѡ�κ��а���(��������Ǯ)��";
            _itemType = ItemType::CONSUMABLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "65472") {
            _itemName = "65472";
            _description = "同学的证明";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "";
        }
        else if (_itemId == "donation_certificate") {
            _itemName = "donation certificate";
            _description = "һ�����ȵĴ���ʢ�硣";
            _itemType = ItemType::DOCUMENT;
            _linkedAchievement = "";
        }
        else if (_itemId == "safety_helmet") {
            _itemName = "safety helmet";
            _description = "��ľѧ����ʥ�����ϡ�ɼ������ĻԻ͡�";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "campus_card") {
            _itemName = "campus card";
            _description = "�����������죬ֱ�������㣺��������Ū������";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "take_out") {
            _itemName = "take out";
            _description = "����ϭ��\n--�������Ӿ�����\n����Ǭ�������£����뺮��������!\n��������¥�·ţ�������Ъ�Ų�æ��\n��Ѱ�ſ�������������ãã�Բ�����\n����С�����ôˣ�Ϊ�β���������?\n����ż�ŵ�ʳ�ߣ���ȻһЦδ��Ȼ��\nͬΪѧԺʦ�ֵܣ�����һ�������ɣ�\n�����������������������길ĸ�ˣ�\n��֮����ֵ��Ǯ�������ι�ʳ����?\n��֮����ۼ�����ͬΪһУȴ����?\n��֮Т���м��壬��˽�常֮Ǯ��?\n����ѧ��֮���࣬�߾�˫��֮����\nΪ����ʱ��ѵ�㣬���Ը�������״��\n��˲���˫�������Ҷ����������\n��֪��ƶ����������ְ����ղ�ɢ\n����˭�������У�����ȥ���޴���\n����֪�������常XXX�����������̳���!\nȻ���ĪҪ����Ϊ�常�������д�������!";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "���˺��գ�------�����ҵ��ֻ�";
        }
        else if (_itemId == "hanoi_tower") {
            _itemName = "hanoi tower";
            _description = "������һЩ���õĻ���...";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "sj_larry") {
            _itemName = "SJ";
            _description = "ʱ�䣿���磿���ݣ��ֻ���˯������ƣ���٣�ʵ�ʣ�˾�������ǣ��Ͻ������ͣ����ң������˲�䣿�񾭣��ļ����̼ң���ƣ��������ϼܣ��Ӿ����鼮���������ռ���Larry?!";
            _itemType = ItemType::KEY_ITEM;
            _linkedAchievement = "";
        }
        else if (_itemId == "man") {
            _itemName = "man";
            _description = "��������...";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
        else if (_itemId == "iced_black_tea") {
            _itemName = "iced black tea";
            _description = "������6��13�����ϵ�8:63......";
            _itemType = ItemType::CONSUMABLE;
            _linkedAchievement = "";
        }
        else {
            // Ĭ��ֵ
            _itemName = _itemId;
            _description = "δ֪��Ʒ";
            _itemType = ItemType::COLLECTIBLE;
            _linkedAchievement = "";
        }
    } 
}

void Item::setWorldPosition(float worldX, float worldY) {
    _worldPositionX = worldX;
    _worldPositionY = worldY;
    // �������þ���λ�ã���Ҫ��MainGameScene�и��������ƫ�Ƶ�����
    this->setPosition(Vec2(worldX, worldY));
}

void Item::collect() {
    if (_isCollected) return;

    _isCollected = true;

    // �����ռ�����
    auto scaleUp = ScaleTo::create(0.2f, 1.5f);
    auto scaleDown = ScaleTo::create(0.2f, 0.1f);
    auto fadeOut = FadeOut::create(0.2f);
    auto remove = RemoveSelf::create();

    auto sequence = Sequence::create(scaleUp, scaleDown, fadeOut, remove, nullptr);
    this->runAction(sequence);

    // ֪ͨItemManager
    ItemManager::getInstance()->collectItem(_itemId);

    // ˢ�±�����ʾ����������Ѵ򿪣�
    auto scene = Director::getInstance()->getRunningScene();
    if (scene) {
        // Ѱ�ұ����㲢ˢ��
        scene->enumerateChildren("ItemBagLayer", [](Node* node) -> bool {
            auto bagLayer = dynamic_cast<ItemBagLayer*>(node);
            if (bagLayer && bagLayer->isBagVisible()) {
                bagLayer->refreshBag();
            }
            return false;
            });
    }

    // ��ʾ�ռ���ʾ
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithSystemFont("�ռ���: " + _itemName, "Arial", 24);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.8f));
    label->setColor(Color3B::YELLOW);

    scene->addChild(label, 100);

    // ��ʾ���ֶ���
    auto fadeIn = FadeIn::create(0.5f);
    auto delay = DelayTime::create(1.5f);
    auto fadeOutLabel = FadeOut::create(0.5f);
    auto removeLabel = RemoveSelf::create();
    auto labelSequence = Sequence::create(fadeIn, delay, fadeOutLabel, removeLabel, nullptr);
    label->runAction(labelSequence);
}