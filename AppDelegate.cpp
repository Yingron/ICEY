#include "AppDelegate.h"

#define USE_AUDIO_ENGINE 1


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
#include "MainGameScene.h"
#include"StartScene.h"
#include"AudioManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);  // 改为1280x720w
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
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

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
        //glview = GLViewImpl::createWithFullScreen("ICEY Game");
        glview=GLViewImpl::createWithRect("ICEY Game", cocos2d::Rect(0, 0, 1280, 720));
#else
        glview = GLViewImpl::create("ICEY Game");
#endif
        director->setOpenGLView(glview);
    }

    
    
    
    // 初始化音频管理器***hy
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

    glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);


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
auto scene = StartScene::createScene();

// 运行场景
director->runWithScene(scene);

return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // 暂停音频
   AudioManager::getInstance()->onEnterBackground();
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

    // 恢复音频
   AudioManager::getInstance()->onEnterForeground();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}