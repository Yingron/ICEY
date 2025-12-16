#include "Level.h"
#include "EnemyFactory.h"
#include "GameUtils.h"
#include "ui/CocosGUI.h" // [新增] 引入UI库，用于结局按钮
#include <stdexcept>

// ============================================================
// [辅助函数]：成就解锁系统
// ============================================================
static void unlockAchievement(const std::string& achievementName, const std::string& conditionDesc)
{
    try {
        auto userDefault = cocos2d::UserDefault::getInstance();
        bool isUnlocked = userDefault->getBoolForKey(achievementName.c_str(), false);

        if (!isUnlocked) {
            // 解锁成就并保存
            userDefault->setBoolForKey(achievementName.c_str(), true);
            userDefault->flush();

            GameUtils::log("==========================================");
            GameUtils::log("[系统提示] 解锁成就: " + achievementName);
            GameUtils::log("[达成条件]: " + conditionDesc);
            GameUtils::log("==========================================");

            // 可以在这里弹出一个 UI Tips 显示成就解锁
        }
    }
    catch (const std::exception& e) {
        GameUtils::log("[成就系统错误]: " + std::string(e.what()));
    }
}

Level* Level::createWithTMX(const std::string& tmxFile)
{
    Level* pRet = new(std::nothrow) Level();
    // [异常处理]: 对象创建
    try {
        if (pRet && pRet->initWithTMX(tmxFile)) {
            pRet->autorelease();
            return pRet;
        }
    }
    catch (...) {
        delete pRet;
        return nullptr;
    }
    delete pRet;
    return nullptr;
}

bool Level::initWithTMX(const std::string& tmxFile)
{
    if (!Node::init()) return false;

    m_levelName = tmxFile;

    // ============================================================
    // [特殊关卡逻辑]: Level 5 (结局抉择关)
    // ============================================================
    // 如果文件名是 Level5，我们不加载 TMX，而是直接构建剧情场景
    if (m_levelName == "Level5") {
        GameUtils::log("[关卡系统]: 进入最终抉择层 (Level 5)");

        // [剧情部分代码预留]
        // 这里不需要 TMX 地图，只需要黑屏或者简单的背景，以及对话框
        auto bg = cocos2d::LayerColor::create(cocos2d::Color4B::BLACK);
        this->addChild(bg, -1);

        // 延迟调用剧情对话，模拟Meta游戏的叙事节奏
        this->runAction(cocos2d::Sequence::create(
            cocos2d::DelayTime::create(1.0f),
            cocos2d::CallFunc::create([this]() {
                // [中文注释]: 剧情逻辑 - 旁白对话
                // 1. 屏幕显示文字：“你以为你赢了吗？”
                // 2. 旁白：“这里什么都没有，没有数据，没有敌人，只有你自己。”
                // 3. 旁白：“还要继续吗？回到那个痛苦的现实，还是留在这里做个美梦？”
                GameUtils::log("旁白: 还要继续吗？回到那个痛苦的现实，还是留在这里做个美梦？");
                }),
            cocos2d::DelayTime::create(3.0f), // 等待玩家阅读
            cocos2d::CallFunc::create([this]() {
                // 显示抉择按钮
                this->showFinalChoiceUI();
                }),
            nullptr
        ));

        return true; // 直接返回，不执行后续 TMX 解析
    }

    // ============================================================
    // [常规关卡逻辑]: 加载 TMX 地图
    // ============================================================
    try {
        // 1. 加载 TMX 地图
        m_tileMap = cocos2d::TMXTiledMap::create(tmxFile);
        if (!m_tileMap) {
            // 如果加载失败（比如资源不存在），抛出异常
            throw std::runtime_error("Failed to load TMX file: " + tmxFile);
        }
        this->addChild(m_tileMap, 0);

        // 2. 解析对象层
        parseTileMap();

        // 3. 设置世界边界
        cocos2d::Size mapSize = m_tileMap->getMapSize();
        cocos2d::Size tileSize = m_tileMap->getTileSize();
        m_worldBounds = cocos2d::Rect(0, 0, mapSize.width * tileSize.width, mapSize.height * tileSize.height);
    }
    catch (const std::exception& e) {
        GameUtils::log(std::string("[Level Init Error]: ") + e.what());
        // 降级处理：如果没有地图，创建一个空的物理边界防止穿帮
        m_worldBounds = cocos2d::Rect(0, 0, 2000, 1000);
    }

    this->scheduleUpdate();
    return true;
}

void Level::parseTileMap()
{
    try {
        // 解析平台 (Platforms)
        auto platformGroup = m_tileMap->getObjectGroup("Platforms");
        if (platformGroup) {
            auto& objects = platformGroup->getObjects();
            for (auto& obj : objects) {
                cocos2d::ValueMap& dict = obj.asValueMap();
                float x = dict["x"].asFloat();
                float y = dict["y"].asFloat();
                float w = dict["width"].asFloat();
                float h = dict["height"].asFloat();

                // 调试用：画出平台位置
                GameUtils::drawDebugRect(this, cocos2d::Rect(x, y, w, h), cocos2d::Color4F::GREEN);
            }
        }

        // 解析敌人出生点 (Spawners)
        auto spawnerGroup = m_tileMap->getObjectGroup("Spawners");
        if (spawnerGroup) {
            auto& objects = spawnerGroup->getObjects();
            for (auto& obj : objects) {
                cocos2d::ValueMap& dict = obj.asValueMap();
                float x = dict["x"].asFloat();
                float y = dict["y"].asFloat();
                std::string type = dict["Type"].asString();

                spawnEnemyAtPoint(cocos2d::Vec2(x, y), type);
            }
        }
    }
    catch (...) {
        GameUtils::log("[Map Parse Error]: Error reading object groups.");
    }
}

