//
//  EEWidgetInternal.cpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#include "EEWidgetContainerLoader.hpp"

#include <cocosbuilder/CCNode+CCBRelativePositioning.h>
#include <ui/UIWidget.h>

#include <EEWidget.hpp>

namespace ee {
namespace ui {
using Self = WidgetContainerLoader;

cocos2d::Node* Self::createNode(cocos2d::Node* parent,
                                cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Widget::create();
    return result;
}

cocos2d::Point Self::parsePropTypePosition(cocos2d::Node* node,
                                           cocos2d::Node* parent_,
                                           cocosbuilder::CCBReader* reader,
                                           const char* name) {
    auto x = reader->readFloat();
    auto y = reader->readFloat();

    auto type = static_cast<cocosbuilder::CCBReader::PositionType>(
        reader->readInt(false));

    auto parent = dynamic_cast<Widget*>(parent_);
    CC_ASSERT(parent != nullptr);
    auto containerSize = parent->getInnerContentSize();

    auto pt = cocosbuilder::getAbsolutePosition(cocos2d::Point(x, y), type,
                                                containerSize, name);
    node->setPosition(pt);
    if (reader->getAnimatedProperties()->find(name) !=
        reader->getAnimatedProperties()->end()) {
        cocos2d::ValueVector vec;
        vec.emplace_back(x);
        vec.emplace_back(y);
        vec.emplace_back(static_cast<int>(type));
        reader->getAnimationManager()->setBaseValue(cocos2d::Value(vec),
                                                    parent_, name);
    }
    return pt;
}

cocos2d::Size Self::parsePropTypeSize(cocos2d::Node* node,
                                      cocos2d::Node* parent_,
                                      cocosbuilder::CCBReader* reader) {
    auto width = reader->readFloat();
    auto height = reader->readFloat();

    auto type =
        static_cast<cocosbuilder::CCBReader::SizeType>(reader->readInt(false));

    auto parent = dynamic_cast<Widget*>(parent_);
    CC_ASSERT(parent != nullptr);
    auto containerSize = parent->getInnerContentSize();

    switch (type) {
    case cocosbuilder::CCBReader::SizeType::ABSOLUTE: {
        /* Nothing. */
        break;
    }
    case cocosbuilder::CCBReader::SizeType::RELATIVE_CONTAINER: {
        width = containerSize.width - width;
        height = containerSize.height - height;
        break;
    }
    case cocosbuilder::CCBReader::SizeType::PERCENT: {
        width = static_cast<int>(containerSize.width * width / 100.0f);
        height = static_cast<int>(containerSize.height * height / 100.0f);
        break;
    }
    case cocosbuilder::CCBReader::SizeType::HORIZONTAL_PERCENT: {
        width = static_cast<int>(containerSize.width * width / 100.0f);
        break;
    }
    case cocosbuilder::CCBReader::SizeType::VERTICAL_PERCENT: {
        height = static_cast<int>(containerSize.height * height / 100.0f);
        break;
    }
    case cocosbuilder::CCBReader::SizeType::MULTIPLY_RESOLUTION: {
        auto resolutionScale = cocosbuilder::CCBReader::getResolutionScale();
        width *= resolutionScale;
        height *= resolutionScale;
        break;
    }
    default:
        CCLOG("Unknown CCB type.");
        break;
    }

    return cocos2d::Size(width, height);
}
} // namespace ui
} // namespace ee
