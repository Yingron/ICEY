#pragma once
#ifndef __SUBTITLE_DISPLAY_H__
#define __SUBTITLE_DISPLAY_H__

#include "cocos2d.h"

class SubtitleDisplay : public cocos2d::Node
{
private:
    cocos2d::Label* m_textLabel;
    cocos2d::LayerColor* m_background;
    float m_displayTime;
    float m_maxWidth;

public:
    static SubtitleDisplay* create();
    virtual bool init() override;

    void showText(const std::string& text, float duration = 3.0f);
    void hideText();

    void setTextColor(const cocos2d::Color3B& color);
    void setBackgroundOpacity(GLubyte opacity);

    bool isShowing() const;

private:
    void createBackground();
    void createTextLabel();
    void adjustBackgroundSize();

    CREATE_FUNC(SubtitleDisplay);
};

#endif // __SUBTITLE_DISPLAY_H__