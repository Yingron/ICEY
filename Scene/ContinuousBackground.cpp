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

// 修正 setupContinuousBackground，确保加载时能正常工作
void ContinuousBackground::setupContinuousBackground(const std::vector<std::string>& imageFiles) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto fileUtils = FileUtils::getInstance();

    log("=== Setting up Continuous Background ===");
    log("Image count: %d", (int)imageFiles.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    float currentX = 0.0f;

    // 清空已有精灵列表
    _backgroundSprites.clear();

    // 如果没有背景图，则使用纯色占位
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

    // 遍历背景图片
    for (size_t i = 0; i < imageFiles.size(); i++) {
        const auto& filename = imageFiles[i];

        // 检查文件是否存在
        if (filename.empty() || !fileUtils->isFileExist(filename)) {
            log("ERROR: Background file not found or empty: %s", filename.c_str());

            // 使用占位背景
            auto placeholder = Sprite::create();
            placeholder->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
            placeholder->setColor(Color3B(50, 50, 50)); // 灰色占位

            placeholder->setAnchorPoint(Vec2::ZERO);
            placeholder->setPosition(Vec2(currentX, 0));

            this->addChild(placeholder);
            _backgroundSprites.push_back(placeholder);

            currentX += visibleSize.width;
            log("Created placeholder for background %d at x: %.0f", (int)i + 1, currentX - visibleSize.width);
            continue;
        }

        // 加载纹理
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        if (!texture) {
            log("ERROR: Failed to load texture: %s", filename.c_str());

            // 使用占位背景
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

        // 计算缩放，确保至少覆盖屏幕
        float scaleX = visibleSize.width / spriteWidth;
        float scaleY = visibleSize.height / spriteHeight;

        // 选用能完全覆盖屏幕的缩放
        float finalScale = scaleX;
        if (spriteHeight * scaleX < visibleSize.height) {
            // 如果高度不足，则按高度缩放
            finalScale = scaleY;
        }

        // 再次校验，必要时放大
        if (spriteWidth * finalScale < visibleSize.width ||
            spriteHeight * finalScale < visibleSize.height) {
            float neededScaleX = visibleSize.width / spriteWidth;
            float neededScaleY = visibleSize.height / spriteHeight;
            finalScale = MAX(neededScaleX, neededScaleY) * 1.1f; // 预留 10% 余量
        }

        sprite->setScale(finalScale);

        // 设置锚点与位置
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(currentX, 0));

        // 添加到场景
        this->addChild(sprite);
        _backgroundSprites.push_back(sprite);

        // 更新当前 x 位置
        float finalWidth = spriteWidth * finalScale;
        currentX += finalWidth;

        log("Added background %d: %s, original: %.0fx%.0f, scaled: %.0fx%.0f, x: %.0f",
            (int)i + 1, filename.c_str(),
            spriteWidth, spriteHeight,
            finalWidth, spriteHeight * finalScale,
            currentX - finalWidth);
    }

    // 记录世界宽度
    _worldWidth = currentX;
    _totalWidth = _worldWidth;

    log("Background setup complete");
    log("World background width: %.0f, sprite count: %d", _worldWidth, (int)_backgroundSprites.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    // 若总宽度仍小于屏幕宽度，复制最后一张补足
    if (_worldWidth < visibleSize.width) {
        log("Background total width (%.0f) < screen width (%.0f), extending background...",
            _worldWidth, visibleSize.width);

        if (!_backgroundSprites.empty()) {
            // 取最后一张
            Sprite* lastSprite = _backgroundSprites.back();
            float lastSpriteWidth = lastSprite->getContentSize().width * lastSprite->getScale();
            float lastSpriteHeight = lastSprite->getContentSize().height * lastSprite->getScale();

            // 需要重复的次数
            int repeats = ceil((visibleSize.width - _worldWidth) / lastSpriteWidth) + 1;

            for (int i = 0; i < repeats; i++) {
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

    // 如果仍未覆盖屏幕，继续填充
    if (_worldWidth < visibleSize.width) {
        log("WARNING: Background still doesn't cover screen fully. Adding filler.");

        // 使用与最后一张同色的填充块
        auto filler = Sprite::create();
        float fillerWidth = visibleSize.width - _worldWidth;
        filler->setTextureRect(Rect(0, 0, fillerWidth, visibleSize.height));

        if (!_backgroundSprites.empty()) {
            auto lastSprite = _backgroundSprites.back();
            filler->setColor(lastSprite->getColor());
        }
        else {
            filler->setColor(Color3B::BLACK);
        }

        filler->setAnchorPoint(Vec2::ZERO);
        filler->setPosition(Vec2(_worldWidth, 0));
        this->addChild(filler);

        _worldWidth += fillerWidth; // 增加填充部分宽度到世界宽度
        log("Added filler to complete screen coverage, new world width: %.0f", _worldWidth);
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
    // 更新摄像机偏移
    _cameraOffsetX = cameraOffsetX;

    // 设置背景位置
    this->setPositionX(cameraOffsetX);

    // 定期输出调试信息
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 120 == 0) { // 每 2 秒（约 60fps）输出一次
        auto visibleSize = Director::getInstance()->getVisibleSize();
        log("Background: PosX=%.0f, CamOffset=%.0f, WorldWidth=%.0f, ScreenWidth=%.0f",
            this->getPositionX(), cameraOffsetX, _worldWidth, visibleSize.width);
    }
}

void ContinuousBackground::update(float delta) {
    Node::update(delta);
}
