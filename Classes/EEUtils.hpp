//
//  EEUtils.hpp
//  roll-eat
//
//  Created by Hoang Le Hai on 7/22/15.
//
//

#ifndef EE_LIBRARY_UTILS_H
#define EE_LIBRARY_UTILS_H

#include "EEMacro.hpp"

#include <sstream>

namespace_ee_begin
namespace_detail_begin
template<class T>
void toStringHelper(std::stringstream& ss, T&& value) {
    ss << value;
}

template<class T, class... Args>
void toStringHelper(std::stringstream& ss, T&& value, Args&&... args) {
    ss << std::forward<T>(value);
    toStringHelper(ss, std::forward<Args>(args)...);
}
namespace_detail_end

template<class... Args>
std::string toString(Args&&... args) {
    static std::stringstream ss;
    ss.str(std::string());
    ss.clear();
    detail::toStringHelper(ss, std::forward<Args>(args)...);
    return ss.str();
}
namespace_ee_end

#endif /* EE_LIBRARY_UTILS_H */