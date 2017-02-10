//
//  EEButton.cpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#include "EEButton.hpp"
#include "EEUtils.hpp"
#include "EEScale9SpriteWithHsv.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
auto ButtonEx::getDefaultHitTester() -> const HitTester & {
    static const HitTester checker = [](cocos2d::Touch* touch, Button* button) {
        // Retrieve world position.
        auto&& position = touch->getLocation();

        // Convert to local position.
        auto&& localPosition = button->convertToNodeSpace(position);

        // Retrieve local bounding box.
        auto&& contentSize = button->getContentSize();

        return (0 <= localPosition.x && localPosition.x <= contentSize.width &&
                0 <= localPosition.y && localPosition.y <= contentSize.height);
    };
    return checker;
}

ButtonEx::ButtonEx()
    : zoomingDuration_(0.05f)
    , currentTouch_(nullptr)
    , currentEvent_(nullptr)
    , container_(nullptr) {}

ButtonEx::~ButtonEx() = default;

ButtonEx* ButtonEx::create() {
    auto result = new (std::nothrow) ButtonEx();
    if (result != nullptr && result->init()) {
        result->autorelease();
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

ButtonEx* ButtonEx::create(const std::string& normalImage,
                           const std::string& selectedImage,
                           const std::string& disableImage,
                           TextureResType texType) {
    auto result = new (std::nothrow) ButtonEx();
    if (result != nullptr &&
        result->init(normalImage, selectedImage, disableImage, texType)) {
        result->autorelease();
        result->setScale9Enabled(true);
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

void ButtonEx::addChild(Node* child, int localZOrder, int tag) {
    // Forward to the container.
    container_->addChild(child, localZOrder, tag);
}

void ButtonEx::addChild(Node* child, int localZOrder, const std::string& name) {
    // Forward to the container.
    container_->addChild(child, localZOrder, name);
}

auto ButtonEx::getChildByTag(int tag) const -> Node * {
    // Forward to the container.
    return container_->getChildByTag(tag);
}

auto ButtonEx::getChildByName(const std::string& name) const -> Node * {
    // Forward to the container.
    return container_->getChildByName(name);
}

auto ButtonEx::getChildren() -> cocos2d::Vector<Node*> & {
    // Forward to the container.
    return container_->getChildren();
}

auto ButtonEx::getChildren() const -> const cocos2d::Vector<Node*> & {
    // Forward to the container.
    return container_->getChildren();
}

ssize_t ButtonEx::getChildrenCount() const {
    // Forward to the container.
    return container_->getChildrenCount();
}

void ButtonEx::removeChild(Node* child, bool cleanup) {
    // Forward to the container.
    container_->removeChild(child, cleanup);
}

void ButtonEx::removeAllChildrenWithCleanup(bool cleanup) {
    // Forward to the container.
    container_->removeAllChildrenWithCleanup(cleanup);
}

void ButtonEx::reorderChild(Node* child, int localZOrder) {
    if (child == container_) {
        Super::reorderChild(child, localZOrder);
    } else {
        // Forward to the container.
        container_->reorderChild(child, localZOrder);
    }
}

void ButtonEx::sortAllChildren() {
    // Forward to the container.
    container_->sortAllChildren();
}

void ButtonEx::setContentSize(const cocos2d::Size& contentSize) {
    Super::setContentSize(contentSize);
}

void ButtonEx::setCascadeColorEnabled(bool enabled) {
    Super::setCascadeColorEnabled(enabled);
    container_->setCascadeColorEnabled(enabled);
}

void ButtonEx::setCascadeOpacityEnabled(bool enabled) {
    Super::setCascadeOpacityEnabled(enabled);
    container_->setCascadeOpacityEnabled(enabled);
}

bool ButtonEx::hitTest(const cocos2d::Point& point,
                       const cocos2d::Camera* camera, cocos2d::Vec3* p) const {
    // Forward to the container.
    return container_->hitTest(point, camera, p);
}

bool ButtonEx::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    CC_ASSERT(currentTouch_ == nullptr);
    CC_ASSERT(currentEvent_ == nullptr);
    currentTouch_ = touch;
    currentEvent_ = event;
    bool result = Super::onTouchBegan(touch, event);
    currentTouch_ = nullptr;
    currentEvent_ = nullptr;
    return result;
}

void ButtonEx::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (touch->getDelta() == cocos2d::Vec2::ZERO) {
        return;
    }
    CC_ASSERT(currentTouch_ == nullptr);
    CC_ASSERT(currentEvent_ == nullptr);
    currentTouch_ = touch;
    currentEvent_ = event;
    Super::onTouchMoved(touch, event);
    currentTouch_ = nullptr;
    currentEvent_ = nullptr;
}

void ButtonEx::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    CC_ASSERT(currentTouch_ == nullptr);
    CC_ASSERT(currentEvent_ == nullptr);
    currentTouch_ = touch;
    currentEvent_ = event;
    Super::onTouchEnded(touch, event);
    currentTouch_ = nullptr;
    currentEvent_ = nullptr;
}

void ButtonEx::setScale9Enabled(bool enable) {
    Super::setScale9Enabled(enable);
}

std::string ButtonEx::getDescription() const { return "ee::ButtonEx"; }

void ButtonEx::setZoomingDuration(float duration) noexcept {
    zoomingDuration_ = duration;
}

float ButtonEx::getZoomingDuration() const noexcept { return zoomingDuration_; }

void ButtonEx::pressedStateBrightness(float brightness) {
    getRendererClicked()->setBrightness(brightness);
}

void ButtonEx::setTouchBeganCallback(const TouchCallback& callback) {
    addTouchEventListener([callback, this](cocos2d::Ref*, TouchEventType type) {
        if (type == TouchEventType::BEGAN) {
            callback(currentTouch_, currentEvent_);
        }
    });
}

void ButtonEx::setTouchMovedCallback(const TouchCallback& callback) {
    addTouchEventListener([callback, this](cocos2d::Ref*, TouchEventType type) {
        if (type == TouchEventType::MOVED) {
            callback(currentTouch_, currentEvent_);
        }
    });
}

void ButtonEx::setTouchEndedCallback(const TouchCallback& callback) {
    addTouchEventListener([callback, this](cocos2d::Ref*, TouchEventType type) {
        if (type == TouchEventType::ENDED) {
            callback(currentTouch_, currentEvent_);
        }
    });
}

auto ButtonEx::getContainer() const noexcept -> const Widget * {
    return container_;
}

auto ButtonEx::getContainer() noexcept -> Widget * { return container_; }

bool ButtonEx::init() {
    if (not Super::init()) {
        return false;
    }
    return true;
}

bool ButtonEx::init(const std::string& normalImage,
                    const std::string& selectedImage,
                    const std::string& disableImage, TextureResType texType) {
    if (not Super::init(normalImage, selectedImage, disableImage, texType)) {
        return false;
    }
    return true;
}

void ButtonEx::initRenderer() {
    createTitleRenderer();

    removeProtectedChild(_titleRenderer);

    _buttonNormalRenderer = Scale9SpriteWithHsv::create();
    _buttonClickedRenderer = Scale9SpriteWithHsv::create();
    _buttonDisabledRenderer = Scale9SpriteWithHsv::create();
    _buttonClickedRenderer->setRenderingType(
        cocos2d::ui::Scale9Sprite::RenderingType::SIMPLE);
    _buttonNormalRenderer->setRenderingType(
        cocos2d::ui::Scale9Sprite::RenderingType::SIMPLE);
    _buttonDisabledRenderer->setRenderingType(
        cocos2d::ui::Scale9Sprite::RenderingType::SIMPLE);

    container_ = Widget::create();
    container_->ignoreContentAdaptWithSize(false);
    container_->setSwallowTouches(false);
    container_->setPropagateTouchEvents(true);
    container_->setFocusEnabled(false);
    container_->setPositionType(PositionType::PERCENT);
    container_->setPositionPercent(cocos2d::Vec2(0.5f, 0.5f));
    container_->setSizeType(SizeType::PERCENT);
    container_->setSizePercent(cocos2d::Vec2(1.0f, 1.0f));

    addProtectedChild(container_, std::numeric_limits<int>::min());

    container_->addProtectedChild(_buttonNormalRenderer, -2, -1);
    container_->addProtectedChild(_buttonClickedRenderer, -2, -1);
    container_->addProtectedChild(_buttonDisabledRenderer, -2, -1);
    container_->addProtectedChild(_titleRenderer);
}

void ButtonEx::onPressStateChangedToNormal() {
    // Hide all sprites.
    _buttonNormalRenderer->setVisible(false);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisabledRenderer->setVisible(false);

    if (_normalTextureLoaded) {
        _buttonNormalRenderer->setVisible(true);
        getRendererNormal()->setSaturation(1.0f);
    } else {
        // None background.
    }

    container_->stopAllActions();

    if (_pressedActionEnabled) {
        // Zooming is enabled.
        auto zoomAction = cocos2d::ScaleTo::create(zoomingDuration_, 1.0f);
        container_->runAction(zoomAction);
    } else {
        container_->setScale(1.0f);
    }
}

void ButtonEx::onPressStateChangedToPressed() {
    // Hide all sprites.
    _buttonNormalRenderer->setVisible(false);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisabledRenderer->setVisible(false);

    if (_pressedTextureLoaded) {
        // Pressed texture is loaded.
        // Show pressed sprite.
        _buttonClickedRenderer->setVisible(true);
    } else {
        // Pressed texture is not loaded.
        if (_normalTextureLoaded) {
            // Use normal texture instead.
            _buttonNormalRenderer->setVisible(true);
            getRendererNormal()->setSaturation(1.0f);
        } else {
            // None background.
        }
    }

    container_->stopAllActions();

    if (_pressedActionEnabled) {
        // Zooming is enabled.
        auto zoomAction =
            cocos2d::ScaleTo::create(zoomingDuration_, 1.0f + _zoomScale);
        container_->runAction(zoomAction);
    } else {
        // Instantly scaled.
        container_->setScale(1.0f + _zoomScale);
    }
}

void ButtonEx::onPressStateChangedToDisabled() {
    // Hide all sprites.
    _buttonNormalRenderer->setVisible(false);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisabledRenderer->setVisible(false);

    if (_disabledTextureLoaded) {
        // Disabled texture is loaded.
        // Show the disabled texture.
        _buttonDisabledRenderer->setVisible(true);
    } else {
        // Disabled texture is not loaded.
        if (_normalTextureLoaded) {
            // Normal texture is loaded.
            // Use the normal texture instead.
            // Gray out the normal sprite.
            _buttonNormalRenderer->setVisible(true);
            getRendererNormal()->setSaturation(0.0f);
        } else {
            // None background.
        }
    }

    // Stop animation and reset scale.
    container_->stopAllActions();
    container_->setScale(1.0f);
}

void ButtonEx::adaptRenderers() {
    updateTexture();
    Super::adaptRenderers();
}

cocos2d::ui::Widget* ButtonEx::createCloneInstance() { return create(); }

void ButtonEx::copySpecialProperties(Widget* model) {
    auto button = dynamic_cast<ButtonEx*>(model);
    if (button != nullptr) {
        Super::copySpecialProperties(model);
        setZoomingDuration(button->getZoomingDuration());
    }
}

void ButtonEx::visit(cocos2d::Renderer* renderer,
                     const cocos2d::Mat4& parentTransform,
                     std::uint32_t parentFlags) {
    adaptRenderers();
    Super::visit(renderer, parentTransform, parentFlags);
}

void ButtonEx::updateTexture() {
    int counter = 0;

    if (_normalTextureAdaptDirty && isBright() &&
        _brightStyle == BrightStyle::NORMAL) {
        onPressStateChangedToNormal();
        ++counter;
    }
    if (_pressedTextureAdaptDirty && isBright() &&
        _brightStyle == BrightStyle::HIGHLIGHT) {
        onPressStateChangedToPressed();
        ++counter;
    }
    if (_disabledTextureAdaptDirty && not isBright()) {
        onPressStateChangedToDisabled();
        ++counter;
    }

    CC_ASSERT(counter <= 1);
}

Scale9SpriteWithHsv* ButtonEx::getRendererNormal() const {
    return dynamic_cast<Scale9SpriteWithHsv*>(_buttonNormalRenderer);
}

Scale9SpriteWithHsv* ButtonEx::getRendererClicked() const {
    return dynamic_cast<Scale9SpriteWithHsv*>(_buttonClickedRenderer);
}

Scale9SpriteWithHsv* ButtonEx::getRendererDisabled() const {
    return dynamic_cast<Scale9SpriteWithHsv*>(_buttonDisabledRenderer);
}
NS_DETAIL_END
NS_EE_END
