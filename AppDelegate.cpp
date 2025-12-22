/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "HelloWorldScene.h"

//使用上面的音频引擎！！
 #define USE_AUDIO_ENGINE 1   
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif
#include <proj.win32/MainGameScene.h>

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);  // 改为1280x720
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // 创建全屏窗口
        glview = GLViewImpl::createWithFullScreen("ICEY Game");
#else
        glview = GLViewImpl::create("ICEY Game");
#endif
        director->setOpenGLView(glview);
    }

    //初始化音频管理器***hy
    auto audioManager = AudioManager::getInstance();
    audioManager->init();

    // 设置音量
    audioManager->setBGMVolume(0.8f);
    audioManager->setEffectVolume(0.7f);
    audioManager->setUIVolume(0.5f);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // 获取实际屏幕大小
    auto frameSize = glview->getFrameSize();
    log("Screen resolution: %.0f x %.0f", frameSize.width, frameSize.height);

    // 设置设计分辨率
    // 使用 NO_BORDER 策略，保持设计分辨率宽高比，填满屏幕
    glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::NO_BORDER);

    // 计算内容缩放因子
    float scaleFactor = 1.0f;
    float designAspect = 1280.0f / 720.0f;
    float screenAspect = frameSize.width / frameSize.height;

    if (screenAspect > designAspect) {
        // 屏幕更宽，基于高度缩放
        scaleFactor = frameSize.height / 720.0f;
    }
    else {
        // 屏幕更高，基于宽度缩放
        scaleFactor = frameSize.width / 1280.0f;
    }

    // 设置内容缩放因子
    director->setContentScaleFactor(scaleFactor);
    log("Content scale factor: %.2f", scaleFactor);

    // 添加资源搜索路径
    auto fileUtils = FileUtils::getInstance();

    std::vector<std::string> searchPaths;
    searchPaths.push_back("Resources");
    searchPaths.push_back("Resources/images");
    searchPaths.push_back("Resources/images/environment");
    searchPaths.push_back("Resources/images/environment/background");
    searchPaths.push_back("Resources/images/characters/player");
    searchPaths.push_back("Resources/images/character/player");
    searchPaths.push_back("Resources/images/character");
    searchPaths.push_back("images/characters/player");
    searchPaths.push_back("images/character/player");
    searchPaths.push_back("images/environment/background");  // 添加Level2背景路径
    searchPaths.push_back("C:/aishi/test2/Resources/images/environment/background");  // Level2绝对路径
    searchPaths.push_back(".");
    fileUtils->setSearchPaths(searchPaths);

    // 创建主游戏场景
    auto scene = MainGameScene::createScene();

    // 运行场景
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
