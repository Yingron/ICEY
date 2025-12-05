#pragma once
// AnimationLoader.h
#ifndef ANIMATIONLOADER_H
#define ANIMATIONLOADER_H

#include "cocos2d.h"
#include <vector>
#include <string>

class AnimationLoader 
{
public:
    // 根据命名规则创建动画
    static cocos2d::Animation* createAnimationByPattern(
        const std::string& prefix,
        const std::string& suffix,
        int startFrame,
        int endFrame,
        float delayPerFrame,
        bool zeroPad = false,
        int padDigits = 2
    );

    // 自动检测帧数创建动画
    static cocos2d::Animation* createAnimationAutoDetect(
        const std::string& prefix,
        const std::string& suffix,
        int maxFrames = 30,
        float delayPerFrame = 0.08f,
        bool zeroPad = false,
        int padDigits = 2
    );

    // 从文件列表创建动画
    static cocos2d::Animation* createAnimationFromFileList(
        const std::vector<std::string>& fileList,
        float delayPerFrame
    );
};

#endif // ANIMATIONLOADER_H