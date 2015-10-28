//
//  EECocosUtils.cpp
//  EE Library
//
//  Created by enrevol on 10/26/15.
//
//

#include "EECocosUtils.hpp"

#include "EEImage.hpp"
#include "cocos2d.h"
#include "network/HttpClient.h"

#include <cstring>

namespace_ee_begin
void runActionRecursively(cocos2d::Node* target, const std::function<void(cocos2d::Node*)>& actionFunc) {
    actionFunc(target);
    for (auto&& child : target->getChildren()) {
        runActionRecursively(child, actionFunc);
    }
}

void pauseAll(cocos2d::Node* target) {
    runActionRecursively(target, [](cocos2d::Node* current) { current->pause(); });
}

void resumeAll(cocos2d::Node* target) {
    runActionRecursively(target, [](cocos2d::Node* current) { current->resume(); });
}

void runAction(cocos2d::Node* target, std::function<void(cocos2d::Node*)> actionFunc, std::function<float()> delayFunc, std::function<float()> initialDelayFunc /* = nullptr */) {
    auto delay = cocos2d::DelayTime::create(delayFunc());
    auto func = cocos2d::CallFunc::create([actionFunc, target] {
        actionFunc(target);
    });
    auto recursion = cocos2d::CallFunc::create([target, actionFunc, delayFunc] {
        runAction(target, actionFunc, delayFunc);
    });
    cocos2d::Sequence* sequence;
    if (initialDelayFunc != nullptr) {
        auto initialDelay = cocos2d::DelayTime::create(initialDelayFunc());
        sequence = cocos2d::Sequence::create(initialDelay, func, delay, recursion, nullptr);
    } else {
        sequence = cocos2d::Sequence::create(func, delay, recursion, nullptr);
    }
    target->runAction(sequence);
}

void addAnimationToButton(cocos2d::Node* button) {
    auto ease = [](cocos2d::ActionInterval* action) -> cocos2d::ActionEase* {
        switch (cocos2d::random(0, 1)) {
            case 0: return cocos2d::EaseIn::create(action, 0.5f);
            case 1: return cocos2d::EaseOut::create(action, 0.5f);
        }
        return nullptr;
    };
    
    // ScaleX.
    runAction(button, [ease](cocos2d::Node* node) {
        float value = cocos2d::random(0, 1) ? 1.1f : 0.9f;
        auto callback = [node](float scaleX) { node->setScaleX(scaleX); };
        auto scale = cocos2d::ActionFloat::create(0.2f, 1.0f, value, callback);
        auto scaleBack = cocos2d::ActionFloat::create(0.2f, value, 1.0f, callback);
        auto sequence = cocos2d::Sequence::create(ease(scale), ease(scaleBack), nullptr);
        node->runAction(sequence);
    }, [] { return cocos2d::random(0.0f, 1.0f) * 2.0f + 5.0f; });
    
    // ScaleY.
    runAction(button, [ease](cocos2d::Node* node) {
        float value = cocos2d::random(0, 1) ? 1.1f : 0.9f;
        auto callback = [node](float scaleY) { node->setScaleY(scaleY); };
        auto scale = cocos2d::ActionFloat::create(0.2f, 1.0f, value, callback);
        auto scaleBack = cocos2d::ActionFloat::create(0.2f, value, 1.0f, callback);
        auto sequence = cocos2d::Sequence::create(ease(scale), ease(scaleBack), nullptr);
        node->runAction(sequence);
    }, [] { return cocos2d::random(0.0f, 1.0f) * 2.0f + 5.0f; }, [] { return 2.0f; });
    
    // Rotate.
    runAction(button, [ease](cocos2d::Node* node) {
        auto rotate = cocos2d::RotateTo::create(0.2f, cocos2d::random(0, 1) ? -2.0f : 2.0f);
        auto rotateBack = cocos2d::RotateTo::create(0.2f, 0);
        auto sequence = cocos2d::Sequence::create(ease(rotate), ease(rotateBack), nullptr);
        node->runAction(sequence);
    }, [] { return cocos2d::random(0.0f, 1.0f) * 2.0f + 5.0f; }, [] { return 4.0f; });
}

