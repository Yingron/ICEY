// Level1SceneBackground.h
#pragma once
#ifndef SCENEBACKGROUND_H  // 确保宏定义一致
#define SCENEBACKGROUND_H

#include "cocos2d.h"
#include "ContinuousBackground.h"
#include "LevelManager.h"

class SceneBackground : public cocos2d::Node {
public:
    CREATE_FUNC(SceneBackground);  // 创建函数名称改为SceneBackground

    virtual bool init();

    // 重新加载当前关卡的背景
    void reloadCurrentLevelBackground();

    // 根据玩家速度更新背景
    void updateWithPlayerSpeed(float delta, float playerSpeed);

    // 获取当前滚动速度
    float getScrollSpeed() const;

    // 添加update函数声明
    void update(float delta) override;

    // 获取世界宽度
    float getWorldWidth() const;

    // 更新背景位置
    void updateWithCameraOffset(float cameraOffsetX);

private:
    ContinuousBackground* _continuousBackground;

    // 设置关卡背景
    void setupLevel1Background();

    // 获取当前关卡的背景图片
    std::vector<std::string> getCurrentLevelBackgroundImages();

    // 背景图片路径（如果不需要可以删除）
    std::string getImagePath(const std::string& filename);
};

#endif // SCENEBACKGROUND_H