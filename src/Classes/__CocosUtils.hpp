//
//  EECocosUtils.hpp
//  EE Library
//
//  Created by enrevol on 10/26/15.
//
//

#ifndef EE_LIBRARY_COCOS_UTILS_HPP_
#define EE_LIBRARY_COCOS_UTILS_HPP_

#include <functional>
#include <string>

#include "2d/CCNode.h"

namespace ee {
void doRecursively(cocos2d::Node* target, const std::function<void(cocos2d::Node*)>& callback);

[[deprecated("use doRecursively.")]]
inline void runActionRecursively(cocos2d::Node* target, const std::function<void(cocos2d::Node*)>& callback) {
    doRecursively(target, callback);
}

template<class FunctionType, class ReturnType = std::enable_if_t<std::is_member_function_pointer<FunctionType>::value, void>>
ReturnType doRecursively(cocos2d::Node* target, FunctionType&& function) {
    doRecursively(target, [&function](cocos2d::Node* node) {
            (node->*function)();
        });
}

void pauseAll(cocos2d::Node* target);
void resumeAll(cocos2d::Node* target);

// void runAction(cocos2d::Node* target, std::function<void(cocos2d::Node*)> actionFunc, std::function<float()> delayFunc, std::function<float()> initialDelayFunc = nullptr);

/**
 * Captures screenshot in points, i.e. devices screen size.
 * This capture process is slow because it uses glReadPixels.
 * @param afterCaptured is the callback.
 */
void captureScreenInPixels(const std::function<void(cocos2d::Image*)>& afterCaptured);

/**
 * Captures screenshot in points, i.e. scene size.
 * This capture process is faster than `captureScreenInPoints` because it uses FBO.
 * @param afterCaptured is the callback.
 * @param scale is the scene scale.
 */
void captureScreenInPoints(const std::function<void(cocos2d::Image*)>& afterCaptured, float scale);

/**
 * Downloads image given its url.
 * Downloaded image is automatically stored in the texture cache for faster later retrieving.
 * @param imageUrl is the url of the image.
 * @param afterDownloaded is the callback.
 */
void downloadImage(const std::string& imageUrl, const std::function<void(cocos2d::Texture2D*)>& afterDownloaded);
} // namespace ee

#endif /* EE_LIBRARY_COCOS_UTILS_HPP_ */
