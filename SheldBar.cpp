#include "SheldBar.h"
//改变护盾 SheldBar::setSheld(int sheld)

using namespace cocos2d;
using namespace ui;

// 创建护盾条实例
SheldBar* SheldBar::create(int maxSheld)
{
    SheldBar* pRet = new (std::nothrow) SheldBar();
    if (pRet && pRet->init(maxSheld))
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

// 初始化护盾条
bool SheldBar::init(int maxSheld)
{
    if (!Node::init())
    {
        return false;
    }

    // 设置最大护盾格数
    m_maxSheld = maxSheld;
    m_currentSheld = maxSheld;

    // 默认格子样式
    m_cellWidth = 40.0f*3/ maxSheld;
    m_cellHeight = 4.0f;
    m_cellSpacing = 5.0f;
    m_fullCellColor = Color3B(100, 150, 255);   // 亮蓝色
    m_emptyCellColor = Color3B(60, 60, 120);    // 暗蓝色

    // 创建护盾条组件
    createSheldBar();

    // 创建护盾格子
    createCells();

    // 初始显示
    updateDisplay();

    return true;
}

// 创建护盾条UI组件
void SheldBar::createSheldBar()
{
    // 护盾图标
    m_sheldIcon = Sprite::create("sheld_icon.png");
    if (!m_sheldIcon)
    {
        // 如果图标不存在，创建简单的盾牌形状
        m_sheldIcon = Sprite::create();
        auto drawNode = DrawNode::create();
        // 绘制简单的盾牌形状
        Vec2 vertices[] =
        {
            Vec2(-10, 0),
            Vec2(0, 15),
            Vec2(10, 0),
            Vec2(0, -10)
        };
        drawNode->drawPolygon(vertices, 4, Color4F(0.4f, 0.6f, 1.0f, 1.0f), 1, Color4F(0.2f, 0.4f, 0.8f, 1.0f));
        m_sheldIcon->addChild(drawNode);
        m_sheldIcon->setContentSize(Size(20, 25));
    }

    // 护盾值文本显示
    m_sheldText = Label::createWithTTF("", "fonts/Marker Felt.ttf", 16);
    if (!m_sheldText)
    {
        m_sheldText = Label::createWithSystemFont("", "Arial", 16);
    }

    // 设置文本颜色
    m_sheldText->setColor(Color3B::WHITE);
    m_sheldText->enableOutline(Color4B::BLACK, 2);

    // 设置图标大小
    m_sheldIcon->setScale(0.8f);

    // 添加组件到节点
    this->addChild(m_sheldIcon, 1);
    this->addChild(m_sheldText, 2);

    // 设置位置（图标在左，文本在右）
    m_sheldIcon->setPosition(Vec2(-170, 0));
    m_sheldText->setPosition(Vec2(35, 0));
}

// 创建护盾格子
void SheldBar::createCells()
{
    // 清空现有的格子
    for (auto cell : m_sheldCells)
    {
        if (cell)
        {
            cell->removeFromParent();
        }
    }
    m_sheldCells.clear();

    // 计算总宽度
    float totalWidth = m_maxSheld * m_cellWidth +
        (m_maxSheld - 1) * m_cellSpacing;
    float startX = -totalWidth / 2.0f + m_cellWidth / 2.0f - 70;

    // 创建每个护盾格子
    for (int i = 0; i < m_maxSheld; i++)
    {
        Sprite* cell = Sprite::create("sheld_cell.png");
        if (!cell)
        {
            // 如果图片不存在，创建矩形格子
            cell = Sprite::create();
            auto drawNode = DrawNode::create();

            // 绘制格子边框和填充
            float halfWidth = m_cellWidth / 2;
            float halfHeight = m_cellHeight / 2;

            // 绘制填充
            Vec2 fillVertices[] = {
                Vec2(-halfWidth, -halfHeight),
                Vec2(-halfWidth, halfHeight),
                Vec2(halfWidth, halfHeight),
                Vec2(halfWidth, -halfHeight)
            };
            drawNode->drawSolidPoly(fillVertices, 4, Color4F(0.4f, 0.6f, 1.0f, 1.0f));

            // 绘制边框
            drawNode->drawRect(
                Vec2(-halfWidth, -halfHeight),
                Vec2(halfWidth, halfHeight),
                Color4F(0.2f, 0.4f, 0.8f, 1.0f)
            );

            cell->addChild(drawNode);
            cell->setContentSize(Size(m_cellWidth, m_cellHeight));
        }
        else
        {
            // 使用图片时调整大小
            cell->setContentSize(Size(m_cellWidth, m_cellHeight));
        }

        float xPos = startX + i * (m_cellWidth + m_cellSpacing);
        cell->setPosition(Vec2(xPos, 0.0f));

        // 添加到节点和容器
        this->addChild(cell);
        m_sheldCells.push_back(cell);
    }
}

// 设置当前护盾格数
void SheldBar::setSheld(int sheld)
{
    m_currentSheld = sheld;

    // 限制护盾格数范围
    if (m_currentSheld < 0)
    {
        m_currentSheld = 0;
    }
    else if (m_currentSheld > m_maxSheld)
    {
        m_currentSheld = m_maxSheld;
    }

    updateDisplay();
}

// 更新护盾条显示
void SheldBar::updateDisplay()
{
    for (int i = 0; i < m_maxSheld; i++)
    {
        if (i < m_sheldCells.size())
        {
            auto cell = m_sheldCells[i];

            // 有护盾的格子显示，没有护盾的格子隐藏
            cell->setVisible(i < m_currentSheld);
        }
    }

    // 2. 更新文本显示（格数格式）
    std::string sheldStr = StringUtils::format("%d/%d", m_currentSheld, m_maxSheld);
    m_sheldText->setString(sheldStr);
}
