#pragma once
#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__

#include "cocos2d.h"

class GameSceneManager
{
public:
    static void createStartScene();
    static void createGameScene();
    static void createPauseScene();
    static void createGameOverScene(bool isWin = false);

    static void replaceScene(cocos2d::Scene* scene);
    static void pushScene(cocos2d::Scene* scene);
    static void popScene();
};

#endif // __GAME_SCENE_MANAGER_H__