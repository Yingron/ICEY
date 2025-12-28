#include "AchievementSystem.h"
#include "AudioManager.h"
#include "cocos2d.h"
#include <ctime>
#include <fstream>

USING_NS_CC;

#define ACHIEVEMENT_DATA_FILE "achievement_data.dat"

// 单例实例
AchievementSystem* AchievementSystem::getInstance() {
    static AchievementSystem instance;
    return &instance;
}

// 构造函数
AchievementSystem::AchievementSystem() : m_totalPoints(0) {
}

// 析构函数
AchievementSystem::~AchievementSystem() {
    saveData();
}

// 初始化
void AchievementSystem::init() {
    initAchievementList();
    loadData();
}

// 初始化成就列表
void AchievementSystem::initAchievementList() {
    if (!m_achievements.empty()) return;

    m_achievements = {
        // 成就一：辛苦你了，这么烂的游戏也能玩通关
        {
            "completionist", u8"辛苦你了", u8"通关游戏",
            Type::GAME_COMPLETED, Status::LOCKED,
            0, 1, 100, false, ""
        },

        // 成就二：当时，谁也不知道将来会发生什么
        {
            "choice_reality", u8"未知的将来", u8"选择重返现实",
            Type::FINAL_CHOICE_REALITY, Status::LOCKED,
            0, 1, 80, false, ""
        },

        // 成就三：人生最苦痛是梦醒了无路可走，做梦的人是幸福的
        {
            "choice_dream", u8"梦醒时分", u8"选择留在梦境",
            Type::FINAL_CHOICE_DREAM, Status::LOCKED,
            0, 1, 80, false, ""
        },

        // 成就四：它们都是为了忘却的纪念
        {
            "collector_all", u8"忘却的纪念", u8"找到所有物品",
            Type::COLLECT_ALL_ITEMS, Status::LOCKED,
            0, 8, 120, true, ""
        },

        // 成就五：我们对生的认识局限于死
        {
            "death_philosopher", u8"生与死", u8"死亡超过5次",
            Type::DEATH_COUNT, Status::LOCKED,
            0, 5, 60, false, ""
        },

        // 成就六：你的头顶怎么尖尖的
        {
            "top_toucher", u8"头顶尖尖", u8"碰到游戏上边界",
            Type::TOUCH_TOP_BOUNDARY, Status::LOCKED,
            0, 1, 40, false, ""
        },

        // 成就七：虚空索敌
        {
            "air_attacker", u8"虚空索敌", u8"攻击空气50次",
            Type::ATTACK_AIR, Status::LOCKED,
            0, 50, 70, false, ""
        },

        // 成就八：孤独的心就用孤独的心去感受
        {
            "narrator_confront", u8"孤独的心", u8"在与旁白的对峙中选择现实",
            Type::CONFRONT_NARRATOR, Status::LOCKED,
            0, 1, 90, true, ""
        },

        // 成就九：每个人都是疯子，只是疯的方式不一样
        {
            "narrator_annoyer", u8"疯狂的方式", u8"骚扰旁白累计100次",
            Type::ANNOY_NARRATOR, Status::LOCKED,
            0, 100, 110, true, ""
        },

        // 成就十：猴王出世
        {
            "first_launch", u8"猴王出世", u8"第一次打开游戏",
            Type::FIRST_LAUNCH, Status::LOCKED,
            0, 1, 10, false, ""
        }
    };
}

// ==================== 公共触发接口 ====================

void AchievementSystem::triggerGameCompleted() {
    updateProgress(Type::GAME_COMPLETED, 1);
}

void AchievementSystem::triggerFinalChoice(bool choseReality) {
    if (choseReality) {
        updateProgress(Type::FINAL_CHOICE_REALITY, 1);
    }
    else {
        updateProgress(Type::FINAL_CHOICE_DREAM, 1);
    }
}

void AchievementSystem::triggerItemCollected(int totalCollected) {
    if (totalCollected >= 8) {
        // 直接解锁收集所有物品成就
        for (auto& achievement : m_achievements) {
            if (achievement.id == "collector_all" && achievement.status == Status::LOCKED) {
                achievement.status = Status::UNLOCKED;
                achievement.currentProgress = achievement.targetValue;

                time_t now = time(nullptr);
                char buffer[80];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
                achievement.unlockDate = buffer;

                m_totalPoints += achievement.rewardPoints;
                showPopup(achievement);
                saveData();
                break;
            }
        }
    }
}

void AchievementSystem::triggerPlayerDeath() {
    updateProgress(Type::DEATH_COUNT, 1);
}

