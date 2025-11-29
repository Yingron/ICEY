#pragma once
#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "cocos2d.h"
#include <unordered_map>
#include <string>

class ResourceManager
{
private:
    static ResourceManager* m_instance;
    std::unordered_map<std::string, cocos2d::Texture2D*> m_textures;
    std::unordered_map<std::string, cocos2d::Animation*> m_animations;
    std::unordered_map<std::string, std::string> m_soundPaths;

public:
    static ResourceManager* getInstance();
    static void destroyInstance();

    // 纹理管理
    cocos2d::Texture2D* getTexture(const std::string& filename);
    void preloadTextures(const std::vector<std::string>& filenames);
    void unloadTexture(const std::string& filename);

    // 动画管理
    cocos2d::Animation* getAnimation(const std::string& name);
    void addAnimation(const std::string& name, cocos2d::Animation* animation);

    // 音效管理
    std::string getSoundPath(const std::string& soundName);
    void addSoundPath(const std::string& soundName, const std::string& filepath);

    // 资源清理
    void unloadAllTextures();
    void unloadAllAnimations();

private:
    ResourceManager();
    ~ResourceManager();
};

#endif // __RESOURCE_MANAGER_H__