#include "cocos2d.h"
#include "PhysicsConfig.h"

USING_NS_CC;

// 简单的物理碰撞系统测试类
class PhysicsCollisionTest : public Layer
{
public:
    static Scene* createScene() {
        auto scene = Scene::createWithPhysics();
        auto layer = PhysicsCollisionTest::create();
        scene->addChild(layer);
        
        // 启用物理调试绘制
        auto physicsWorld = scene->getPhysicsWorld();
        physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
        physicsWorld->setGravity(Vec2(0, 0)); // 无重力
        
        return scene;
    }
    
    virtual bool init() override {
        if (!Layer::init()) {
            return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        
        // 创建玩家（绿色方块）
        auto player = Sprite::create();
        player->setContentSize(Size(50, 100));
        player->setColor(Color3B::GREEN);
        player->setPosition(origin.x + visibleSize.width * 0.3f, origin.y + visibleSize.height * 0.5f);
        
        auto playerBody = PhysicsBody::createBox(player->getContentSize(), 
            PhysicsMaterial(PhysicsMaterialParams::PLAYER_DENSITY,
                           PhysicsMaterialParams::PLAYER_RESTITUTION,
                           PhysicsMaterialParams::PLAYER_FRICTION));
        playerBody->setDynamic(true);
        playerBody->setGravityEnable(false);
        playerBody->setCategoryBitmask(COLLISION_CATEGORY_PLAYER);
        playerBody->setCollisionBitmask(COLLISION_MASK_PLAYER);
        playerBody->setContactTestBitmask(COLLISION_CATEGORY_ENEMY);
        player->setPhysicsBody(playerBody);
        this->addChild(player);
        
        // 创建敌人（红色方块）
        auto enemy = Sprite::create();
        enemy->setContentSize(Size(50, 100));
        enemy->setColor(Color3B::RED);
        enemy->setPosition(origin.x + visibleSize.width * 0.7f, origin.y + visibleSize.height * 0.5f);
        
        auto enemyBody = PhysicsBody::createBox(enemy->getContentSize(), 
            PhysicsMaterial(PhysicsMaterialParams::ENEMY_DENSITY,
                           PhysicsMaterialParams::ENEMY_RESTITUTION,
                           PhysicsMaterialParams::ENEMY_FRICTION));
        enemyBody->setDynamic(true);
        enemyBody->setGravityEnable(false);
        enemyBody->setCategoryBitmask(COLLISION_CATEGORY_ENEMY);
        enemyBody->setCollisionBitmask(COLLISION_MASK_ENEMY);
        enemyBody->setContactTestBitmask(COLLISION_CATEGORY_PLAYER);
        enemy->setPhysicsBody(enemyBody);
        this->addChild(enemy);
        
        // 创建边界墙
        createBoundaryWall(Size(50, visibleSize.height), Vec2(origin.x + 25, origin.y + visibleSize.height / 2), Color3B::BLUE); // 左墙
        createBoundaryWall(Size(50, visibleSize.height), Vec2(origin.x + visibleSize.width - 25, origin.y + visibleSize.height / 2), Color3B::BLUE); // 右墙
        createBoundaryWall(Size(visibleSize.width, 50), Vec2(origin.x + visibleSize.width / 2, origin.y + 25), Color3B::BLUE); // 下墙
        createBoundaryWall(Size(visibleSize.width, 50), Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 25), Color3B::BLUE); // 上墙
        
        // 添加碰撞监听器
        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(PhysicsCollisionTest::onContactBegin, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        // 添加触摸事件，控制玩家移动
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 touchPos = touch->getLocation();
            Vec2 direction = (touchPos - player->getPosition()).getNormalized();
            playerBody->applyImpulse(direction * 1000.0f);
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        // 每2秒给敌人一个随机力
        this->schedule([=](float delta) {
            float angle = rand() % 360;
            Vec2 direction = Vec2(cos(angle * M_PI / 180), sin(angle * M_PI / 180));
            enemyBody->applyImpulse(direction * 500.0f);
        }, 2.0f, "enemyRandomForce");
        
        return true;
    }
    
    bool onContactBegin(PhysicsContact& contact) {
        log("Collision detected!");
        return true;
    }
    
    void createBoundaryWall(const Size& size, const Vec2& position, const Color3B& color) {
        auto wall = Sprite::create();
        wall->setContentSize(size);
        wall->setPosition(position);
        wall->setColor(color);
        wall->setOpacity(100);
        
        auto wallBody = PhysicsBody::createBox(size, PhysicsMaterial(0, 0, 0));
        wallBody->setDynamic(false);
        wallBody->setCategoryBitmask(COLLISION_CATEGORY_BOUNDARY);
        wallBody->setCollisionBitmask(COLLISION_MASK_BOUNDARY);
        wallBody->setContactTestBitmask(0);
        wall->setPhysicsBody(wallBody);
        this->addChild(wall);
    }
};

// 主函数
int main() {
    Director::getInstance()->runWithScene(PhysicsCollisionTest::createScene());
    return 0;
}
