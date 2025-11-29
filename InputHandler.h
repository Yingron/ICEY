#pragma once
#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

#include "cocos2d.h"
#include "ICEY.h"
#include <unordered_map>

class InputHandler : public cocos2d::Node
{
private:
    ICEY* m_player;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, bool> m_keyStates;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, std::function<void()>> m_keyActions;

    bool m_isEnabled;

public:
    static InputHandler* create(ICEY* player);
    virtual bool init(ICEY* player) override;
    virtual void update(float delta) override;

    void setPlayer(ICEY* player) { m_player = player; }
    void setEnabled(bool enabled) { m_isEnabled = enabled; }

    void setupKeyBindings();
    void bindKey(cocos2d::EventKeyboard::KeyCode keyCode, const std::function<void()>& action);

    //  ‰»Îªÿµ˜
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

private:
    void processInput(float delta);
    bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode) const;

    CREATE_FUNC(InputHandler);
};

#endif // __INPUT_HANDLER_H__