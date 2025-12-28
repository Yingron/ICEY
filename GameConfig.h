// GameConfig.h
#pragma once
#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// 锟斤拷戏锟斤拷锟斤拷
namespace GameConfig {
    // 锟斤拷锟斤拷锟斤拷锟斤拷
    const int WINDOW_WIDTH = 2560;
    const int WINDOW_HEIGHT = 1440;

    // 锟斤拷色锟斤拷锟斤拷
    const float PLAYER_SPEED = 400.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = 600.0f;      // 锟斤拷跃锟斤拷锟斤拷
    const float PLAYER_GROUND_Y = 0.0f;   // 锟斤拷锟斤拷叨锟?
    // 锟斤拷业锟斤拷锟轿伙拷茫锟斤拷锟斤拷锟侥伙拷系锟結锟斤拷锟斤拷俜直龋锟?
    const float PLAYER_GROUND_Y_PERCENT = 0.3f; // 锟斤拷 0.2f 锟斤拷为 0.3f

     // 锟斤拷锟斤拷锟斤拷锟斤拷
    namespace Animation {
        const float RUN_FRAME_DELAY = 0.05f;   // 锟杰讹拷锟斤拷锟斤拷帧锟接筹拷
        const float IDLE_FRAME_DELAY = 0.06f;  // 锟斤拷锟斤拷锟斤拷锟斤拷帧锟接筹拷
        const float ATTACK_FRAME_DELAY = 0.01f; // 锟斤拷锟斤拷锟斤拷锟斤拷帧锟接筹拷
        const float JUMP_FRAME_DELAY = 0.06f;  // 锟斤拷跃锟斤拷锟斤拷帧锟接筹拷 - 锟斤拷锟斤拷
    }

    // 锟斤拷锟斤拷锟斤拷锟斤拷
    namespace Input {
        const char MOVE_LEFT = 'A';
        const char MOVE_RIGHT = 'D';
        const char JUMP = 'W';
        const char ATTACK = 'K';
        const char DASH = ' ';
        const char SKILL1 = 'Q';
        const char SKILL2 = 'E';
        const char INVINCIBLE_MODE = 'B'; // 鏂板鏃犳晫妯″紡鎸夐敭瀹氫箟
        
        const cocos2d::EventKeyboard::KeyCode KEY_MOVE_LEFT = cocos2d::EventKeyboard::KeyCode::KEY_A;
        const cocos2d::EventKeyboard::KeyCode KEY_MOVE_RIGHT = cocos2d::EventKeyboard::KeyCode::KEY_D;
        const cocos2d::EventKeyboard::KeyCode KEY_JUMP = cocos2d::EventKeyboard::KeyCode::KEY_W;
        const cocos2d::EventKeyboard::KeyCode KEY_ATTACK = cocos2d::EventKeyboard::KeyCode::KEY_K;
        const cocos2d::EventKeyboard::KeyCode KEY_DASH = cocos2d::EventKeyboard::KeyCode::KEY_SPACE;
        const cocos2d::EventKeyboard::KeyCode KEY_SKILL1 = cocos2d::EventKeyboard::KeyCode::KEY_Q;
        const cocos2d::EventKeyboard::KeyCode KEY_SKILL2 = cocos2d::EventKeyboard::KeyCode::KEY_E;
        const cocos2d::EventKeyboard::KeyCode KEY_INVINCIBLE_MODE = cocos2d::EventKeyboard::KeyCode::KEY_B; // 鏂板鏃犳晫妯″紡鎸夐敭鐨凨eyCode瀹氫箟
    }

    // 锟斤拷锟斤拷锟斤拷锟?
    namespace Dash {
        const float DASH_SPEED = 1500.0f;      // 锟斤拷锟斤拷俣锟?
        const float DASH_DISTANCE = 300.0f;    // 锟斤拷叹锟斤拷锟?
        const float DASH_DURATION = 0.2f;      // 锟斤拷坛锟斤拷锟绞憋拷锟?
        const float DASH_COOLDOWN = 0.5f;      // 锟斤拷锟斤拷锟饺词憋拷锟?
        const float DASH_FRAME_DELAY = 0.03f;  // 锟斤拷潭锟斤拷锟街★拷映锟?
    }

    // 锟斤拷锟斤拷锟斤拷锟斤拷
    namespace Combo {
        const float COMBO_WINDOW = 0.3f;       // 锟斤拷锟斤拷时锟戒窗锟斤拷
        const int MAX_COMBO = 3;               // 锟斤拷锟斤拷锟斤拷锟斤拷锟?
    }

    // 锟斤拷锟缴硷拷锟姐集锟斤拷
    namespace ZOrder {
        const int BACKGROUND = -100;      // 锟斤拷鏅眰绾?
        const int PLATFORM = 0;           // 锟斤拷锟教筹拷锟姐集锟斤拷
        const int ENEMY = 50;             // 锟斤拷投锟斤拷慵拷锟?
        const int Z_ORDER_BOSS_ENEMY = 100; // BOSS锟斤拷投锟斤拷慵拷锟?
        const int PLAYER = 150;           // 锟斤拷戏锟斤拷员锟斤拷慵拷锟?
        const int ITEM = 200;             // 锟斤拷锟接讹拷锟斤拷
        const int EFFECT = 250;           // 锟斤拷效锟斤拷
        const int HUD = 1000;             // HUD锟斤拷锟姐集锟斤拷
    }

}
#endif // GAMECONFIG_H