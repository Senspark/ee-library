//
//  EEProjection-Impl.hpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#ifndef EE_LIBRARY_PROJECTION_IMPL_HPP_
#define EE_LIBRARY_PROJECTION_IMPL_HPP_

#include "EEProjection.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_detail_begin
class Projection::Impl : public Projection {
public:
    cocos2d::Size _horizonSize;
    cocos2d::Size _viewSize;
};
namespace_detail_end
namespace_ee_end

#endif /* EE_LIBRARY_PROJECTION_IMPL_HPP_ */
