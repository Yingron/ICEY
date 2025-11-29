#pragma once
#ifndef __INPUT_CONFIG_H__
#define __INPUT_CONFIG_H__

#include "cocos2d.h"

namespace InputConfig
{
    // º¸≈Ã”≥…‰
    const cocos2d::EventKeyboard::KeyCode KEY_LEFT = cocos2d::EventKeyboard::KeyCode::KEY_A;
    const cocos2d::EventKeyboard::KeyCode KEY_RIGHT = cocos2d::EventKeyboard::KeyCode::KEY_D;
    const cocos2d::EventKeyboard::KeyCode KEY_UP = cocos2d::EventKeyboard::KeyCode::KEY_W;
    const cocos2d::EventKeyboard::KeyCode KEY_DOWN = cocos2d::EventKeyboard::KeyCode::KEY_S;
    const cocos2d::EventKeyboard::KeyCode KEY_JUMP = cocos2d::EventKeyboard::KeyCode::KEY_SPACE;
    const cocos2d::EventKeyboard::KeyCode KEY_DODGE = cocos2d::EventKeyboard::KeyCode::KEY_SHIFT;
    const cocos2d::EventKeyboard::KeyCode KEY_ATTACK_LIGHT = cocos2d::EventKeyboard::KeyCode::KEY_J;
    const cocos2d::EventKeyboard::KeyCode KEY_ATTACK_HEAVY = cocos2d::EventKeyboard::KeyCode::KEY_K;
    const cocos2d::EventKeyboard::KeyCode KEY_SKILL_1 = cocos2d::EventKeyboard::KeyCode::KEY_U;
    const cocos2d::EventKeyboard::KeyCode KEY_SKILL_2 = cocos2d::EventKeyboard::KeyCode::KEY_I;
    const cocos2d::EventKeyboard::KeyCode KEY_PAUSE = cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE;
}

#endif // __INPUT_CONFIG_H__