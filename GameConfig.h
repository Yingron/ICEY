// GameConfig.h
#pragma once
#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// ��Ϸ����
namespace GameConfig {
    // ��������
    const int WINDOW_WIDTH = 2560;
    const int WINDOW_HEIGHT = 1440;

    // ��ɫ����
    const float PLAYER_SPEED = 400.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = 600.0f;      // ��Ծ����
    const float PLAYER_GROUND_Y = 0.0f;   // ����߶�
    // ��ҵ���λ�ã�����Ļ�ϵ�Y����ٷֱȣ�
    const float PLAYER_GROUND_Y_PERCENT = 0.3f; // �� 0.2f ��Ϊ 0.3f

     // ��������
    namespace Animation {
        const float RUN_FRAME_DELAY = 0.05f;   // �ܶ�����֡�ӳ�
        const float IDLE_FRAME_DELAY = 0.06f;  // ��������֡�ӳ�
        const float ATTACK_FRAME_DELAY = 0.01f; // ��������֡�ӳ�
        const float JUMP_FRAME_DELAY = 0.06f;  // ��Ծ����֡�ӳ� - ����
    }

    // ��������
    namespace Input {
        const char MOVE_LEFT = 'A';
        const char MOVE_RIGHT = 'D';
        const char JUMP = 'W';
        const char ATTACK = 'K';
        const char DASH = ' ';
        const char SKILL1 = 'Q';
        const char SKILL2 = 'E';
        const char INVINCIBLE_MODE = 'B'; // 新增无敌模式按键定义
        
        const cocos2d::EventKeyboard::KeyCode KEY_MOVE_LEFT = cocos2d::EventKeyboard::KeyCode::KEY_A;
        const cocos2d::EventKeyboard::KeyCode KEY_MOVE_RIGHT = cocos2d::EventKeyboard::KeyCode::KEY_D;
        const cocos2d::EventKeyboard::KeyCode KEY_JUMP = cocos2d::EventKeyboard::KeyCode::KEY_W;
        const cocos2d::EventKeyboard::KeyCode KEY_ATTACK = cocos2d::EventKeyboard::KeyCode::KEY_K;
        const cocos2d::EventKeyboard::KeyCode KEY_DASH = cocos2d::EventKeyboard::KeyCode::KEY_SPACE;
        const cocos2d::EventKeyboard::KeyCode KEY_SKILL1 = cocos2d::EventKeyboard::KeyCode::KEY_Q;
        const cocos2d::EventKeyboard::KeyCode KEY_SKILL2 = cocos2d::EventKeyboard::KeyCode::KEY_E;
        const cocos2d::EventKeyboard::KeyCode KEY_INVINCIBLE_MODE = cocos2d::EventKeyboard::KeyCode::KEY_B; // 新增无敌模式按键的KeyCode定义
    }

    // �������
    namespace Dash {
        const float DASH_SPEED = 1500.0f;      // ����ٶ�
        const float DASH_DISTANCE = 300.0f;    // ��̾���
        const float DASH_DURATION = 0.2f;      // ��̳���ʱ��
        const float DASH_COOLDOWN = 0.5f;      // �����ȴʱ��
        const float DASH_FRAME_DELAY = 0.03f;  // ��̶���֡�ӳ�
    }

    // ��������
    namespace Combo {
        const float COMBO_WINDOW = 0.3f;       // ����ʱ�䴰��
        const int MAX_COMBO = 3;               // ���������
    }

    // ���ɼ��㼯��
    namespace ZOrder {
        const int BACKGROUND = -100;      // ��景层级
        const int PLATFORM = 0;           // ���̳��㼯��
        const int ENEMY = 50;             // ��Ͷ��㼯��
        const int Z_ORDER_BOSS_ENEMY = 100; // BOSS��Ͷ��㼯��
        const int PLAYER = 150;           // ��Ϸ��Ա��㼯��
        const int ITEM = 200;             // ���Ӷ���
        const int EFFECT = 250;           // ��Ч��
        const int HUD = 1000;             // HUD���㼯��
    }

}
#endif // GAMECONFIG_H