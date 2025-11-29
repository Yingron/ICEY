#pragma once
#ifndef __COMBO_DISPLAY_H__
#define __COMBO_DISPLAY_H__

#include "cocos2d.h"

class ComboDisplay : public cocos2d::Node
{
private:
    cocos2d::Label* m_comboCountLabel;
    cocos2d::Label* m_comboTextLabel;
    int m_currentCombo;
    float m_comboTimer;
    bool m_isVisible;

public:
    static ComboDisplay* create();
    virtual bool init() override;
    virtual void update(float delta) override;

    void setComboCount(int count);
    void incrementCombo();
    void resetCombo();
    void show();
    void hide();

    int getComboCount() const { return m_currentCombo; }

private:
    void createDisplay();
    void updateComboTimer(float delta);
    void animateComboText();

    CREATE_FUNC(ComboDisplay);
};

#endif // __COMBO_DISPLAY_H__