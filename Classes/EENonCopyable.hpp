//
//  NonCopyable.hpp
//  test-3.8.1
//
//  Created by enrevol on 10/20/15.
//
//

#ifndef EE_LIBRARY_NON_COPYABLE_HPP
#define EE_LIBRARY_NON_COPYABLE_HPP

#include "EEMacro.hpp"

namespace_ee_begin
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = default;
};
namespace_ee_end

#endif /* EE_LIBRARY_NON_COPYABLE_HPP */