void AchievementSystem::triggerTouchTopBoundary() {
    updateProgress(Type::TOUCH_TOP_BOUNDARY, 1);
}

void AchievementSystem::triggerAttackInAir(bool hasEnemyNearby) {
    static int airAttackCount = 0;

    if (!hasEnemyNearby) {
        airAttackCount++;
        updateProgress(Type::ATTACK_AIR, 1);
    }
    else {
        airAttackCount = 0;
    }
}

void AchievementSystem::triggerConfrontNarrator(bool choseReality) {
    if (choseReality) {
        updateProgress(Type::CONFRONT_NARRATOR, 1);
    }
}

void AchievementSystem::triggerAnnoyNarrator() {
    updateProgress(Type::ANNOY_NARRATOR, 1);
}

void AchievementSystem::triggerFirstLaunch() {
    updateProgress(Type::FIRST_LAUNCH, 1);
}

// ==================== 核心逻辑 ====================

void AchievementSystem::updateProgress(Type type, int value) {
    // 更新进度
    m_progressMap[type] += value;

    // 检查相关成就
    for (auto& achievement : m_achievements) {
        if (achievement.status == Status::LOCKED && achievement.type == type) {
            achievement.currentProgress = m_progressMap[type];
            checkAndUnlock(achievement);
        }
    }

    // 保存数据
    saveData();
}

void AchievementSystem::checkAndUnlock(Achievement& achievement) {
    if (achievement.currentProgress >= achievement.targetValue) {
        achievement.status = Status::UNLOCKED;

        // 记录解锁时间
        time_t now = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
        achievement.unlockDate = buffer;

        // 增加点数
        m_totalPoints += achievement.rewardPoints;

        // 记录日志
        CCLOG("成就解锁: %s (%d 点数)", achievement.title.c_str(), achievement.rewardPoints);

        Director::getInstance()->getScheduler()->schedule([this, achievement](float dt) {
            showPopup(achievement);
            log("弹窗显示: %s", achievement.title.c_str());
            }, this, 0, 0, 2.0f, false, "delay_popup_" + achievement.id);
    }
}

// ==================== 数据持久化 ====================

void AchievementSystem::saveData() {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + ACHIEVEMENT_DATA_FILE;

    std::string data;
    for (const auto& achievement : m_achievements) {
        data += achievement.id + "," +
            std::to_string(static_cast<int>(achievement.status)) + "," +
            std::to_string(achievement.currentProgress) + "," +
            achievement.unlockDate + "\n";
    }

    FILE* file = fopen(filePath.c_str(), "w");
    if (file) {
        fwrite(data.c_str(), 1, data.size(), file);
        fclose(file);
    }
}

void AchievementSystem::loadData() {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + ACHIEVEMENT_DATA_FILE;

    if (!FileUtils::getInstance()->isFileExist(filePath)) {
        return;
    }

    std::string content = FileUtils::getInstance()->getStringFromFile(filePath);
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        std::vector<std::string> parts;
        std::stringstream lineStream(line);
        std::string part;

        while (std::getline(lineStream, part, ',')) {
            parts.push_back(part);
        }

        if (parts.size() >= 4) {
            std::string id = parts[0];
            Status status = static_cast<Status>(std::stoi(parts[1]));
            int progress = std::stoi(parts[2]);
            std::string date = parts[3];

            for (auto& achievement : m_achievements) {
                if (achievement.id == id) {
                    achievement.status = status;
                    achievement.currentProgress = progress;
                    achievement.unlockDate = date;

                    // 更新总点数
                    if (status == Status::UNLOCKED) {
                        m_totalPoints += achievement.rewardPoints;
                    }

                    // 更新进度映射
                    m_progressMap[achievement.type] = std::max(
                        m_progressMap[achievement.type],
                        achievement.currentProgress
                    );

                    break;
                }
            }
        }
    }
}

// ==================== 辅助函数 ====================

int AchievementSystem::getUnlockedCount() const {
    int count = 0;
    for (const auto& achievement : m_achievements) {
        if (achievement.status == Status::UNLOCKED) {
            count++;
        }
    }
    return count;
}


