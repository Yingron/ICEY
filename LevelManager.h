// LevelManager.h - 修复编码并添加缺少的枚举  
#pragma once  
#ifndef LEVELMANAGER_H  
#define LEVELMANAGER_H  

#include <string>  
#include <vector>  

class LevelManager {  
public:  
   // 关卡状态枚举  
   enum class LevelState {  
       START_SCENE,     // 开始场景  
       LEVEL1,          // 关卡1  
       LEVEL2_1,        // 关卡2-1  
       LEVEL2_2,        // 关卡2-2  
       LEVEL2_3,        // 关卡2-3  
       LEVEL2_4,        // 关卡2-4  
       LEVEL2_5,        // 关卡2-5  
       LEVEL2_6,        // 关卡2-6  
       LEVEL3_1,        // 关卡3-1  
       LEVEL3_2,        // 关卡3-2  
       LEVEL3_3,        // 关卡3-3  
       LEVEL3_4,        // 关卡3-4  
       LEVEL3_5,        // 关卡3-5  
       LEVEL3_6,        // 关卡3-6  
       LEVEL4_1,        // 关卡4-1  
       LEVEL4_2,        // 关卡4-2  
       LEVEL4_3,        // 关卡4-3  
       LEVEL4_4,        // 关卡4-4  
       LEVEL4_5,        // 关卡4-5  
       LEVEL4_6,        // 关卡4-6  
       PAUSE_SCENE,     // 暂停场景  
       GAMEOVER_SCENE,  // 游戏结束场景  
       FINAL_LEVEL,     // 最终关卡  
       COMPLETED        // 游戏完成  
   };  

   // 游戏模式枚举  
   enum class GameMode {  
       DEFAULT,    // 默认模式：击败所有敌人后通关  
       EASY        // 简单模式：只需到达终点即可通关  
   };  
   // 关卡配置数据结构  
   struct LevelConfig {
       LevelState level;
       std::string name;
       std::vector<std::string> backgroundImages;
       float worldWidth;  // 单位：关卡世界宽度  
   };

   static LevelManager* getInstance();  

   // 获取当前关卡  
   LevelState getCurrentLevel() const { return _currentLevel; }  
   void setCurrentLevel(LevelState level);  

   // 获取当前游戏模式  
   GameMode getCurrentGameMode() const { return _currentGameMode; }  

   // 设置当前游戏模式  
   void setCurrentGameMode(GameMode mode);  

   // 切换游戏模式  
   void toggleGameMode();  

   // 判断玩家是否可以切换到下一关卡  
   bool canSwitchToNextLevel(float playerWorldX);  

   // 切换到下一关卡  
   LevelState switchToNextLevel();  

   // 初始化关卡  
   void resetLevels();  

   // 获取当前关卡的背景图片  
   std::vector<std::string> getCurrentLevelBackgrounds() const;  

   // 获取当前关卡的世界宽度  
   float getCurrentLevelWorldWidth() const;  

   // 获取当前关卡的最大世界X坐标  
   float getCurrentLevelMaxWorldX() const;  

   // 判断玩家是否在关卡边界  
   bool isPlayerAtLevelBoundary(float playerWorldX) const;  
   
   // 设置当前显示的背景图片索引 - 用于跟踪Level4-6中的特定背景
   void setCurrentBackgroundIndex(int index);
   
   // 获取当前显示的背景图片索引
   int getCurrentBackgroundIndex() const;  
   
   // 检查是否到达Level4-6的最后一张背景图(background-level4-6-3.png)
   bool isAtLevel4FinalBackground() const;  
   
   // 标记是否已触发Level4结束处理
   bool hasTriggeredLevel4Completion() const;  
   void setTriggeredLevel4Completion(bool triggered);

private:  
   LevelManager();  
   ~LevelManager() = default;  

   static LevelManager* _instance;  
   LevelState _currentLevel;  
   std::vector<LevelConfig> _levelConfigs;  

   // 当前的游戏模式状态  
   GameMode _currentGameMode;  
   
   // 当前显示的背景图片索引  
   int _currentBackgroundIndex;  
   
   // 是否已触发Level4结束处理  
   bool _triggeredLevel4Completion;  

   // 初始化关卡配置数据  
   void initLevelConfigs();  

   
};  

#endif // LEVELMANAGER_H