#include "Level.h"
#include "EnemyFactory.h"
#include "GameUtils.h"

Level* Level::createWithTMX(const std::string& tmxFile)
{
    Level* pRet = new(std::nothrow) Level();
    if (pRet && pRet->initWithTMX(tmxFile)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool Level::initWithTMX(const std::string& tmxFile)
{
    if (!Node::init()) return false;

    m_levelName = tmxFile;

    // 1. 加载 TMX 地图
    m_tileMap = cocos2d::TMXTiledMap::create(tmxFile);
    if (!m_tileMap) {
        GameUtils::log("Error: Failed to load TMX: " + tmxFile);
        return false;
    }
    this->addChild(m_tileMap, 0);

    // 2. 解析对象层
    parseTileMap();

    // 3. 设置世界边界
    cocos2d::Size mapSize = m_tileMap->getMapSize();
    cocos2d::Size tileSize = m_tileMap->getTileSize();
    m_worldBounds = cocos2d::Rect(0, 0, mapSize.width * tileSize.width, mapSize.height * tileSize.height);

    this->scheduleUpdate();
    return true;
}

void Level::parseTileMap()
{
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

            // 假设 Platform 类有对应的创建方法
            // auto platform = Platform::createWithRect(cocos2d::Rect(x, y, w, h));
            // this->addChild(platform);
            // m_platforms.push_back(platform);

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
            std::string type = dict["Type"].asString(); // 获取 Tiled 中设置的类型名

            spawnEnemyAtPoint(cocos2d::Vec2(x, y), type);
        }
    }
}

void Level::update(float delta)
{
    // 关卡全局逻辑
}

void Level::spawnEnemies()
{
    // 如果有波次刷怪逻辑写在这里
}

void Level::spawnEnemyAtPoint(const cocos2d::Vec2& point, std::string typeName) // 修改接口以支持字符串
{
    // 调用工厂创建敌人
    auto enemy = EnemyFactory::createEnemy(typeName, point);
    if (enemy) {
        this->addChild(enemy.get(), 10); // Z-Order 10
        m_enemies.push_back(enemy);
        GameUtils::log("Spawned Enemy: " + typeName);
    }
}

// 适配头文件中的重载 (Enum版)
void Level::spawnEnemyAtPoint(const cocos2d::Vec2& point, EnemyType type) {
    // 简单转发
    auto enemy = EnemyFactory::createEnemy(type, point);
    if (enemy) {
        this->addChild(enemy.get(), 10);
        m_enemies.push_back(enemy);
    }
}

void Level::checkTriggers(const cocos2d::Vec2& playerPosition)
{
    // 遍历 m_triggers 检查碰撞
}

bool Level::checkCollision(const cocos2d::Rect& rect) const
{
    // 检查是否在世界边界内
    return m_worldBounds.intersectsRect(rect);
}

cocos2d::Vec2 Level::getGroundPosition(const cocos2d::Vec2& position) const
{
    return cocos2d::Vec2(position.x, 0); // 默认地面
}

void Level::createPhysicsBodies() {}
void Level::createPlatforms() {}
void Level::createTriggers() {}
void Level::setupCamera() {}