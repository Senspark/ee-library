//
//  EEProjection.hpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#ifndef EE_LIBRARY_PROJECTION_HPP_
#define EE_LIBRARY_PROJECTION_HPP_

#include "EEMacro.hpp"
#include "EEForward.hpp"

#include <utility>

namespace_ee_begin
namespace_detail_begin
class Projection {
public:
    void setHorizonSize(const cocos2d::Size& size);
    void setViewSize(const cocos2d::Size& size);
    
    const cocos2d::Size& getHorizonSize() const;
    const cocos2d::Size& getViewSize() const;
    
    cocos2d::Rect query(float eyeOffset, const cocos2d::Point& eyePosition, float layerOffset) const;
    
private:
    class Impl;
};
namespace_detail_end
namespace_ee_end

#endif /* EE_LIBRARY_PROJECTION_HPP_ */
