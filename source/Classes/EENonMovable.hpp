//
//  EENonMovable.hpp
//  ee-library
//
//  Created by enrevol on 10/27/15.
//
//

#ifndef EE_LIBRARY_NON_MOVABLE_HPP_
#define EE_LIBRARY_NON_MOVABLE_HPP_

#include "EEMacro.hpp"

namespace_ee_begin
class NonMovable {
public:
    constexpr NonMovable() = default;
    NonMovable(const NonMovable&) = default;
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(const NonMovable&) = default;
    NonMovable& operator=(NonMovable&&) = delete;
};
namespace_ee_end

#endif /* EE_LIBRARY_NON_MOVABLE_HPP_ */
