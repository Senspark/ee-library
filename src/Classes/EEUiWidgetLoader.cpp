//
//  EEUiWidgetLoader.cpp
//  ee-library
//
//  Created by Zinge on 5/5/16.
//
//

#include "EEUiWidgetLoader.hpp"
#include "EEUiWidgetCallback.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
namespace property {
constexpr const char* enabled                       = "enabled";
constexpr const char* bright                        = "bright";
constexpr const char* highlighted                   = "highlighted";
constexpr const char* ignoreContentAdaptWithSize    = "ignoreContentAdaptWithSize";
constexpr const char* block                         = "block";
constexpr const char* touch_enabled                 = "touchEnabled";
constexpr const char* swallow_touches               = "swallowTouches";
constexpr const char* propagate_touch_events        = "propagateTouchEvents";
} // namespace property.

cocos2d::Node* UiWidgetLoader::createNode(cocos2d::Node* parent,
                                          cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Widget::create();
    return result;
}

void UiWidgetLoader::onHandlePropTypeBlock(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocosbuilder::BlockData* blockData,
                                           cocosbuilder::CCBReader* reader) {
    auto widget = dynamic_cast<cocos2d::ui::Widget*>(node);
    std::string propName(propertyName);
    if (propName == property::block) {
        if (blockData != nullptr) {
            auto&& touchCallback = detail::UiWidgetCallback::getInstance()->getActiveTouchCallback();
            auto&& clickCallback = detail::UiWidgetCallback::getInstance()->getActiveClickCallback();
            if (touchCallback) {
                widget->addTouchEventListener(touchCallback);
                detail::UiWidgetCallback::getInstance()->setActiveTouchCallback(nullptr);
            } else if (clickCallback) {
                widget->addClickEventListener(clickCallback);
                detail::UiWidgetCallback::getInstance()->setActiveClickCallback(nullptr);
            } else {
                CCASSERT(false, "Unexpected value.");
            }
        }
        return;
    }
    NodeLoader::onHandlePropTypeBlock(node, parent, propertyName,
                                      blockData, reader);
}

void UiWidgetLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           bool check,
                                           cocosbuilder::CCBReader* reader) {
    auto widget = dynamic_cast<cocos2d::ui::Widget*>(node);
    std::string propName(propertyName);
    if (propName == property::enabled) {
        return widget->setEnabled(check);
    }
    if (propName == property::bright) {
        return widget->setBright(check);
    }
    if (propName == property::highlighted) {
        return widget->setHighlighted(check);
    }
    if (propName == property::ignoreContentAdaptWithSize) {
        return widget->ignoreContentAdaptWithSize(check);
    }
    if (propName == property::touch_enabled) {
        return widget->setTouchEnabled(check);
    }
    if (propName == property::swallow_touches) {
        return widget->setSwallowTouches(check);
    }
    if (propName == property::propagate_touch_events) {
        return widget->setPropagateTouchEvents(check);
    }
    NodeLoader::onHandlePropTypeCheck(node, parent, propertyName,
                                      check, reader);
}
NS_EE_END