#include "SceneTrigger.h"
#include "GameUtils.h"

// SceneTrigger 通常用于：播放剧情、刷怪、切换关卡
// 假设继承自 Node

SceneTrigger* SceneTrigger::createWithRect(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene)
{
    SceneTrigger* pRet = new(std::nothrow) SceneTrigger();
    if (pRet && pRet->init(rect, type, targetScene))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool SceneTrigger::init(const cocos2d::Rect& rect, const std::string& type, const std::string& targetScene)
{
    if (!cocos2d::Node::init()) return false;
    m_triggerRect = rect;
    m_triggerType = type;
    m_targetScene = targetScene;
    m_isActivated = false;
    this->setPosition(rect.origin);
    return true;
}

void SceneTrigger::setup(const cocos2d::Rect& rect, const std::string& eventName)
{
    m_triggerRect = rect;
    m_eventName = eventName;

    this->setPosition(rect.origin);

    // 调试显示：黄色半透明区域
    auto drawNode = cocos2d::DrawNode::create();
    cocos2d::Vec2 vertices[] = {
        cocos2d::Vec2(0, 0),
        cocos2d::Vec2(rect.size.width, 0),
        cocos2d::Vec2(rect.size.width, rect.size.height),
        cocos2d::Vec2(0, rect.size.height)
    };
    drawNode->drawPolygon(vertices, 4, cocos2d::Color4F(1, 1, 0, 0.2f), 1, cocos2d::Color4F::YELLOW);
    this->addChild(drawNode);
}

void SceneTrigger::checkTrigger(cocos2d::Node* player)
{
    if (!m_isActive || m_isTriggered || !player) return;

    // 获取玩家的世界坐标
    cocos2d::Vec2 playerPos = player->getPosition();

    // 检查点是否在矩形内
    // m_triggerRect 通常是世界坐标定义
    if (m_triggerRect.containsPoint(playerPos))
    {
        executeEvent();
    }
}

void SceneTrigger::executeEvent()
{
    m_isTriggered = true;
    GameUtils::log("Trigger Activated: " + m_eventName);

    // 这里可以分发事件
    // 例如：EventCustom
    cocos2d::EventCustom event("scene_trigger_event");
    event.setUserData((void*)m_eventName.c_str());
    _eventDispatcher->dispatchEvent(&event);

    // 视觉反馈
    this->runAction(cocos2d::FadeOut::create(0.5f));

    // 如果是一次性触发器，可以禁用或移除
    // m_isActive = false;
}

void SceneTrigger::reset()
{
    m_isTriggered = false;
    this->setOpacity(255);
}