void captureScreenInPixels(const std::function<void(cocos2d::Image*)>& afterCaptured) {
    static cocos2d::CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = [afterCaptured] {
        auto glView = cocos2d::Director::getInstance()->getOpenGLView();
        auto frameSize = glView->getFrameSize();
        
        GLsizei width = static_cast<GLsizei>(frameSize.width);
        GLsizei height = static_cast<GLsizei>(frameSize.height);
        
        do {
            auto buffer = new GLubyte[width * height * 4];
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            // The frame buffer is always created with portrait orientation on WP8.
            // So if the current device orientation is landscape, we need to rotate the frame buffer.
            auto&& renderTargetSize = glView->getRenderTargetSize();
            CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
            glReadPixels(0, 0, (int) renderTargetSize.width, (int) renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
#endif
            
            auto flippedBuffer = new GLubyte[width * height * 4];
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            if (width == static_cast<int>(renderTargetSize.width)) {
                // The current device orientation is portrait.
                for (int row = 0; row < height; ++row) {
                    std::memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
                }
            } else {
                // The current device orientation is landscape.
                for (int row = 0; row < width; ++row) {
                    for (int col = 0; col < height; ++col) {
                        *(int*) (flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*) (buffer.get() + row * height * 4 + col * 4);
                    }
                }
            }
#else
            for (int row = 0; row < height; ++row) {
                std::memcpy(flippedBuffer + (height - row - 1) * width * 4, buffer + row * width * 4, (unsigned) width * 4);
            }
            delete[] buffer;
#endif
            auto image = new (std::nothrow) cocos2d::Image();
            image->initWithRawData(flippedBuffer, width * height * 4, width, height, 8);
            afterCaptured(image);
            image->release();
            
            delete[] flippedBuffer;
        } while (false);
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}

void captureScreenInPoints(const std::function<void(cocos2d::Image*)>& afterCaptured, float scale) {
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    auto&& size = cocos2d::Director::getInstance()->getWinSize() * scale;
    int width = (int) size.width;
    int height = (int) size.height;
    
    auto renderer = cocos2d::RenderTexture::create(width, height);
    // Set scale and anchor point for rendering.
    scene->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    scene->setScale(scale);
    renderer->begin();
    scene->visit();
    renderer->end();
    
    static cocos2d::CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = [renderer, scene, afterCaptured] {
        auto image = renderer->newImage();
        afterCaptured(image);
        image->release();
        // Reset scale and anchor point.
        scene->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        scene->setScale(1.0f);
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}

void downloadImage(const std::string& imageUrl, const std::function<void(cocos2d::Texture2D*)>& afterDownloaded) {
    auto cache = cocos2d::Director::getInstance()->getTextureCache();
    auto cachedTexture = cache->getTextureForKey(imageUrl);
    if (cachedTexture != nullptr) {
        afterDownloaded(cachedTexture);
    } else {
        auto request = new (std::nothrow) cocos2d::network::HttpRequest();
        request->setUrl(imageUrl.c_str());
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setResponseCallback([imageUrl, afterDownloaded](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
            cocos2d::Image* image = nullptr;
            do {
                CC_BREAK_IF(response == nullptr || response->isSucceed() == false);
                CC_BREAK_IF(response->getResponseCode() != 200);
                
                auto buffer = response->getResponseData();
                CC_BREAK_IF(buffer == nullptr);
                CC_BREAK_IF(buffer->size() == 0);
                
                // GIF images are not supported by cocos2d-x.
                CC_BREAK_IF((*buffer)[0] == 'G' && (*buffer)[1] == 'I' && (*buffer)[2] == 'F');
                
                image = new (std::nothrow) cocos2d::Image();
                image->initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), (ssize_t) buffer->size());
            } while (false);
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([afterDownloaded, imageUrl, image] {
                if (image != nullptr) {
                    auto newTexture = cocos2d::Director::getInstance()->getTextureCache()->addImage(image, imageUrl);
                    afterDownloaded(newTexture);
                    // Release resources.
                    image->release();
                } else {
                    afterDownloaded(nullptr);
                }
            });
        });
        cocos2d::network::HttpClient::getInstance()->sendImmediate(request);
        request->release();
    }
}
namespace_ee_end
