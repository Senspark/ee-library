//
//  EEUiButtonLoader.cpp
//  ee-library
//
//  Created by enrevol on 3/22/16.
//
//

#include "EEUiButtonLoader.hpp"
#include "EEUiWidgetCallback.hpp"

#include "cocos2d.h"
#include "ui/UIButton.h"

NS_EE_BEGIN
namespace property {
constexpr const char* block                         = "block";
constexpr const char* touch_enabled                 = "touchEnabled";
constexpr const char* swallow_touches               = "swallowTouches";
constexpr const char* propagate_touch_events        = "propagateTouchEvents";

constexpr const char* pressed_action_enabled        = "pressedActionEnabled";
constexpr const char* zoom_scale                    = "zoomScale";

constexpr const char* title_text                    = "titleText";
constexpr const char* title_font_name               = "titleFontName";
constexpr const char* title_font_size               = "titleFontSize";
constexpr const char* title_color                   = "titleColor";

constexpr const char* normal_sprite_frame_enabled   = "normalSpriteFrameEnabled";
constexpr const char* pressed_sprite_frame_enabled  = "pressedSpriteFrameEnabled";
constexpr const char* disabled_sprite_frame_enabled = "disabledSpriteFrameEnabled";

constexpr const char* normal_sprite_frame           = "normalSpriteFrame";
constexpr const char* pressed_sprite_frame          = "pressedSpriteFrame";
constexpr const char* disabled_sprite_frame         = "disabledSpriteFrame";
} // namespace property.

cocos2d::Node* UiButtonLoader::createNode(cocos2d::Node* parent,
                                          cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Button::create();
    result->setScale9Enabled(true);
    return result;
}

cocos2d::SpriteFrame* UiButtonLoader::parsePropTypeSpriteFrame(cocos2d::Node* node,
                                                               cocos2d::Node* parent,
                                                               cocosbuilder::CCBReader* reader,
                                                               const char* propertyName) {
    auto spriteSheet = reader->readCachedString();
    auto spriteFile = reader->readCachedString();
    cocos2d::SpriteFrame* spriteFrame = nullptr;
    if (spriteFrameEnabled_ && spriteFile.empty() == false) {
        if (spriteSheet.empty()) {
            spriteFile = reader->getCCBRootPath() + spriteFile;
            spriteFrameName_ = spriteFile;
            textureResType_ = cocos2d::ui::Widget::TextureResType::LOCAL;
            auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(spriteFile.c_str());
            if (texture != nullptr) {
                auto bounds = cocos2d::Rect(0, 0,
                                            texture->getContentSize().width,
                                            texture->getContentSize().height);
                spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, bounds);
            }
        } else {
            auto frameCache = cocos2d::SpriteFrameCache::getInstance();
            spriteSheet = reader->getCCBRootPath() + spriteSheet;
            // Load the sprite sheet only if it is not loaded.
            if (reader->getLoadedSpriteSheet().find(spriteSheet) == reader->getLoadedSpriteSheet().end()) {
                frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                reader->getLoadedSpriteSheet().insert(spriteSheet);
            }
            spriteFrame = frameCache->getSpriteFrameByName(spriteFile.c_str());
            spriteFrameName_ = spriteFile;
            textureResType_ = cocos2d::ui::Widget::TextureResType::PLIST;
        }
        if (reader->getAnimatedProperties()->find(propertyName) != reader->getAnimatedProperties()->end()) {
            reader->getAnimationManager()->setObject(spriteFrame, node, propertyName);
        }
    }
    return spriteFrame;
}

