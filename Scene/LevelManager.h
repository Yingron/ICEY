// LevelManager.h - 淇缂栫爜骞舵坊鍔犵己灏戠殑鏋氫妇  
#pragma once  
#ifndef LEVELMANAGER_H  
#define LEVELMANAGER_H  

#include <string>  
#include <vector>  

class LevelManager {  
public:  
   // 鍏冲崱鐘舵€佹灇涓? 
   enum class LevelState {  
       START_SCENE,     // 寮€濮嬪満鏅? 
       LEVEL1,          // 鍏冲崱1  
       LEVEL2_1,        // 鍏冲崱2-1  
       LEVEL2_2,        // 鍏冲崱2-2  
       LEVEL2_3,        // 鍏冲崱2-3  
       LEVEL2_4,        // 鍏冲崱2-4  
       LEVEL2_5,        // 鍏冲崱2-5  
       LEVEL2_6,        // 鍏冲崱2-6  
       LEVEL3_1,        // 鍏冲崱3-1  
       LEVEL3_2,        // 鍏冲崱3-2  
       LEVEL3_3,        // 鍏冲崱3-3  
       LEVEL3_4,        // 鍏冲崱3-4  
       LEVEL3_5,        // 鍏冲崱3-5  
       LEVEL3_6,        // 鍏冲崱3-6  
       LEVEL4_1,        // 鍏冲崱4-1  
       LEVEL4_2,        // 鍏冲崱4-2  
       LEVEL4_3,        // 鍏冲崱4-3  
       LEVEL4_4,        // 鍏冲崱4-4  
       LEVEL4_5,        // 鍏冲崱4-5  
       LEVEL4_6,        // 鍏冲崱4-6
       HIDDEN_LEVEL1,   // 闅愯棌鍏冲崱1
       PAUSE_SCENE,     // 鏆傚仠鍦烘櫙
       GAMEOVER_SCENE,  // 娓告垙缁撴潫鍦烘櫙
       FINAL_LEVEL,     // 鏈€缁堝叧鍗?
       COMPLETED        // 娓告垙瀹屾垚  
   };  

   // 娓告垙妯″紡鏋氫妇  
   enum class GameMode {  
       DEFAULT,    // 榛樿妯″紡锛氬嚮璐ユ墍鏈夋晫浜哄悗閫氬叧  
       EASY        // 绠€鍗曟ā寮忥細鍙渶鍒拌揪缁堢偣鍗冲彲閫氬叧  
   };  
   // 鍏冲崱閰嶇疆鏁版嵁缁撴瀯  
   struct LevelConfig {
       LevelState level;
       std::string name;
       std::vector<std::string> backgroundImages;
       float worldWidth;  // 鍗曚綅锛氬叧鍗′笘鐣屽搴? 
   };

   static LevelManager* getInstance();  

   // 鑾峰彇褰撳墠鍏冲崱  
   LevelState getCurrentLevel() const { return _currentLevel; }  
   void setCurrentLevel(LevelState level);  

   // 鑾峰彇褰撳墠娓告垙妯″紡  
   GameMode getCurrentGameMode() const { return _currentGameMode; }  

   // 璁剧疆褰撳墠娓告垙妯″紡  
   void setCurrentGameMode(GameMode mode);  

   // 鍒囨崲娓告垙妯″紡  
   void toggleGameMode();  

   // 鍒ゆ柇鐜╁鏄惁鍙互鍒囨崲鍒颁笅涓€鍏冲崱  
   bool canSwitchToNextLevel(float playerWorldX);  

   // 鍒囨崲鍒颁笅涓€鍏冲崱  
   LevelState switchToNextLevel();  

   // 鍒濆鍖栧叧鍗? 
   void resetLevels();  

   // 鑾峰彇褰撳墠鍏冲崱鐨勮儗鏅浘鐗? 
   std::vector<std::string> getCurrentLevelBackgrounds() const;  

   // 鑾峰彇褰撳墠鍏冲崱鐨勪笘鐣屽搴? 
   float getCurrentLevelWorldWidth() const;  

   // 鑾峰彇褰撳墠鍏冲崱鐨勬渶澶т笘鐣孹鍧愭爣
   float getCurrentLevelMaxWorldX() const;
   
   // 鑾峰彇褰撳墠鍏冲崱鐨勫畬鏁撮厤缃?
   LevelConfig getCurrentLevelConfig() const;  

   // 鍒ゆ柇鐜╁鏄惁鍦ㄥ叧鍗¤竟鐣? 
   bool isPlayerAtLevelBoundary(float playerWorldX) const;  
   
   // 璁剧疆褰撳墠鏄剧ず鐨勮儗鏅浘鐗囩储寮?- 鐢ㄤ簬璺熻釜Level4-6涓殑鐗瑰畾鑳屾櫙
   void setCurrentBackgroundIndex(int index);
   
   // 鑾峰彇褰撳墠鏄剧ず鐨勮儗鏅浘鐗囩储寮?
   int getCurrentBackgroundIndex() const;  
   
   // 妫€鏌ユ槸鍚﹀埌杈綥evel4-6鐨勬渶鍚庝竴寮犺儗鏅浘(background-level4-6-3.png)
   bool isAtLevel4FinalBackground() const;  
   
   // 鏍囪鏄惁宸茶Е鍙慙evel4缁撴潫澶勭悊
   bool hasTriggeredLevel4Completion() const;
   void setTriggeredLevel4Completion(bool triggered);
   
   // 鍒囨崲鍒伴殣钘忓叧鍗?
   void switchToHiddenLevel1();
   
   // 浠庨殣钘忓叧鍗¤繑鍥?
   void returnFromHiddenLevel();

private:  
   LevelManager();  
   ~LevelManager() = default;  

   static LevelManager* _instance;  
   LevelState _currentLevel;  
   LevelState _previousLevel;  // 淇濆瓨涔嬪墠鐨勫叧鍗★紝鐢ㄤ簬杩斿洖闅愯棌鍏冲崱
   std::vector<LevelConfig> _levelConfigs;  

   // 褰撳墠鐨勬父鎴忔ā寮忕姸鎬? 
   GameMode _currentGameMode;  
   
   // 褰撳墠鏄剧ず鐨勮儗鏅浘鐗囩储寮? 
   int _currentBackgroundIndex;  
   
   // 鏄惁宸茶Е鍙慙evel4缁撴潫澶勭悊  
   bool _triggeredLevel4Completion;  

   // 鍒濆鍖栧叧鍗￠厤缃暟鎹? 
   void initLevelConfigs();  

   
};  

#endif // LEVELMANAGER_H