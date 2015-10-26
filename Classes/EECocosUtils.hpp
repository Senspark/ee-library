//
//  EECocosUtils.hpp
//  EE Library
//
//  Created by enrevol on 10/26/15.
//
//

#ifndef EE_LIBRARY_COCOS_UTILS_HPP
#define EE_LIBRARY_COCOS_UTILS_HPP

#include "EEMacro.hpp"
#include "EEForward.hpp"

#include <functional>

namespace_ee_begin
void runActionRecursively(cocos2d::Node* target, const std::function<void(cocos2d::Node*)>& actionFunc);

void pauseAll(cocos2d::Node* target);
void resumeAll(cocos2d::Node* target);

void runAction(cocos2d::Node* target, std::function<void(cocos2d::Node*)> actionFunc, std::function<float()> delayFunc, std::function<float()> initialDelayFunc = nullptr);
void addAnimationToButton(cocos2d::Node* button);

/// Captures screenshot with callback.
void captureScreen(const std::function<void(cocos2d::Image*)>& afterCaptured);

/// Captures screenshot in point with callback.
void captureScreenInPoints(const std::function<void(cocos2d::Image*)>& afterCapture, float scale);

void downloadImage(const char* imageUrl, const std::function<void(cocos2d::Texture2D*)>& callback);
namespace_ee_end

#endif /* EE_LIBRARY_COCOS_UTILS_HPP */