void AchievementSystem::showPopup(const Achievement& achievement) {
    AudioManager::getInstance()->playEffect("achievement");

    log("=== showPopup 开始 ===");

    auto scene = Director::getInstance()->getRunningScene();
    if (!scene) {
        log("ERROR: 没有运行中的场景");
        return;
    }
    log("场景获取成功");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    log("可见区域: %.0fx%.0f, 原点: (%.0f,%.0f)",
        visibleSize.width, visibleSize.height, origin.x, origin.y);

    // 创建简单的弹窗
    auto popup = LayerColor::create(Color4B(0, 0, 0, 0));
    if (!popup) {
        log("ERROR: 创建popup失败");
        return;
    }
    popup->setContentSize(visibleSize);
    popup->setPosition(origin);
    log("popup创建成功，尺寸: %.0fx%.0f", visibleSize.width, visibleSize.height);

    // 背景框 - 尺寸：300x100
    auto bg = LayerColor::create(Color4B(30, 30, 50, 220));
    if (!bg) {
        log("ERROR: 创建bg失败");
        return;
    }
    bg->setContentSize(Size(300, 100));  // 高度从150改为100

    // 计算位置：放在右下角
    float bgX = visibleSize.width - 300 - 10 + origin.x;  // 右边留10像素边距
    float bgY = 10 + origin.y;                           // 底部留10像素边距
    bg->setPosition(Vec2(bgX, bgY));
    log("bg位置(右下角): (%.0f,%.0f)", bgX, bgY);

    popup->addChild(bg);





    // 标题 - 字体大小调整为20，位置调整
    auto title = Label::createWithTTF(achievement.title, "fonts/forui2.ttf", 24); 
    if (!title) {
        log("WARNING: 创建title失败，尝试其他字体");
        title = Label::createWithSystemFont(achievement.title, "Arial", 24);  
    }
    if (title) {
        title->setTextColor(Color4B::YELLOW);
        title->setPosition(Vec2(150, 70)); // 从110调整到70（因为总高度100）
        title->setDimensions(280, 25);     // 限制高度减小
        title->setHorizontalAlignment(TextHAlignment::CENTER);
        bg->addChild(title);
        log("title创建成功: %s", achievement.title.c_str());
    }

    // 描述 - 字体大小调整为14，位置调整，使用forui2.ttf
    auto desc = Label::createWithTTF(achievement.description, "fonts/forui2.ttf", 18); 
    if (!desc) {
        log("WARNING: 创建desc失败");
        desc = Label::createWithSystemFont(achievement.description, "Arial", 18); 
    }
    if (desc) {
        desc->setTextColor(Color4B::WHITE);
        desc->setPosition(Vec2(150, 40)); // 从70调整到40
        desc->setDimensions(280, 40);     // 限制高度减小
        desc->setHorizontalAlignment(TextHAlignment::CENTER);
        desc->setLineBreakWithoutSpace(true);  // 允许中文自动换行
        bg->addChild(desc);
        log("desc创建成功: %s", achievement.description.c_str());
    }

    // 隐藏成就标记 - 字体改小，位置调整，使用forui2.ttf
    if (achievement.isSecret) {
        auto secret = Label::createWithTTF("隐藏成就", "fonts/forui2.ttf", 12); 
        if (!secret) {
            secret = Label::createWithSystemFont("隐藏成就", "Arial", 12);  
        }
        if (secret) {
            secret->setTextColor(Color4B::ORANGE);
            secret->setPosition(Vec2(280, 85));  // 从135调整到85
            bg->addChild(secret);
            log("添加隐藏成就标记");
        }
    }



    // 添加到场景 - 使用最高的z-order
    int newZOrder = 999999; // 确保比所有现有节点都高
    scene->addChild(popup, newZOrder);
    log("弹窗已添加到场景，z-order: %d", newZOrder);

    // 添加进入动画
    bg->setScale(0.8f);
    bg->setOpacity(0);

    auto scaleIn = ScaleTo::create(0.3f, 1.0f);
    auto fadeIn = FadeIn::create(0.3f);
    auto easeBack = EaseBackOut::create(scaleIn);
    auto spawn = Spawn::create(easeBack, fadeIn, nullptr);
    bg->runAction(spawn);

    // 1秒后开始淡出动画
    auto sequence = Sequence::create(
        DelayTime::create(1.0f),
        CallFunc::create([popup, bg]() {
            log("开始淡出动画");

            // 同时淡出 popup 和 bg
            auto popupFadeOut = FadeOut::create(0.3f);
            auto bgFadeOut = FadeOut::create(0.3f);
            auto bgScaleOut = ScaleTo::create(0.3f, 0.8f);
            auto bgSpawn = Spawn::create(bgFadeOut, bgScaleOut, nullptr);

            popup->runAction(popupFadeOut);
            bg->runAction(bgSpawn);
            }),
        DelayTime::create(0.3f), // 等待淡出动画完成
        CallFunc::create([popup]() {
            log("移除弹窗");
            popup->removeFromParent();
            }),
        nullptr
    );

    popup->runAction(sequence);

    log("=== showPopup 结束 ===");
}