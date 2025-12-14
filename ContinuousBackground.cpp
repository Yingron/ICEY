#include "ContinuousBackground.h"
#include "GameConfig.h"

USING_NS_CC;

bool ContinuousBackground::init() {
    if (!Node::init()) return false;

    _scrollSpeed = 0.0f;
    _totalWidth = 0.0f;
    _worldWidth = 0.0f;
    _cameraOffsetX = 0.0f;

    return true;
}

bool ContinuousBackground::initWithImages(const std::vector<std::string>& imageFiles) {
    if (!init()) return false;

    setupContinuousBackground(imageFiles);

    return true;
}

ContinuousBackground* ContinuousBackground::createWithImageSequence(const std::vector<std::string>& imageFiles) {
    ContinuousBackground* bg = new (std::nothrow) ContinuousBackground();
    if (bg && bg->initWithImages(imageFiles)) {
        bg->autorelease();
        return bg;
    }
    CC_SAFE_DELETE(bg);
    return nullptr;
}

// 修改 setupContinuousBackground 函数，使用更可靠的背景覆盖策略
void ContinuousBackground::setupContinuousBackground(const std::vector<std::string>& imageFiles) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto fileUtils = FileUtils::getInstance();

    log("=== Setting up Continuous Background ===");
    log("Image count: %d", (int)imageFiles.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    float currentX = 0.0f;

    // 清空现有精灵
    _backgroundSprites.clear();

    // 如果没有背景图片，创建一个黑色背景
    if (imageFiles.empty()) {
        log("WARNING: No background images provided");
        auto blackBackground = Sprite::create();
        blackBackground->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
        blackBackground->setColor(Color3B::BLACK);
        blackBackground->setAnchorPoint(Vec2::ZERO);
        blackBackground->setPosition(Vec2(0, 0));
        this->addChild(blackBackground);
        _backgroundSprites.push_back(blackBackground);

        _worldWidth = visibleSize.width;
        _totalWidth = visibleSize.width;
        log("Created black background with screen size");
        return;
    }

    // 加载背景图片
    for (size_t i = 0; i < imageFiles.size(); i++) {
        const auto& filename = imageFiles[i];

        // 检查文件是否存在
        if (filename.empty() || !fileUtils->isFileExist(filename)) {
            log("ERROR: Background file not found or empty: %s", filename.c_str());

            // 创建占位符精灵
            auto placeholder = Sprite::create();
            placeholder->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
            placeholder->setColor(Color3B(50, 50, 50)); // 深灰色占位符

            placeholder->setAnchorPoint(Vec2::ZERO);
            placeholder->setPosition(Vec2(currentX, 0));

            this->addChild(placeholder);
            _backgroundSprites.push_back(placeholder);

            currentX += visibleSize.width;
            log("Created placeholder for background %d at x: %.0f", (int)i + 1, currentX - visibleSize.width);
            continue;
        }

        // 创建背景精灵
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        if (!texture) {
            log("ERROR: Failed to load texture: %s", filename.c_str());

            // 创建占位符
            auto placeholder = Sprite::create();
            placeholder->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
            placeholder->setColor(Color3B(100, 100, 100));
            placeholder->setAnchorPoint(Vec2::ZERO);
            placeholder->setPosition(Vec2(currentX, 0));

            this->addChild(placeholder);
            _backgroundSprites.push_back(placeholder);
            currentX += visibleSize.width;
            continue;
        }

        Texture2D::TexParams texParams;
        texParams.minFilter = GL_LINEAR;
        texParams.magFilter = GL_LINEAR;
        texParams.wrapS = GL_CLAMP_TO_EDGE;
        texParams.wrapT = GL_CLAMP_TO_EDGE;
        texture->setTexParameters(texParams);

        auto sprite = Sprite::createWithTexture(texture);
        if (!sprite) {
            log("ERROR: Failed to create sprite for: %s", filename.c_str());
            continue;
        }

        // 获取图片原始尺寸
        float spriteWidth = sprite->getContentSize().width;
        float spriteHeight = sprite->getContentSize().height;

        // 关键修复：使用更精确的覆盖算法
        // 计算需要完全覆盖屏幕的最小缩放比例
        float scaleX = visibleSize.width / spriteWidth;
        float scaleY = visibleSize.height / spriteHeight;

        // 选择能完全覆盖屏幕的较大缩放比例
        float finalScale = scaleX;
        if (spriteHeight * scaleX < visibleSize.height) {
            // 如果按宽度缩放后高度不够，使用高度缩放
            finalScale = scaleY;
        }

        // 确保最终缩放比例至少能覆盖屏幕
        if (spriteWidth * finalScale < visibleSize.width ||
            spriteHeight * finalScale < visibleSize.height) {
            // 如果仍然不能覆盖，使用更大的缩放比例
            float neededScaleX = visibleSize.width / spriteWidth;
            float neededScaleY = visibleSize.height / spriteHeight;
            finalScale = MAX(neededScaleX, neededScaleY) * 1.1f; // 增加10%确保覆盖
        }

        sprite->setScale(finalScale);

        // 设置锚点和位置
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(currentX, 0));

        // 添加到节点
        this->addChild(sprite);
        _backgroundSprites.push_back(sprite);

        // 更新当前位置
        float finalWidth = spriteWidth * finalScale;
        currentX += finalWidth;

        log("Added background %d: %s, original: %.0fx%.0f, scaled: %.0fx%.0f, x: %.0f",
            (int)i + 1, filename.c_str(),
            spriteWidth, spriteHeight,
            finalWidth, spriteHeight * finalScale,
            currentX - finalWidth);
    }

    // 记录世界宽度（实际背景宽度）
    _worldWidth = currentX;
    _totalWidth = _worldWidth;

    log("Background setup complete");
    log("World background width: %.0f, sprite count: %d", _worldWidth, (int)_backgroundSprites.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    // 关键修复：强制确保背景完全覆盖屏幕
    if (_worldWidth < visibleSize.width) {
        log("Background total width (%.0f) < screen width (%.0f), extending background...",
            _worldWidth, visibleSize.width);

        if (!_backgroundSprites.empty()) {
            // 获取最后一张背景图
            Sprite* lastSprite = _backgroundSprites.back();
            float lastSpriteWidth = lastSprite->getContentSize().width * lastSprite->getScale();
            float lastSpriteHeight = lastSprite->getContentSize().height * lastSprite->getScale();

            // 计算需要重复的次数
            int repeats = ceil((visibleSize.width - _worldWidth) / lastSpriteWidth) + 1;

            for (int i = 0; i < repeats; i++) {
                // 复制最后一张背景图
                auto repeatSprite = Sprite::createWithTexture(lastSprite->getTexture());
                repeatSprite->setScale(lastSprite->getScale());
                repeatSprite->setAnchorPoint(Vec2::ZERO);
                repeatSprite->setPosition(Vec2(currentX, 0));

                this->addChild(repeatSprite);
                _backgroundSprites.push_back(repeatSprite);

                currentX += lastSpriteWidth;
            }

            _worldWidth = currentX;
            log("Extended background, new total width: %.0f", _worldWidth);
        }
    }

    // 最终检查：如果背景仍然不能完全覆盖屏幕，添加填充
    if (_worldWidth < visibleSize.width) {
        log("WARNING: Background still doesn't cover screen fully. Adding filler.");

        // 创建与最后一张背景图颜色相近的填充
        auto filler = Sprite::create();
        float fillerWidth = visibleSize.width - _worldWidth;
        filler->setTextureRect(Rect(0, 0, fillerWidth, visibleSize.height));

        if (!_backgroundSprites.empty()) {
            // 尝试从最后一张背景图中获取颜色
            auto lastSprite = _backgroundSprites.back();
            filler->setColor(lastSprite->getColor());
        }
        else {
            filler->setColor(Color3B::BLACK);
        }

        filler->setAnchorPoint(Vec2::ZERO);
        filler->setPosition(Vec2(_worldWidth, 0));
        this->addChild(filler);

        _worldWidth = visibleSize.width;
        log("Added filler to complete screen coverage");
    }

    log("Final background width: %.0f, screen width: %.0f", _worldWidth, visibleSize.width);
}

float ContinuousBackground::calculateTotalWidth() {
    float width = 0.0f;
    for (auto sprite : _backgroundSprites) {
        width += sprite->getContentSize().width * sprite->getScale();
    }
    return width;
}

void ContinuousBackground::updateWithCameraOffset(float cameraOffsetX) {
    // 保存摄像机偏移
    _cameraOffsetX = cameraOffsetX;

    // 移动整个背景节点
    this->setPositionX(cameraOffsetX);

    // 调试信息 - 减少日志频率
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 120 == 0) { // 每2秒记录一次（假设60fps）
        auto visibleSize = Director::getInstance()->getVisibleSize();
        log("Background: PosX=%.0f, CamOffset=%.0f, WorldWidth=%.0f, ScreenWidth=%.0f",
            this->getPositionX(), cameraOffsetX, _worldWidth, visibleSize.width);
    }
}

void ContinuousBackground::update(float delta) {
    Node::update(delta);
}