void Level::update(float delta)
{
    // [关卡逻辑]: 检测 BOSS 死亡以触发关卡跳转
    // 仅在 Level 4 且不是 Level 5 时检测
    if (m_levelName.find("Level4") != std::string::npos) {
        bool bossAlive = false;
        for (auto& enemy : m_enemies) {
            // 假设 Boss 死亡后会被从 m_enemies 移除，或者我们可以检查状态
            // 这里检查是否有名为 "KuMingYuanYang" 且存活的单位
            // 需要 Enemy 类有 getName() 方法
             // 注意：这里需要强转或者 Enemy 基类有 getName
            if (enemy->getName() == "KuMingYuanYang" && enemy->getHealth() > 0) {
                bossAlive = true;
                break;
            }
        }

        // 如果 BOSS 死完了 (列表中找不到活着的苦命鸳鸯)，且还没跳转
        static bool levelTransitioning = false;
        if (!bossAlive && !m_enemies.empty() && !levelTransitioning) {
            levelTransitioning = true;

            GameUtils::log("[剧情触发]: 苦命鸳鸯已被击败，记忆回廊开始崩塌...");

            // 延迟跳转到 Level 5
            this->runAction(cocos2d::Sequence::create(
                cocos2d::DelayTime::create(3.0f),
                cocos2d::CallFunc::create([this]() {
                    // [中文注释]: 切换场景逻辑
                    // 这里通常调用 SceneManager::switchScene("Level5");
                    // 模拟：重新创建一个 Level5 的 Layer 并替换当前内容
                    this->removeAllChildren();
                    this->initWithTMX("Level5"); // 重新初始化为 Level 5 模式
                    }),
                nullptr
            ));
        }
    }
}

void Level::spawnEnemyAtPoint(const cocos2d::Vec2& point, std::string typeName)
{
    try {
        EnemyType type = EnemyFactory::stringToEnemyType(typeName);
        auto enemy = EnemyFactory::createEnemy(type, point);
        if (enemy) {
            this->addChild(enemy.get(), 10);
            m_enemies.push_back(enemy);
        }
    }
    catch (const std::exception& e) {
        GameUtils::log("[Spawn Error]: " + std::string(e.what()));
    }
}

// ============================================================
// [新增功能]: Level 5 最终抉择 UI
// ============================================================
// 注意：需要在 Level.h 中声明 void showFinalChoiceUI();
// 如果没法改 Level.h，这部分逻辑可以写在 initWithTMX 的 lambda 中，
// 但为了代码清晰，我将其独立写出，假设你已添加到头文件。
void Level::showFinalChoiceUI()
{
    try {
        cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

        // 1. 按钮：醒来 (重返现实)
        auto btnWakeUp = cocos2d::ui::Button::create("Btn_WakeUp_Normal.png", "Btn_WakeUp_Pressed.png");
        if (!btnWakeUp) {
            // 降级：如果没有图片，使用系统文字按钮逻辑 (需要 Label 代替)
            GameUtils::log("Missing UI images, using logic placeholders.");
            return;
        }

        btnWakeUp->setTitleText("醒来 (重返现实)");
        btnWakeUp->setTitleFontSize(30);
        btnWakeUp->setPosition(cocos2d::Vec2(winSize.width * 0.3f, winSize.height * 0.5f));
        btnWakeUp->addClickEventListener([this](cocos2d::Ref* sender) {
            // [中文注释]: 玩家选择了现实
            GameUtils::log("玩家选择: 醒来。");

            // 达成成就一：辛苦你了 (通关)
            unlockAchievement("Achievement_GameClear", "辛苦你了，这么烂的游戏也能玩通关");

            // 达成成就二：重返现实
            unlockAchievement("Achievement_Reality", "当时，谁也不知道将来会发生什么");

            // 游戏结束逻辑 -> 退回主菜单或显示制作人员名单
            cocos2d::Director::getInstance()->end(); // 或者跳转 Scene
            });
        this->addChild(btnWakeUp, 100);

        // 2. 按钮：留下 (留在梦境)
        auto btnStay = cocos2d::ui::Button::create("Btn_Stay_Normal.png", "Btn_Stay_Pressed.png");
        btnStay->setTitleText("留下 (沉溺梦境)");
        btnStay->setTitleFontSize(30);
        btnStay->setPosition(cocos2d::Vec2(winSize.width * 0.7f, winSize.height * 0.5f));
        btnStay->addClickEventListener([this](cocos2d::Ref* sender) {
            // [中文注释]: 玩家选择了梦境
            GameUtils::log("玩家选择: 留下。");

            // 达成成就一：辛苦你了 (通关)
            unlockAchievement("Achievement_GameClear", "辛苦你了，这么烂的游戏也能玩通关");

            // 达成成就三：留在梦境
            unlockAchievement("Achievement_Dream", "人生最苦痛是梦醒了无路可走，做梦的人是幸福的");

            // 游戏结束逻辑 -> 可能会进入无尽模式或者 Meta 结局
            });
        this->addChild(btnStay, 100);

    }
    catch (const std::exception& e) {
        GameUtils::log("[UI Error]: " + std::string(e.what()));
    }
}

// 其他占位函数
void Level::checkTriggers(const cocos2d::Vec2& playerPosition) {}
bool Level::checkCollision(const cocos2d::Rect& rect) const { return m_worldBounds.intersectsRect(rect); }
cocos2d::Vec2 Level::getGroundPosition(const cocos2d::Vec2& position) const { return cocos2d::Vec2(position.x, 0); }