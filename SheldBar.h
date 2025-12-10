#ifndef __SHELD_BAR_H__
#define __SHELD_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SheldBar : public cocos2d::Node
{
private:
    // 使用格子显示
    std::vector<cocos2d::Sprite*> m_sheldCells;  // 护盾格子
    cocos2d::Sprite* m_sheldIcon;                // 护盾图标
    cocos2d::Label* m_sheldText;                 // 文本显示

    int m_currentSheld;      // 当前护盾格数
    int m_maxSheld;          // 最大护盾格数

    // 格子显示相关
    float m_cellWidth;       // 每个格子的宽度
    float m_cellHeight;      // 每个格子的高度
    float m_cellSpacing;     // 格子间距
    cocos2d::Color3B m_fullCellColor;   // 满格颜色
    cocos2d::Color3B m_emptyCellColor;  // 空格颜色

public:
    static SheldBar* create(int maxSheld);
    virtual bool init(int maxSheld);

    // 设置当前护盾格数
    void setSheld(int sheld);

    void setMaxSheld(int maxSheld);

    // 获取当前护盾格数
    int getCurrentSheld() const { return m_currentSheld; }

    // 获取最大护盾格数
    int getMaxSheld() const { return m_maxSheld; }


private:
    void createSheldBar();
    void createCells();
    void updateDisplay();
};

#endif // __SHELD_BAR_H__