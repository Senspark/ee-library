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
/// Faster and safer implementation alternative
/// to cocos2d::StringUtils::format
/// by using @c static @c std::stringstream.
///
/// Supports multiple arguments.
///
/// Example:
/// @code
/// /// Alternative to cocos2d::StringUtils::format("%02d:%02d:%02d",
///                                                 hours,
///                                                 minutes,
///                                                 seconds);
/// ee::toString(std::setfill('0'),
///              std::setw(2), hours, ":",
///              std::setw(2), minutes, ":",
///              std::setw(2), seconds);
/// @endcode
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