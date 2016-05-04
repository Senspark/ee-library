//
//  EECocosUtils.cpp
//  EE Library
//
//  Created by enrevol on 10/26/15.
//
//

#include "CocosUtils.hpp"
#include "EEDialogManager.hpp"

#include "HttpClient.h"

#include <cstring>

namespace ee {
void doRecursively(cocos2d::Node* target, const std::function<void(cocos2d::Node*)>& callback) {
    callback(target);
    for (auto&& child : target->getChildren()) {
        doRecursively(child, callback);
    }
}

void pauseAll(cocos2d::Node* target) {
    doRecursively(target, &cocos2d::Node::pause);
}

void resumeAll(cocos2d::Node* target) {
    doRecursively(target, &cocos2d::Node::resume);
}

//void runAction(cocos2d::Node* target, std::function<void(cocos2d::Node*)> actionFunc, std::function<float()> delayFunc, std::function<float()> initialDelayFunc /* = nullptr */) {
//    auto delay = cocos2d::DelayTime::create(delayFunc());
//    auto func = cocos2d::CallFunc::create([actionFunc, target] {
//            actionFunc(target);
//        });
//    auto recursion = cocos2d::CallFunc::create([target, actionFunc, delayFunc] {
//            runAction(target, actionFunc, delayFunc);
//        });
//    cocos2d::Sequence* sequence;
//    if (initialDelayFunc != nullptr) {
//        auto initialDelay = cocos2d::DelayTime::create(initialDelayFunc());
//        sequence = cocos2d::Sequence::create(initialDelay, func, delay, recursion, nullptr);
//    } else {
//        sequence = cocos2d::Sequence::create(func, delay, recursion, nullptr);
//    }
//    target->runAction(sequence);
//

void captureScreenInPixels(const std::function<void(cocos2d::Image*)>& afterCaptured) {
    cocos2d::CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = [afterCaptured] {
        auto glView = cocos2d::Director::getInstance()->getOpenGLView();
        auto&& frameSize = glView->getFrameSize();

        auto width = static_cast<GLsizei>(frameSize.width);
        auto height = static_cast<GLsizei>(frameSize.height);

        do {
            auto arraySize = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;
            auto buffer = std::make_unique<GLubyte[]>(arraySize);
            glPixelStorei(GL_PACK_ALIGNMENT, 1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            // The frame buffer is always created with portrait orientation on WP8.
            // So if the current device orientation is landscape, we need to rotate the frame buffer.
            auto&& renderTargetSize = glView->getRenderTargetSize();
            CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
            glReadPixels(0, 0, (int) renderTargetSize.width, (int) renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif

            auto flippedBuffer = std::make_unique<GLubyte[]>(arraySize);

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
                std::memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, static_cast<std::size_t>(width) * 4);
            }
#endif
            auto image = std::make_unique<cocos2d::Image>();
            image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8);
            afterCaptured(image.get());
        } while (false);
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}

void captureScreenInPoints(const std::function<void(cocos2d::Image*)>& afterCaptured, float scale) {
    auto scene = DialogManager::getInstance()->getCurrentScene();
    // auto scene = cocos2d::Director::getInstance()->getRunningScene();
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

    // Retain visited nodes to ensure their custom commands don't release.
    std::vector<cocos2d::Node*> visitedNodes;
    runActionRecursively(scene, [&visitedNodes](cocos2d::Node* node) {
            node->retain();
            visitedNodes.push_back(node);
        });
    CCLOG("Visited nodes count = %d", (int) visitedNodes.size());

    static cocos2d::CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = [visitedNodes, renderer, scene, afterCaptured] {
        auto image = renderer->newImage();
        afterCaptured(image);
        image->release();
        // Reset scale and anchor point.
        scene->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        scene->setScale(1.0f);
        // Release visited nodes.
        for (auto node : visitedNodes) {
            node->release();
        }
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}

void downloadImage(const std::string& imageUrl, const std::function<void(cocos2d::Texture2D*)>& afterDownloaded) {
    auto cache = cocos2d::Director::getInstance()->getTextureCache();
    auto cachedTexture = cache->getTextureForKey(imageUrl);
    if (cachedTexture != nullptr) {
        afterDownloaded(cachedTexture);
    } else {
        auto request = new cocos2d::network::HttpRequest();
        request->setUrl(imageUrl.c_str());
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setResponseCallback([imageUrl, afterDownloaded](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
            CC_UNUSED_PARAM(client);
            cocos2d::Image* image = nullptr;
            do {
                CC_BREAK_IF(response == nullptr || response->isSucceed() == false);
                CC_BREAK_IF(response->getResponseCode() != 200);
                
                auto buffer = response->getResponseData();
                CC_BREAK_IF(buffer == nullptr);
                CC_BREAK_IF(buffer->size() == 0);
                
                // GIF images are not supported by cocos2d-x.
                CC_BREAK_IF((*buffer)[0] == 'G' && (*buffer)[1] == 'I' && (*buffer)[2] == 'F');
                
                image = new cocos2d::Image();
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
} // namespace ee