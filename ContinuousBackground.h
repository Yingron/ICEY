#pragma once
#ifndef CONTINUOUSBACKGROUND_H
#define CONTINUOUSBACKGROUND_H

#include "cocos2d.h"

class ContinuousBackground : public cocos2d::Node {
public:
    CREATE_FUNC(ContinuousBackground);

    // 创建指定图片序列的背景
    static ContinuousBackground* createWithImageSequence(const std::vector<std::string>& imageFiles);

    virtual bool init();
    bool initWithImages(const std::vector<std::string>& imageFiles);

    // 根据摄像机偏移更新背景位置
    void updateWithCameraOffset(float cameraOffsetX);

    // 手动设置滚动速度（调试用）
    void setScrollSpeed(float speed) { _scrollSpeed = speed; }
    float getScrollSpeed() const { return _scrollSpeed; }

    // 获取背景总宽度
    float getTotalWidth() const { return _totalWidth; }

    // 获取背景世界宽度（不包含循环部分）
    float getWorldWidth() const { return _worldWidth; }

    // 添加update函数声明
    void update(float delta) override;

    // 添加一个函数来检查背景是否完全覆盖屏幕
    bool isFullyCoveringScreen() const {
        // 修正这里：使用 cocos2d::Director
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        return (_worldWidth >= visibleSize.width);
    }

private:
    std::vector<cocos2d::Sprite*> _backgroundSprites; // 所有背景精灵
    float _scrollSpeed;  // 当前滚动速度
    float _totalWidth;   // 所有背景图片的总宽度（包含循环部分）
    float _worldWidth;   // 实际世界宽度（不包含循环部分）

    // 辅助函数
    void setupContinuousBackground(const std::vector<std::string>& imageFiles);
    float calculateTotalWidth();

    // 摄像机偏移量
    float _cameraOffsetX;
};

#endif // CONTINUOUSBACKGROUND_H