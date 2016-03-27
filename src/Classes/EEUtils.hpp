//
//  EEUtils.hpp
//  roll-eat
//
//  Created by Hoang Le Hai on 7/22/15.
//
//

#ifndef EE_LIBRARY_UTILS_HPP_
#define EE_LIBRARY_UTILS_HPP_

#include <sstream>

#include "EEMacro.hpp"

NS_EE_BEGIN
template<class... Args>
std::string toString(Args&&... args) {
    static std::stringstream ss;
    ss.str(std::string());
    ss.clear();
    (ss << ... << std::forward<Args>(args));
    return ss.str();
}
NS_EE_END

#endif /* EE_LIBRARY_UTILS_HPP_ */