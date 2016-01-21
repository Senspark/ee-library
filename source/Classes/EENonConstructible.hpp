//
//  EEConstructible.hpp
//  ee-library
//
//  Created by enrevol on 10/27/15.
//
//

#ifndef EE_LIBRARY_NON_CONSTRUCTIBLE_HPP_
#define EE_LIBRARY_NON_CONSTRUCTIBLE_HPP_

#include "EEMacro.hpp"

namespace_ee_begin
class NonConstructible {
public:
    constexpr NonConstructible() = delete;
};
namespace_ee_end

#endif /* EE_LIBRARY_NON_CONSTRUCTIBLE_HPP_ */
