//
//  EEBlurBackgroundLoader.cpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

#include "EEBlurBackgroundLoader.hpp"
#include "EEBlurBackground.hpp"

NS_EE_BEGIN
namespace property {
constexpr const char* render_scale = "renderScale";
} // namespace property

cocos2d::Node*
BlurBackgroundLoader::createNode(cocos2d::Node* parent,
                                 cocosbuilder::CCBReader* reader) {
    return BlurBackground::create();
}

void BlurBackgroundLoader::onHandlePropTypeFloat(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float floatValue, cocosbuilder::CCBReader* reader) {
    auto background = dynamic_cast<BlurBackground*>(node);
    std::string prop{propertyName};
    if (prop == property::render_scale) {
        return background->setRenderScale(floatValue);
    }
    return NodeV3Loader::onHandlePropTypeFloat(node, parent, propertyName,
                                               floatValue, reader);
}
NS_EE_END