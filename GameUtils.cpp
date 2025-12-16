#include "GameUtils.h"
#include "ResourceManager.h" // 确保已包含

// [辅助函数]: 创建序列帧动画
// folder: 资源子目录 (例如 "cxk")
// prefix: 文件名前缀 (例如 "walk")
// frameCount: 总帧数
// delay: 每帧间隔时间
cocos2d::Animation* getOrCreateAnimation(const std::string& folder, const std::string& prefix, int frameCount, float delay)
{
    // 缓存 Key: "cxk-walk"
    std::string key = folder + "-" + prefix;
    auto anim = ResourceManager::getInstance()->getAnimation(key);

    if (!anim) {
        cocos2d::Vector<cocos2d::SpriteFrame*> frames;
        for (int i = 1; i <= frameCount; i++) {
            // 路径格式: images/characters/enemies/cxk/cxk-walk1.png
            std::string path = "images/characters/enemies/" + folder + "/" + folder + "-" + prefix + std::to_string(i) + ".png";

            auto texture = ResourceManager::getInstance()->getTexture(path);
            if (!texture) {
                texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(path);
            }

            if (texture) {
                auto frame = cocos2d::SpriteFrame::createWithTexture(texture, cocos2d::Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                frames.pushBack(frame);
            }
        }

        if (!frames.empty()) {
            anim = cocos2d::Animation::createWithSpriteFrames(frames, delay);
            ResourceManager::getInstance()->addAnimation(key, anim);
        }
    }
    return anim;
}