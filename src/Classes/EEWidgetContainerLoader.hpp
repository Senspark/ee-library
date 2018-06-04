//
//  EEWidgetInternal.hpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#ifndef EE_LIBRARY_WIDGET_CONTAINER_LOADER_HPP
#define EE_LIBRARY_WIDGET_CONTAINER_LOADER_HPP

#include "EEUiWidgetLoader.hpp"

namespace ee {
namespace ui {
class WidgetContainerLoader : public UiWidgetLoader {
private:
    using Self = WidgetContainerLoader;
    using Super = UiWidgetLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Self, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual cocos2d::Point
    parsePropTypePosition(cocos2d::Node* node, cocos2d::Node* parent,
                          cocosbuilder::CCBReader* reader,
                          const char* name) override;

    virtual cocos2d::Size
    parsePropTypeSize(cocos2d::Node* node, cocos2d::Node* parent,
                      cocosbuilder::CCBReader* reader) override;
};
} // namespace ui
} // namespace ee

#endif /* EE_LIBRARY_WIDGET_CONTAINER_LOADER_HPP */
