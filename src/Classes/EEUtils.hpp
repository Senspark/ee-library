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
/// Reference guard.
///
/// Automatically retains instance on constructor
/// and releases instance on destructor.
///
/// Example:
/// @code
/// ee::RefGuard some_guard(this);
/// ee::RefGuard another_guard(some_instance);
/// auto my_guard = ee::makeRefGuard(another_instance);
/// @endcode
using RefGuard = cocos2d::RefPtr<cocos2d::Ref>;

/// @see @c RefGuard.
template<class T>
auto makeRefGuard(T&& instance) {
    return ReferenceGuard(instance);
}

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

/// Attempts to capture a screenshot in pixels, i.e. the device screen size.
///
/// @note       This method is slow because it uses @c glReadPixels,
///             which is synchronous.
/// @note       Prefer this function to @c captureScreenInPoints if you want to
///             capture a screenshot and share it via social networks.
/// @param      afterCaptured will be invoked after the capturing process
///             has done.
/// @see        @c captureScreenInPoints.
void captureScreenInPixels(const std::function<void(cocos2d::Image*)>& afterCaptured);

/// Attempts to capture a screenshot in points, i.e. the scene size
/// multiplied by content scale factor.
///
/// @note       This method is faster than @c captureScreenInPixels be cause it
///             uses FBO.
/// @note       If you are capturing the screenshot for blurring, set the scale
///             to be less than 1 for better performance.
/// @param      afterCaptured will be invoked after the capturing process
///             has done.
/// @param      scale Resulting image size will be multiplied by this parameter.
/// @see        @c captureScreenInPoints.
/// @warning    If you use a transition scene then you should not call this
///             function in @c onEnter or @c onEnterTransitionDidFinish because
///             @c director->getRunningScene() will return the transition scene.
/// @warning    You can fix this by wrapping this function with
///             @c runAction(cocos2d::CallFunc::create([] { ... }));
void captureScreenInPoints(const std::function<void(cocos2d::Image*)>& afterCaptured,
                           float scale);

/// Attempts to download an image from an url.
///
/// @note       Downloaded image is automatically store in the texture cache
///             for faster later retrieving.
/// @param      imageUrl is the url of the image.
/// @param      afterDownloaded will be invoked after the downloading process
///             has finished.
/// @warning    GIF images are not supported.
void downloadImage(const std::string& imageUrl,
                   const std::function<void(cocos2d::Texture2D*)>& afterDownloaded);
NS_EE_END

#endif /* EE_LIBRARY_UTILS_HPP_ */