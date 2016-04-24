//
//  EEUtils.hpp
//  roll-eat
//
//  Created by Hoang Le Hai on 7/22/15.
//
//

#ifndef EE_LIBRARY_UTILS_HPP_
#define EE_LIBRARY_UTILS_HPP_

#include <functional>
#include <sstream>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEExtension.hpp"

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

/// Performs an action recursively on a node.
void doRecursively(cocos2d::Node* node,
                   const std::function<void(cocos2d::Node*)>& action);

/// @c doRecursively with member function pointer version.
template<
    class FunctionPointer,
    class... Args
>
std::enable_if_t<
    is_member_function_pointer_v<FunctionPointer>
>
doRecursively(cocos2d::Node* node,
              FunctionPointer&& ptr,
              Args&&... args) {
    doRecursively(node, [&](cocos2d::Node* n) {
        (n->*ptr)(args...);
    });
}

/// Pauses the given node and all of its sub-children.
void pauseAll(cocos2d::Node* node);

/// Resumes the given node and all of its sub-children.
void resumeAll(cocos2d::Node* node);

/// Checks whether the node is actually visible.
///
/// A button is actually visible means that it and its ancestor nodes
/// are visible.
bool isActuallyVisible(const cocos2d::Node* node);
NS_EE_END

#endif /* EE_LIBRARY_UTILS_HPP_ */