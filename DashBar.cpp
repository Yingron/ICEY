#include "DashBar.h"

using namespace cocos2d;

// 创建实例
DashBar* DashBar::create(int maxDashes, float rechargeTime)
{
    DashBar* pRet = new (std::nothrow) DashBar();
    if (pRet && pRet->init(maxDashes, rechargeTime))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// 初始化
bool DashBar::init(int maxDashes, float rechargeTime)
{
    if (!Node::init())
    {
        return false;
    }

    // 设置冲刺参数
    m_maxDashes = maxDashes;
    m_availableDashes = maxDashes;
    m_rechargeTime = rechargeTime;
    m_currentRechargeTime = 0.0f;
    m_isRecharging = false;

    // 默认格子样式
    m_cellWidth = 40.0f;
    m_cellHeight = 10.0f;
    m_cellSpacing = 5.0f;
    m_fullCellColor = Color3B(0, 200, 255);      // 亮青色
    m_emptyCellColor = Color3B(50, 100, 150);    // 暗青色
    m_rechargingCellColor = Color3B(255, 200, 0); // 充能中的黄色

    // 创建UI组件
    createDashBar();
    createCells();
    createRechargeProgress();

    // 初始显示
    updateDisplay();

    return true;
}

// 创建冲刺条UI
void DashBar::createDashBar()
{

    // 冲刺次数文本
    m_dashText = Label::createWithTTF("", "fonts/Marker Felt.ttf", 14);
    if (!m_dashText)
    {
        m_dashText = Label::createWithSystemFont("", "Arial", 14);
    }

    // 设置文本样式
    m_dashText->setColor(Color3B::WHITE);
    m_dashText->enableOutline(Color4B::BLACK, 1);


    // 添加组件到节点
    this->addChild(m_dashText, 2);

    // 设置位置
    m_dashText->setPosition(Vec2(-170, 0));
}

// 创建冲刺格子
void DashBar::createCells()
{
    // 清空现有的格子
    for (auto cell : m_dashCells)
    {
        if (cell)
        {
            cell->removeFromParent();
        }
    }
    m_dashCells.clear();

    // 计算总宽度
    float totalWidth = m_maxDashes * m_cellWidth + (m_maxDashes - 1) * m_cellSpacing;
    float startX = -totalWidth / 2.0f + m_cellWidth / 2.0f;

    // 创建每个冲刺格子
    for (int i = 0; i < m_maxDashes; i++)
    {
        // 创建格子背景
        Sprite* cell = Sprite::create();
        cell->setContentSize(Size(m_cellWidth, m_cellHeight));

        // 创建 DrawNode 用于显示颜色
        auto drawNode = DrawNode::create();
        drawNode->setTag(999); // 设置tag以便查找
        cell->addChild(drawNode);

        float xPos = startX + i * (m_cellWidth + m_cellSpacing);
        cell->setPosition(Vec2(xPos, 0.0f));

        // 添加到节点和容器
        this->addChild(cell);
        m_dashCells.push_back(cell);
    }

    // 初始绘制格子
    updateDisplay();
}


// 创建充能进度显示
void DashBar::createRechargeProgress()
{
    // 清空现有的进度条
    for (auto timer : m_rechargeTimers)
    {
        if (timer)
        {
            timer->removeFromParent();
        }
    }
    m_rechargeTimers.clear();

    // 计算总宽度
    float totalWidth = m_maxDashes * m_cellWidth +
        (m_maxDashes - 1) * m_cellSpacing;
    float startX = -totalWidth / 2.0f + m_cellWidth / 2.0f;

    // 为每个格子创建进度条
    for (int i = 0; i < m_maxDashes; i++)
    {
        // 创建进度条精灵
        auto progressSprite = Sprite::create();
            auto drawNode = DrawNode::create();

            float halfWidth = m_cellWidth / 2;
            float halfHeight = m_cellHeight / 2;

            drawNode->drawSolidRect(
                Vec2(-halfWidth, -halfHeight),
                Vec2(halfWidth, halfHeight),
                Color4F(0.2f, 0.2f, 0.2f, 0.7f)
            );
            progressSprite->addChild(drawNode);
            progressSprite->setContentSize(Size(m_cellWidth, m_cellHeight));

        // 创建进度计时器
        auto progressTimer = ProgressTimer::create(progressSprite);
        progressTimer->setType(ProgressTimer::Type::RADIAL);
        progressTimer->setPercentage(0);
        progressTimer->setVisible(false); // 初始隐藏

        float xPos = startX + i * (m_cellWidth + m_cellSpacing);
        progressTimer->setPosition(Vec2(xPos, 0.0f));

        // 添加到节点和容器
        this->addChild(progressTimer, 10); // 高层级确保显示在前
        m_rechargeTimers.push_back(progressTimer);
    }
}

// 使用一次冲刺
bool DashBar::useDash()
{
    if (m_availableDashes <= 0)
    {
        return false; // 没有可用的冲刺
    }

    m_availableDashes--;
    log("Dash used. Remaining dashes: %d/%d", m_availableDashes, m_maxDashes);

    // 更新显示
    updateDisplay();

    // 如果还有未满的格子，开始充能计时
    if (m_availableDashes < m_maxDashes && !m_isRecharging)
    {
        startRechargeTimer();
    }

    return true;
}

// 恢复一次冲刺
void DashBar::rechargeDash()
{
    if (m_availableDashes >= m_maxDashes)
    {
        return; // 已经满了
    }

    m_availableDashes++;
    log("Dash recharged. Available dashes: %d/%d", m_availableDashes, m_maxDashes);

    // 更新显示
    updateDisplay();

    // 如果还没满，继续充能
    if (m_availableDashes < m_maxDashes)
    {
        startRechargeTimer();
    }
    else
    {
        // 满了就停止充能
        m_isRecharging = false;
        m_currentRechargeTime = 0.0f;
        this->unschedule(schedule_selector(DashBar::onRechargeTimer));
    }
}

// 立即恢复所有冲刺
void DashBar::rechargeAll()
{
    if (m_availableDashes == m_maxDashes)
    {
        return;
    }

    m_availableDashes = m_maxDashes;
    m_isRecharging = false;
    m_currentRechargeTime = 0.0f;

    // 停止计时器
    this->unschedule(schedule_selector(DashBar::onRechargeTimer));

    // 更新显示
    updateDisplay();

    log("All dashes recharged: %d/%d", m_availableDashes, m_maxDashes);
}

// 设置新的最大冲刺次数
void DashBar::setMaxDashes(int maxDashes)
{
    if (maxDashes <= 0) return;

    // 保存当前可用冲刺次数（不超过新的最大值）
    if (m_availableDashes > maxDashes)
    {
        m_availableDashes = maxDashes;
    }

    m_maxDashes = maxDashes;

    // 重新创建UI
    createCells();
    createRechargeProgress();

    // 更新显示
    updateDisplay();
}

// 设置新的充能时间
void DashBar::setRechargeTime(float rechargeTime)
{
    if (rechargeTime <= 0) return;

    m_rechargeTime = rechargeTime;

    // 如果正在充能，重新计算进度
    if (m_isRecharging)
    {
        startRechargeTimer();
    }
}

// 获取剩余充能时间
float DashBar::getRemainingRechargeTime() const
{
    if (!m_isRecharging) return 0.0f;

    float elapsedTime = m_currentRechargeTime;
    return m_rechargeTime - elapsedTime;
}

// 更新显示
void DashBar::updateDisplay()
{
    // 更新所有格子的颜色
    for (int i = 0; i < m_dashCells.size(); i++)
    {
        auto cell = m_dashCells[i];
        auto drawNode = dynamic_cast<DrawNode*>(cell->getChildByTag(999));

        if (!drawNode) continue;

        drawNode->clear();

        float halfWidth = m_cellWidth / 2;
        float halfHeight = m_cellHeight / 2;

        Vec2 fillVertices[] = {
            Vec2(-halfWidth, -halfHeight),
            Vec2(-halfWidth, halfHeight),
            Vec2(halfWidth, halfHeight),
            Vec2(halfWidth, -halfHeight)
        };

        // 根据冲刺状态设置颜色
        Color4F cellColor;
        if (i < m_availableDashes)
        {
            // 有冲刺 - 亮青色
            cellColor = Color4F(0.0f, 0.8f, 1.0f, 1.0f);
        }
        else
        {
            // 无冲刺 - 暗青色
            cellColor = Color4F(0.2f, 0.4f, 0.6f, 1.0f);
        }

        // 绘制填充
        drawNode->drawSolidPoly(fillVertices, 4, cellColor);

        // 绘制边框
        drawNode->drawRect(
            Vec2(-halfWidth, -halfHeight),
            Vec2(halfWidth, halfHeight),
            Color4F(0.0f, 0.6f, 0.8f, 1.0f)
        );
    }

    // 更新文本显示
    std::string dashStr = StringUtils::format("%d/%d", m_availableDashes, m_maxDashes);
    if (m_dashText)
    {
        m_dashText->setString(dashStr);
    }

    // 更新进度条显示
    updateProgressDisplay();
}

// 更新进度条显示
void DashBar::updateProgressDisplay()
{
    // 隐藏所有进度条
    for (auto timer : m_rechargeTimers)
    {
        if (timer)
        {
            timer->setVisible(false);
        }
    }

    // 如果有正在充能的格子，显示进度条
    if (m_isRecharging && m_availableDashes < m_maxDashes)
    {
        // 计算进度百分比
        float progress = (m_currentRechargeTime / m_rechargeTime) * 100.0f;

        // 显示在下一个要充能的格子上
        int rechargingCellIndex = m_availableDashes; // 当前正在充能的是下一个格子

        if (rechargingCellIndex < m_rechargeTimers.size())
        {
            auto timer = m_rechargeTimers[rechargingCellIndex];
            timer->setPercentage(progress);
            timer->setVisible(true);
        }
    }
}

// 开始充能计时
void DashBar::startRechargeTimer()
{
    if (m_isRecharging)
    {
        return; // 已经在充能中
    }

    m_isRecharging = true;
    m_currentRechargeTime = 0.0f;

    // 每帧更新
    this->schedule(schedule_selector(DashBar::onRechargeTimer), 0.016f); // 约60FPS
}

// 充能计时器回调
void DashBar::onRechargeTimer(float dt)
{
    m_currentRechargeTime += dt;

    // 更新进度显示
    updateProgressDisplay();

    // 检查是否充能完成
    if (m_currentRechargeTime >= m_rechargeTime)
    {
        // 恢复一格冲刺
        m_currentRechargeTime = 0.0f;
        rechargeDash();
    }
}

// 强制更新冲刺状态
void DashBar::forceUpdate()
{
    updateDisplay();
}