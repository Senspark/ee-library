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
NS_ANONYMOUS_BEGIN
constexpr const char* PROPERTY_ENABLED                              = "enabled";
constexpr const char* PROPERTY_SELECTED                             = "selected";
constexpr const char* PROPERTY_CCCONTROL                            = "ccControl";
constexpr const char* PROPERTY_ZOOMONTOUCHDOWN                      = "zoomOnTouchDown";
constexpr const char* PROPERTY_TITLE_NORMAL                         = "title|1";
constexpr const char* PROPERTY_TITLE_HIGHLIGHTED                    = "title|2";
constexpr const char* PROPERTY_TITLE_DISABLED                       = "title|3";
constexpr const char* PROPERTY_TITLECOLOR_NORMAL                    = "titleColor|1";
constexpr const char* PROPERTY_TITLECOLOR_HIGHLIGHTED               = "titleColor|2";
constexpr const char* PROPERTY_TITLECOLOR_DISABLED                  = "titleColor|3";
constexpr const char* PROPERTY_TITLETTF_NORMAL                      = "titleTTF|1";
constexpr const char* PROPERTY_TITLETTF_HIGHLIGHTED                 = "titleTTF|2";
constexpr const char* PROPERTY_TITLETTF_DISABLED                    = "titleTTF|3";
constexpr const char* PROPERTY_TITLETTFSIZE_NORMAL                  = "titleTTFSize|1";
constexpr const char* PROPERTY_TITLETTFSIZE_HIGHLIGHTED             = "titleTTFSize|2";
constexpr const char* PROPERTY_TITLETTFSIZE_DISABLED                = "titleTTFSize|3";
constexpr const char* PROPERTY_LABELANCHORPOINT                     = "labelAnchorPoint";
// TODO: Should be "preferredSize". This is a typo in cocos2d-iphone, cocos2d-x and CocosBuilder!
constexpr const char* PROPERTY_PREFEREDSIZE                         = "preferedSize";
constexpr const char* PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL         = "backgroundSpriteFrame|1";
constexpr const char* PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED    = "backgroundSpriteFrame|2";
constexpr const char* PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED       = "backgroundSpriteFrame|3";
NS_ANONYMOUS_END

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
    if (spriteFile.empty() == false) {
        if (spriteSheet.empty()) {
            spriteFile = reader->getCCBRootPath() + spriteFile;
            _spriteFrameName = spriteFile;
            _textureResType = cocos2d::ui::Widget::TextureResType::LOCAL;
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
            _spriteFrameName = spriteFile;
            _textureResType = cocos2d::ui::Widget::TextureResType::PLIST;
        }
        if (reader->getAnimatedProperties()->find(propertyName) != reader->getAnimatedProperties()->end()) {
            reader->getAnimationManager()->setObject(spriteFrame, node, propertyName);
        }
    }
    return spriteFrame;
}

void UiButtonLoader::onHandlePropTypeBlockControl(cocos2d::Node* node,
                                                  cocos2d::Node* parent,
                                                  const char* propertyName,
                                                  cocosbuilder::BlockControlData* blockControlData,
                                                  cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_CCCONTROL) == 0) {
        // Backward-compability way.
        // auto handler = blockControlData->mSELControlHandler;
        // auto target = blockControlData->_target;
        // auto events = blockControlData->mControlEvents;
        // CCASSERT(events == cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE, "Unexpected value.");
        // button->addTouchEventListener([handler, target, events](cocos2d::Ref* sender,
        //                                                         cocos2d::ui::Widget::TouchEventType eventType) {
        //     if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
        //         (target->*handler)(sender, cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
        //     }
        // });
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
    } else {
        NodeLoader::onHandlePropTypeBlockControl(node, parent, propertyName,
                                                 blockControlData, reader);
    }
}

void UiButtonLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           bool check,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_ZOOMONTOUCHDOWN) == 0) {
        button->setPressedActionEnabled(check);
    } else if (std::strcmp(propertyName, PROPERTY_ENABLED) == 0) {
        button->setEnabled(check);
    } else if (std::strcmp(propertyName, PROPERTY_SELECTED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else {
        NodeLoader::onHandlePropTypeCheck(node, parent, propertyName,
                                          check, reader);
    }
}

void UiButtonLoader::onHandlePropTypeString(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            const char* string,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_TITLE_NORMAL) == 0) {
        button->setTitleText(string);
    } else if (std::strcmp(propertyName, PROPERTY_TITLE_HIGHLIGHTED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else if (std::strcmp(propertyName, PROPERTY_TITLE_DISABLED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else {
        NodeLoader::onHandlePropTypeString(node, parent, propertyName,
                                           string, reader);
    }
}

void UiButtonLoader::onHandlePropTypeFontTTF(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             const char* fontTTF,
                                             cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_TITLETTF_NORMAL) == 0) {
        button->setTitleFontName(fontTTF);
    } else if (std::strcmp(propertyName, PROPERTY_TITLETTF_HIGHLIGHTED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else if (std::strcmp(propertyName, PROPERTY_TITLETTF_DISABLED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else {
        NodeLoader::onHandlePropTypeFontTTF(node, parent, propertyName,
                                            fontTTF, reader);
    }
}

void UiButtonLoader::onHandlePropTypeFloatScale(cocos2d::Node* node,
                                                cocos2d::Node* parent,
                                                const char* propertyName,
                                                float floatScale,
                                                cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_TITLETTFSIZE_NORMAL) == 0) {
        button->setTitleFontSize(floatScale);
    } else if (std::strcmp(propertyName, PROPERTY_TITLETTFSIZE_HIGHLIGHTED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else if (std::strcmp(propertyName, PROPERTY_TITLETTFSIZE_DISABLED) == 0) {
        CCASSERT(false, "Unexpected arg.");
    } else {
        NodeLoader::onHandlePropTypeFloatScale(node, parent, propertyName,
                                               floatScale, reader);
    }
}

void UiButtonLoader::onHandlePropTypePoint(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocos2d::Vec2 point,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_LABELANCHORPOINT) == 0) {
        button->getTitleRenderer()->setAnchorPoint(point);
    } else {
        NodeLoader::onHandlePropTypePoint(node, parent, propertyName,
                                          point, reader);
    }
}

void UiButtonLoader::onHandlePropTypeSize(cocos2d::Node* node,
                                          cocos2d::Node* parent,
                                          const char* propertyName,
                                          cocos2d::Size size,
                                          cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_PREFEREDSIZE) == 0) {
        button->setContentSize(size);
    } else {
        NodeLoader::onHandlePropTypeSize(node, parent, propertyName,
                                         size, reader);
    }
}

void UiButtonLoader::onHandlePropTypeSpriteFrame(cocos2d::Node* node,
                                                 cocos2d::Node* parent,
                                                 const char* propertyName,
                                                 cocos2d::SpriteFrame* spriteFrame,
                                                 cocosbuilder::CCBReader* reader) {
    if (spriteFrame == nullptr) {
        LOG_FUNC_FORMAT("Failed to load spriteFrame: name = %s",
                        _spriteFrameName.c_str());
        return;
    }
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL) == 0) {
        CCASSERT(_spriteFrameName.empty() == false, "Unexpected value.");
        button->loadTextureNormal(_spriteFrameName, _textureResType);
    } else if (std::strcmp(propertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED) == 0) {
        CCASSERT(_spriteFrameName.empty() == false, "Unexpected value.");
        button->loadTexturePressed(_spriteFrameName, _textureResType);
    } else if (std::strcmp(propertyName, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED) == 0) {
        CCASSERT(_spriteFrameName.empty() == false, "Unexpected value.");
        button->loadTextureDisabled(_spriteFrameName, _textureResType);
    } else {
        NodeLoader::onHandlePropTypeSpriteFrame(node, parent, propertyName,
                                                spriteFrame, reader);
    }
}

void UiButtonLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            cocos2d::Color3B color3B,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    if (std::strcmp(propertyName, PROPERTY_TITLECOLOR_NORMAL) == 0) {
        button->setTitleColor(color3B);
    } else if(strcmp(propertyName, PROPERTY_TITLECOLOR_HIGHLIGHTED) == 0) {
        // Ignored.
    } else if(strcmp(propertyName, PROPERTY_TITLECOLOR_DISABLED) == 0) {
        // Ignored.
    } else {
        NodeLoader::onHandlePropTypeColor3(node, parent, propertyName,
                                           color3B, reader);
    }
}
NS_EE_END