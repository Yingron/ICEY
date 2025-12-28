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

// 锟睫革拷 setupContinuousBackground 锟斤拷锟斤拷锟斤拷使锟矫革拷锟缴匡拷锟侥憋拷锟斤拷锟斤拷锟角诧拷锟斤拷
void ContinuousBackground::setupContinuousBackground(const std::vector<std::string>& imageFiles) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto fileUtils = FileUtils::getInstance();

    log("=== Setting up Continuous Background ===");
    log("Image count: %d", (int)imageFiles.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    float currentX = 0.0f;

    // 锟斤拷锟斤拷锟斤拷芯锟斤拷锟?
    _backgroundSprites.clear();

    // 锟斤拷锟矫伙拷斜锟斤拷锟酵计拷锟斤拷锟斤拷锟揭伙拷锟斤拷锟缴拷锟斤拷锟?
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

    // 锟斤拷锟截憋拷锟斤拷图片
    for (size_t i = 0; i < imageFiles.size(); i++) {
        const auto& filename = imageFiles[i];

        // 锟斤拷锟斤拷募锟斤拷欠锟斤拷锟斤拷
        if (filename.empty() || !fileUtils->isFileExist(filename)) {
            log("ERROR: Background file not found or empty: %s", filename.c_str());

            // 锟斤拷锟斤拷占位锟斤拷锟斤拷锟斤拷
            auto placeholder = Sprite::create();
            placeholder->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
            placeholder->setColor(Color3B(50, 50, 50)); // 锟斤拷锟缴嘉伙拷锟?

            placeholder->setAnchorPoint(Vec2::ZERO);
            placeholder->setPosition(Vec2(currentX, 0));

            this->addChild(placeholder);
            _backgroundSprites.push_back(placeholder);

            currentX += visibleSize.width;
            log("Created placeholder for background %d at x: %.0f", (int)i + 1, currentX - visibleSize.width);
            continue;
        }

        // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        if (!texture) {
            log("ERROR: Failed to load texture: %s", filename.c_str());

            // 锟斤拷锟斤拷占位锟斤拷
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

        // 锟斤拷取图片原始锟竭达拷
        float spriteWidth = sprite->getContentSize().width;
        float spriteHeight = sprite->getContentSize().height;

        // 锟截硷拷锟睫革拷锟斤拷使锟矫革拷锟斤拷确锟侥革拷锟斤拷锟姐法
        // 锟斤拷锟斤拷锟斤拷要锟斤拷全锟斤拷锟斤拷锟斤拷幕锟斤拷锟斤拷小锟斤拷锟脚憋拷锟斤拷
        float scaleX = visibleSize.width / spriteWidth;
        float scaleY = visibleSize.height / spriteHeight;

        // 选锟斤拷锟斤拷锟斤拷全锟斤拷锟斤拷锟斤拷幕锟侥较达拷锟斤拷锟脚憋拷锟斤拷
        float finalScale = scaleX;
        if (spriteHeight * scaleX < visibleSize.height) {
            // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷藕锟竭度诧拷锟斤拷锟斤拷使锟矫高讹拷锟斤拷锟斤拷
            finalScale = scaleY;
        }

        // 确锟斤拷锟斤拷锟斤拷锟斤拷锟脚憋拷锟斤拷锟斤拷锟斤拷锟杰革拷锟斤拷锟斤拷幕
        if (spriteWidth * finalScale < visibleSize.width ||
            spriteHeight * finalScale < visibleSize.height) {
            // 锟斤拷锟斤拷锟饺伙拷锟斤拷芨锟斤拷牵锟绞癸拷酶锟斤拷锟斤拷锟斤拷锟脚憋拷锟斤拷
            float neededScaleX = visibleSize.width / spriteWidth;
            float neededScaleY = visibleSize.height / spriteHeight;
            finalScale = MAX(neededScaleX, neededScaleY) * 1.1f; // 锟斤拷锟斤拷10%确锟斤拷锟斤拷锟斤拷
        }

        sprite->setScale(finalScale);

        // 锟斤拷锟斤拷锚锟斤拷锟轿伙拷锟?
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(currentX, 0));

        // 锟斤拷锟接碉拷锟节碉拷
        this->addChild(sprite);
        _backgroundSprites.push_back(sprite);

        // 锟斤拷锟铰碉拷前位锟斤拷
        float finalWidth = spriteWidth * finalScale;
        currentX += finalWidth;

        log("Added background %d: %s, original: %.0fx%.0f, scaled: %.0fx%.0f, x: %.0f",
            (int)i + 1, filename.c_str(),
            spriteWidth, spriteHeight,
            finalWidth, spriteHeight * finalScale,
            currentX - finalWidth);
    }

    // 锟斤拷录锟斤拷锟斤拷锟斤拷龋锟绞碉拷时锟斤拷锟斤拷锟斤拷龋锟?
    _worldWidth = currentX;
    _totalWidth = _worldWidth;

    log("Background setup complete");
    log("World background width: %.0f, sprite count: %d", _worldWidth, (int)_backgroundSprites.size());
    log("Screen size: %.0fx%.0f", visibleSize.width, visibleSize.height);

    // 锟截硷拷锟睫革拷锟斤拷强锟斤拷确锟斤拷锟斤拷锟斤拷锟斤拷全锟斤拷锟斤拷锟斤拷幕
    if (_worldWidth < visibleSize.width) {
        log("Background total width (%.0f) < screen width (%.0f), extending background...",
            _worldWidth, visibleSize.width);

        if (!_backgroundSprites.empty()) {
            // 锟斤拷取锟斤拷锟揭伙拷疟锟斤拷锟酵?
            Sprite* lastSprite = _backgroundSprites.back();
            float lastSpriteWidth = lastSprite->getContentSize().width * lastSprite->getScale();
            float lastSpriteHeight = lastSprite->getContentSize().height * lastSprite->getScale();

            // 锟斤拷锟斤拷锟斤拷要锟截革拷锟侥达拷锟斤拷
            int repeats = ceil((visibleSize.width - _worldWidth) / lastSpriteWidth) + 1;

            for (int i = 0; i < repeats; i++) {
                // 锟斤拷锟斤拷锟斤拷锟揭伙拷疟锟斤拷锟酵?
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

    // 锟斤拷锟秸硷拷椋猴拷锟斤拷锟斤拷锟斤拷锟斤拷然锟斤拷锟斤拷锟斤拷全锟斤拷锟斤拷锟斤拷幕锟斤拷锟斤拷锟斤拷锟斤拷锟?
    if (_worldWidth < visibleSize.width) {
        log("WARNING: Background still doesn't cover screen fully. Adding filler.");

        // 锟斤拷锟斤拷锟斤拷锟斤拷锟揭伙拷疟锟斤拷锟酵硷拷锟缴拷锟斤拷锟斤拷锟斤拷锟?
        auto filler = Sprite::create();
        float fillerWidth = visibleSize.width - _worldWidth;
        filler->setTextureRect(Rect(0, 0, fillerWidth, visibleSize.height));

        if (!_backgroundSprites.empty()) {
            // 锟斤拷锟皆达拷锟斤拷锟揭伙拷疟锟斤拷锟酵硷拷谢锟饺★拷锟缴?
            auto lastSprite = _backgroundSprites.back();
            filler->setColor(lastSprite->getColor());
        }
        else {
            filler->setColor(Color3B::BLACK);
        }

        filler->setAnchorPoint(Vec2::ZERO);
        filler->setPosition(Vec2(_worldWidth, 0));
        this->addChild(filler);

        _worldWidth += fillerWidth; // 澧炲姞濉厖鐨勫搴﹀埌涓栫晫瀹藉害
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
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟狡拷锟?
    _cameraOffsetX = cameraOffsetX;

    // 锟狡讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟节碉拷
    this->setPositionX(cameraOffsetX);

    // 锟斤拷锟斤拷锟斤拷息 - 锟斤拷锟斤拷锟斤拷志频锟斤拷
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 120 == 0) { // 每2锟斤拷锟铰家伙拷危锟斤拷锟斤拷锟?0fps锟斤拷
        auto visibleSize = Director::getInstance()->getVisibleSize();
        log("Background: PosX=%.0f, CamOffset=%.0f, WorldWidth=%.0f, ScreenWidth=%.0f",
            this->getPositionX(), cameraOffsetX, _worldWidth, visibleSize.width);
    }
}

void ContinuousBackground::update(float delta) {
    Node::update(delta);
}