//
//  EEProjection.cpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#include "EEProjection-Impl.hpp"

namespace_ee_begin
namespace_detail_begin
void Projection::setHorizonSize(const cocos2d::Size& size) {
    auto impl = static_cast<Impl*>(this);
    impl->_horizonSize = size;
}

void Projection::setViewSize(const cocos2d::Size& size) {
    auto impl = static_cast<Impl*>(this);
    impl->_viewSize = size;
}

const cocos2d::Size& Projection::getHorizonSize() const {
    auto impl = static_cast<const Impl*>(this);
    return impl->_horizonSize;
}

const cocos2d::Size& Projection::getViewSize() const {
    auto impl = static_cast<const Impl*>(this);
    return impl->_viewSize;
}

cocos2d::Rect Projection::query(float eyeOffset, const cocos2d::Point& eyePosition, float layerOffset) const {
    auto impl = static_cast<const Impl*>(this);
    CC_ASSERT(layerOffset < 0);
    CC_ASSERT(eyeOffset < layerOffset);
    
    const auto horizonBox = cocos2d::Rect(0, 0, impl->_horizonSize.width, impl->_horizonSize.height);
    const float scale = (eyeOffset - layerOffset) / eyeOffset;
    const auto layerBox = cocos2d::Rect(eyePosition.x - (eyePosition.x - horizonBox.getMinX()) * scale,
                                        eyePosition.y - (eyePosition.y - horizonBox.getMinY()) * scale,
                                        horizonBox.size.width * scale,
                                        horizonBox.size.height * scale);
    
    const float scaleX = impl->_viewSize.width / layerBox.size.width;
    const float scaleY = impl->_viewSize.height / layerBox.size.height;
    
    const float viewScale = std::min(scaleX, scaleY);
    const auto newViewSize = viewScale * impl->_viewSize;
    
    auto viewBox = cocos2d::Rect(eyePosition.x - newViewSize.x * 0.5f,
                                 eyePosition.y - newViewSize.y * 0.5f,
                                 newViewSize.x,
                                 newViewSize.y);
    if (viewBox.getMinX() < layerBox.getMinX()) {
        viewBox.origin.x = layerBox.getMinX();
    }
    if (viewBox.getMaxX() > layerBox.getMaxX()) {
        viewBox.origin.x = layerBox.getMaxX() - viewBox.size.width;
    }
    if (viewBox.getMinY() < layerBox.getMinY()) {
        viewBox.origin.y = layerBox.getMinY();
    }
    if (viewBox.getMaxY() > layerBox.getMaxY()) {
        viewBox.origin.y = layerBox.getMaxY() - viewBox.size.height;
    }
    return viewBox;
}
namespace_detail_end
namespace_ee_end