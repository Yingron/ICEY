#pragma once
#ifndef __HUD_LAYER_H__
#define __HUD_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HUDLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Button* m_pauseButton;

public:
    virtual bool init() override;

    void setPauseCallback(const std::function<void(cocos2d::Ref*)>& callback);

private:
    void createPauseButton();

    CREATE_FUNC(HUDLayer);
};

#endif // __HUD_LAYER_H__