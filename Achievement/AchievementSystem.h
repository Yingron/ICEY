#ifndef ACHIEVEMENT_SYSTEM_H
#define ACHIEVEMENT_SYSTEM_H

#include "cocos2d.h"
#include <vector>
#include <map>
#include <string>

USING_NS_CC;

class AchievementSystem {
public:
    // 单例模式
    static AchievementSystem* getInstance();

    // 成就类型
    enum class Type {
        GAME_COMPLETED,           // 通关
        FINAL_CHOICE_REALITY,     // 选择现实
        FINAL_CHOICE_DREAM,       // 选择梦境
        COLLECT_ALL_ITEMS,        // 收集所有物品
        DEATH_COUNT,              // 死亡次数
        TOUCH_TOP_BOUNDARY,       // 碰到上边界
        ATTACK_AIR,               // 攻击空气
        CONFRONT_NARRATOR,        // 对峙旁白
        ANNOY_NARRATOR,           // 骚扰旁白
        FIRST_LAUNCH              // 首次启动
    };

    // 成就状态
    enum class Status {
        LOCKED,
        UNLOCKED
    };

    // 成就数据
    struct Achievement {
        std::string id;
        std::string title;
        std::string description;
        Type type;
        Status status;
        int currentProgress;
        int targetValue;
        int rewardPoints;
        bool isSecret;
        std::string unlockDate;
    };

    // 初始化成就系统
    void init();

    // 触发成就（简化接口）
    void triggerGameCompleted();                     // 通关
    void triggerFinalChoice(bool choseReality);      // 最终选择
    void triggerItemCollected(int totalCollected);   // 收集物品
    void triggerPlayerDeath();                       // 死亡
    void triggerTouchTopBoundary();                  // 碰到上边界
    void triggerAttackInAir(bool hasEnemyNearby);    // 攻击空气
    void triggerConfrontNarrator(bool choseReality); // 对峙旁白
    void triggerAnnoyNarrator();                     // 骚扰旁白
    void triggerFirstLaunch();                       // 首次启动

    // 获取成就数据
    const std::vector<Achievement>& getAchievements() const { return m_achievements; }
    int getTotalPoints() const { return m_totalPoints; }
    int getUnlockedCount() const;

private:
    AchievementSystem();
    ~AchievementSystem();

    // 初始化成就列表
    void initAchievementList();

    // 更新成就进度
    void updateProgress(Type type, int value);

    // 检查并解锁成就
    void checkAndUnlock(Achievement& achievement);

    // 保存/加载数据
    void saveData();
    void loadData();

    // 显示成就弹窗（可选，可注释掉）
    void showPopup(const Achievement& achievement);

private:
    std::vector<Achievement> m_achievements;
    std::map<Type, int> m_progressMap;
    int m_totalPoints;
};

#endif // ACHIEVEMENT_SYSTEM_H