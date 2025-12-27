#ifndef __POST_LEVEL4_HANDLER_H__
#define __POST_LEVEL4_HANDLER_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 * 处理Level 4完成后的交互逻辑
 * 显示30秒倒计时，提供继续到最终关卡或退出游戏的选项
 */
class PostLevel4Handler : public Node {
public:
    // 创建实例的静态工厂方法
    static PostLevel4Handler* create();
    
    // 初始化方法
    virtual bool init();
    
    // 开始处理Level 4完成后的交互
    void startProcessing();
    
    // 停止处理，清理资源
    void stopProcessing();
    
    // 每帧更新倒计时
    virtual void update(float delta) override;
    
    // 是否正在处理中
    bool isProcessing() const { return _isProcessing; }
    
    // 获取当前倒计时时间
    float getCountdownTime() const { return _countdownTimer; }
    
    /**
     * 处理用户输入Y键 - 继续到最终关卡
     * @note 内部使用，确保_isProcessing为true时调用
     */
    void handleUserInputY();
    
    /**
     * 处理用户输入N键 - 显示游戏结束
     * @note 内部使用，确保_isProcessing为true时调用
     */
    void handleUserInputN();
    
    /**
     * 处理倒计时结束的情况
     * @note 内部使用，确保_isProcessing为true时调用
     */
    void handleTimeout();
    
protected:
    // 构造函数和析构函数
    PostLevel4Handler();
    virtual ~PostLevel4Handler();
    
    // 暂停游戏
    void pauseGame();
    
    // 恢复游戏
    void resumeGame();
    
    // 创建UI元素
    void createUI();
    
    // 更新UI显示
    void updateUI();
    
    // 启用键盘输入
    void enableKeyboardInput();
    
    // 禁用键盘输入
    void disableKeyboardInput();
    
    // 加载最终关卡
    void loadFinalLevel();
    
    // 显示游戏结束界面
    void showGameOver();
    
    // 清理所有资源
    void cleanupResources();
    
private:
    // 处理状态标志
    bool _isProcessing;
    
    // 倒计时计时器
    float _countdownTimer;
    
    // UI元素
    Label* _countdownLabel;    // 倒计时标签
    Label* _promptLabel;       // 提示信息标签
    LayerColor* _overlay;      // 半透明遮罩层
    
    // 键盘监听器
    EventListenerKeyboard* _keyboardListener;
};

#endif // __POST_LEVEL4_HANDLER_H__