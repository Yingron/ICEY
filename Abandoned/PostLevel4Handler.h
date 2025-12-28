#ifndef __POST_LEVEL4_HANDLER_H__
#define __POST_LEVEL4_HANDLER_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 * 澶勭悊Level 4瀹屾垚鍚庣殑浜や簰閫昏緫
 * 鏄剧ず30绉掑€掕鏃讹紝鎻愪緵缁х画鍒版渶缁堝叧鍗℃垨閫€鍑烘父鎴忕殑閫夐」
 */
class PostLevel4Handler : public Node {
public:
    // 鍒涘缓瀹炰緥鐨勯潤鎬佸伐鍘傛柟娉?
    static PostLevel4Handler* create();
    
    // 鍒濆鍖栨柟娉?
    virtual bool init();
    
    // 寮€濮嬪鐞哃evel 4瀹屾垚鍚庣殑浜や簰
    void startProcessing();
    
    // 鍋滄澶勭悊锛屾竻鐞嗚祫婧?
    void stopProcessing();
    
    // 姣忓抚鏇存柊鍊掕鏃?
    virtual void update(float delta) override;
    
    // 鏄惁姝ｅ湪澶勭悊涓?
    bool isProcessing() const { return _isProcessing; }
    
    // 鑾峰彇褰撳墠鍊掕鏃舵椂闂?
    float getCountdownTime() const { return _countdownTimer; }
    
    /**
     * 澶勭悊鐢ㄦ埛杈撳叆Y閿?- 缁х画鍒版渶缁堝叧鍗?
     * @note 鍐呴儴浣跨敤锛岀‘淇漘isProcessing涓簍rue鏃惰皟鐢?
     */
    void handleUserInputY();
    
    /**
     * 澶勭悊鐢ㄦ埛杈撳叆N閿?- 鏄剧ず娓告垙缁撴潫
     * @note 鍐呴儴浣跨敤锛岀‘淇漘isProcessing涓簍rue鏃惰皟鐢?
     */
    void handleUserInputN();
    
    /**
     * 澶勭悊鍊掕鏃剁粨鏉熺殑鎯呭喌
     * @note 鍐呴儴浣跨敤锛岀‘淇漘isProcessing涓簍rue鏃惰皟鐢?
     */
    void handleTimeout();
    
protected:
    // 鏋勯€犲嚱鏁板拰鏋愭瀯鍑芥暟
    PostLevel4Handler();
    virtual ~PostLevel4Handler();
    
    // 鏆傚仠娓告垙
    void pauseGame();
    
    // 鎭㈠娓告垙
    void resumeGame();
    
    // 鍒涘缓UI鍏冪礌
    void createUI();
    
    // 鏇存柊UI鏄剧ず
    void updateUI();
    
    // 鍚敤閿洏杈撳叆
    void enableKeyboardInput();
    
    // 绂佺敤閿洏杈撳叆
    void disableKeyboardInput();
    
    // 鍔犺浇鏈€缁堝叧鍗?
    void loadFinalLevel();
    
    // 鏄剧ず娓告垙缁撴潫鐣岄潰
    void showGameOver();
    
    // 娓呯悊鎵€鏈夎祫婧?
    void cleanupResources();
    
private:
    // 澶勭悊鐘舵€佹爣蹇?
    bool _isProcessing;
    
    // 鍊掕鏃惰鏃跺櫒
    float _countdownTimer;
    
    // UI鍏冪礌
    Label* _countdownLabel;    // 鍊掕鏃舵爣绛?
    Label* _promptLabel;       // 鎻愮ず淇℃伅鏍囩
    LayerColor* _overlay;      // 鍗婇€忔槑閬僵灞?
    
    // 閿洏鐩戝惉鍣?
    EventListenerKeyboard* _keyboardListener;
};

#endif // __POST_LEVEL4_HANDLER_H__