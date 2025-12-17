#pragma once
#ifndef __DASH_BAR_H__
#define __DASH_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DashBar : public cocos2d::Node
{
public:
    static DashBar* create(int maxDashes, float rechargeTime);
    virtual bool init(int maxDashes, float rechargeTime);

    // 使用一次冲刺
    bool useDash();

    // 恢复一次冲刺
    void rechargeDash();

    // 立即恢复所有冲刺
    void rechargeAll();

    // 获取最大冲刺次数
    int getMaxDashes() const { return m_maxDashes; }

    // 获取充能时间
    float getRechargeTime() const { return m_rechargeTime; }

    // 设置新的最大冲刺次数
    void setMaxDashes(int maxDashes);

    // 设置新的充能时间
    void setRechargeTime(float rechargeTime);

    // 是否正在充能
    bool isRecharging() const { return m_isRecharging; }

    // 获取剩余充能时间
    float getRemainingRechargeTime() const;

    // 更新显示
    void updateDisplay();

    // 强制更新冲刺状态
    void forceUpdate();

    // 添加canDash方法
    bool canDash() const { return m_availableDashes > 0; }

    // 获取当前可用冲刺次数
    int getAvailableDashes() const { return m_availableDashes; }

private:
    void createDashBar();
    void createCells();
    void createRechargeProgress();
    void updateProgressDisplay();
    void startRechargeTimer();
    void onRechargeTimer(float dt);

private:
    std::vector<cocos2d::Sprite*> m_dashCells;           // 冲刺格子
    std::vector<cocos2d::ProgressTimer*> m_rechargeTimers; // 充能进度
    cocos2d::Label* m_dashText;                          // 文本显示

    int m_availableDashes;       // 当前可用冲刺次数
    int m_maxDashes;             // 最大冲刺次数
    float m_rechargeTime;        // 每格充能时间（秒）
    float m_currentRechargeTime; // 当前充能计时
    bool m_isRecharging;         // 是否正在充能

    // 格子样式参数
    float m_cellWidth;
    float m_cellHeight;
    float m_cellSpacing;
    cocos2d::Color3B m_fullCellColor;
    cocos2d::Color3B m_emptyCellColor;
    cocos2d::Color3B m_rechargingCellColor;
};

#endif // __DASH_BAR_H__