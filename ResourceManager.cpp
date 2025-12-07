#include"ResourceManager.h"
#include"AnimationManager.h"
#include"GameConfig.h"
USING_NS_CC;

ResourceManager* ResourceManager::m_instance = nullptr;//实现单例模式，初始化

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// 释放所有缓存资源
	unloadAllTextures();
	unloadAllAnimations();
	m_soundPaths.clear();
}

ResourceManager* ResourceManager::getInstance()//创建单例
{
	if (!m_instance)
	{
		m_instance = new(std::nothrow) ResourceManager();
	}
	return m_instance;
}


void ResourceManager::destroyInstance()//销毁单例
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

Texture2D* ResourceManager::getTexture(const std::string& filename)
{
	auto it = m_textures.find(filename);
	if (it != m_textures.end())
	{
		return it->second; // 已缓存，直接返回
	}
	//加载路径
	std::string Path = GameConfig::TEXTURE_PATH + filename;
	Texture2D* texture = TextureCache::getInstance()->addImage(Path);
	if (texture)
	{
		m_textures[filename] = texture; // 加载成功后存入缓存
	}
	else
	{
		CCLOG("加载纹理失败 - %s", Path.c_str()); // 日志输出错误
	}
	return texture;
}

void ResourceManager::preloadTextures(const std::vector<std::string>& filenames)
{
	for (const auto& filename : filenames)
	{
		getTexture(filename);
	}
}

void ResourceManager::unloadTexture(const std::string& filename)
{
	auto it = m_textures.find(filename);
	if (it != m_textures.end())
	{
		// 先从Cocos2d-x的全局纹理缓存中移除
		TextureCache::getInstance()->removeTexture(it->second);
		// 再从本地缓存中删除
		m_textures.erase(it);
		CCLOG("卸载纹理 - %s", filename.c_str());
	}
}

// 添加动画到缓存
void ResourceManager::addAnimation(const std::string& name, Animation* animation)
{
	if (animation && m_animations.find(name) == m_animations.end())
	{
		m_animations[name] = animation; // 以动画名为键存储
		CCLOG("添加动画 - %s", name.c_str());
	}
}

// 获取动画
Animation* ResourceManager::getAnimation(const std::string& name)
{
	auto it = m_animations.find(name);
	if (it != m_animations.end())
	{
		return it->second;
	}
	CCLOG("未找到动画 - %s", name.c_str());
	return nullptr;
}

// 添加音效路径
void ResourceManager::addSoundPath(const std::string& soundName, const std::string& filepath)
{
	if (m_soundPaths.find(soundName) == m_soundPaths.end())
	{
		m_soundPaths[soundName] = GameConfig::SOUND_PATH + filepath;//音频路经
		CCLOG("添加音效路径 - %s", soundName.c_str());
	}
}

// 获取音效路径
std::string ResourceManager::getSoundPath(const std::string& soundName)
{
	auto it = m_soundPaths.find(soundName);
	if (it != m_soundPaths.end())
	{
		return it->second;
	}
	CCLOG("未找到音效路径 - %s", soundName.c_str());
	return "";
}
// 卸载所有纹理
void ResourceManager::unloadAllTextures()
{
	for (const auto& pair : m_textures)
	{
		TextureCache::getInstance()->removeTexture(pair.second);
	}
	m_textures.clear();
	CCLOG("卸载所有纹理");
}

// 卸载所有动画
void ResourceManager::unloadAllAnimations()
{
	m_animations.clear(); // 仅清除引用（动画实际由AnimationCache管理）
	CCLOG("卸载所有动画");
}