void UiButtonLoader::onHandlePropTypeBlock(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocosbuilder::BlockData* blockData,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::block) == 0) {
        auto&& touchCallback = detail::UiWidgetCallback::getInstance()->getActiveTouchCallback();
        auto&& clickCallback = detail::UiWidgetCallback::getInstance()->getActiveClickCallback();
        if (touchCallback) {
            button->addTouchEventListener(touchCallback);
            detail::UiWidgetCallback::getInstance()->setActiveTouchCallback(nullptr);
        } else if (clickCallback) {
            button->addClickEventListener(clickCallback);
            detail::UiWidgetCallback::getInstance()->setActiveClickCallback(nullptr);
        } else {
            CCASSERT(false, "Unexpected value.");
        }
        return;
    }
    NodeLoader::onHandlePropTypeBlock(node, parent, propertyName, blockData, reader);
}

void UiButtonLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           bool check,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::touch_enabled) == 0) {
        return button->setTouchEnabled(check);
    }
    if (std::strcmp(propertyName, property::swallow_touches) == 0) {
        return button->setSwallowTouches(check);
    }
    if (std::strcmp(propertyName, property::propagate_touch_events) == 0) {
        return button->setPropagateTouchEvents(check);
    }
    if (std::strcmp(propertyName, property::pressed_action_enabled) == 0) {
        return button->setPressedActionEnabled(check);
    }
    if (std::strcmp(propertyName, property::normal_sprite_frame_enabled) == 0 ||
        std::strcmp(propertyName, property::pressed_sprite_frame_enabled) == 0 ||
        std::strcmp(propertyName, property::disabled_sprite_frame_enabled) == 0) {
        spriteFrameEnabled_ = check;
        return;
    }
    NodeLoader::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void UiButtonLoader::onHandlePropTypeString(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            const char* string,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::title_text) == 0) {
        return button->setTitleText(string);
    }
    NodeLoader::onHandlePropTypeString(node, parent, propertyName, string, reader);
}

void UiButtonLoader::onHandlePropTypeFontTTF(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             const char* fontTTF,
                                             cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::title_font_name) == 0) {
        return button->setTitleFontName(fontTTF);
    }
    NodeLoader::onHandlePropTypeFontTTF(node, parent, propertyName, fontTTF, reader);
}

void UiButtonLoader::onHandlePropTypeFloat(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           float floatValue,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::zoom_scale) == 0) {
        return button->setZoomScale(floatValue);
    }
    NodeLoader::onHandlePropTypeFloat(node, parent, propertyName, floatValue, reader);
}

void UiButtonLoader::onHandlePropTypeFloatScale(cocos2d::Node* node,
                                                cocos2d::Node* parent,
                                                const char* propertyName,
                                                float floatScale,
                                                cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::title_font_size) == 0) {
        return button->setTitleFontSize(floatScale);
    }
    NodeLoader::onHandlePropTypeFloatScale(node, parent, propertyName, floatScale, reader);
}

void UiButtonLoader::onHandlePropTypeSpriteFrame(cocos2d::Node* node,
                                                 cocos2d::Node* parent,
                                                 const char* propertyName,
                                                 cocos2d::SpriteFrame* spriteFrame,
                                                 cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (spriteFrameEnabled_) {
        CCASSERT(spriteFrameName_.empty() == false,
                 "Sprite frame name is empty!");
    }
    if (std::strcmp(propertyName, property::normal_sprite_frame) == 0) {
        if (spriteFrameEnabled_) {
            button->loadTextureNormal(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (std::strcmp(propertyName, property::pressed_sprite_frame) == 0) {
        if (spriteFrameEnabled_) {
            button->loadTexturePressed(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (std::strcmp(propertyName, property::disabled_sprite_frame) == 0) {
        if (spriteFrameEnabled_) {
            button->loadTextureDisabled(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (spriteFrameEnabled_) {
        CCASSERT(false, "Sprite frame is not loaded!");
    }
    NodeLoader::onHandlePropTypeSpriteFrame(node, parent, propertyName, spriteFrame, reader);
}

void UiButtonLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            cocos2d::Color3B color3B,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, property::title_color) == 0) {
        return button->setTitleColor(color3B);
    }
    NodeLoader::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
}
NS_EE_END