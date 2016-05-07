//
//  EEUtils.cpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#include "EEUtils.hpp"
#include "EEImage.hpp"

#include <cocos2d.h>
#include <network/HttpClient.h>

NS_EE_BEGIN
UniqueListener make_unique_listener(cocos2d::EventListener* listener) {
    auto deleter = [](cocos2d::EventListener* lst) {
        cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->removeEventListener(lst);
    };
    return UniqueListener(listener, deleter);
}

void doRecursively(cocos2d::Node* node,
                   const std::function<void(cocos2d::Node*)>& action) {
    action(node);
    auto&& children = node->getChildren();
    for (auto&& child : children) {
        doRecursively(child, action);
    }
}

void pauseAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::pause);
}

void resumeAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::resume);
}

bool isActuallyVisible(const cocos2d::Node* node) {
    if (node == nullptr) {
        // node is nullptr.
        return false;
    }
    if (node->isRunning() == false) {
        // node is not running.
        return false;
    }
    auto current = node;
    while (current != nullptr) {
        if (current->isVisible() == false) {
            // One of node's ancestors is not visible.
            return false;
        }
        current = current->getParent();
    }
    return true;
}

void captureScreenInPixels(const std::function<void(cocos2d::Image*)>& afterCaptured) {
    auto command = std::make_shared<cocos2d::CustomCommand>();
    
    command->init(std::numeric_limits<float>::max());
    command->func = [afterCaptured, command]() mutable {
        auto glView = cocos2d::Director::getInstance()->getOpenGLView();
        auto frameSize = glView->getFrameSize();
        
        GLsizei width = static_cast<GLsizei>(frameSize.width);
        GLsizei height = static_cast<GLsizei>(frameSize.height);
        
        do {
            auto bufferSize = static_cast<std::size_t>(width * height * 4);
            auto buffer = std::make_unique<GLubyte[]>(bufferSize);
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            // The frame buffer is always created with portrait orientation on WP8.
            // So if the current device orientation is landscape,
            // we need to rotate the frame buffer.
            auto&& renderTargetSize = glView->getRenderTargetSize();
            CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height),
                     "The frame size is not matched");
            
            glReadPixels(0, 0, (int) renderTargetSize.width, (int) renderTargetSize.height,
                         GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
            glReadPixels(0, 0, width, height,
                         GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif
            
            auto flippedBuffer = std::make_unique<GLubyte[]>(bufferSize);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            if (width == static_cast<int>(renderTargetSize.width)) {
                // The current device orientation is portrait.
                for (int row = 0; row < height; ++row) {
                    std::memcpy(flippedBuffer.get() + (height - row - 1) * width * 4,
                                buffer.get() + row * width * 4, width * 4);
                }
            } else {
                // The current device orientation is landscape.
                for (int row = 0; row < width; ++row) {
                    for (int col = 0; col < height; ++col) {
                        *(int*) (flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) =
                        *(int*) (buffer.get() + row * height * 4 + col * 4);
                    }
                }
            }
#else
            for (int row = 0; row < height; ++row) {
                std::memcpy(flippedBuffer.get() + (height - row - 1) * width * 4,
                            buffer.get() + row * width * 4, (unsigned) width * 4);
            }
#endif
            auto image = new cocos2d::Image();
            image->initWithRawData(flippedBuffer.get(), width * height * 4,
                                   width, height, 8);
            afterCaptured(image);
            image->release();
        } while (false);
        
        command.reset();
    };
    cocos2d::Director::getInstance()->getRenderer()->addCommand(command.get());
}

cocos2d::Image* captureScreenInPoints(float scale) {
    // Retrieve the current scene.
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    
    auto transition = dynamic_cast<cocos2d::TransitionScene*>(scene);
    if (transition != nullptr) {
        // Current scene is a transition scene,
        // which means this function is called when the transition
        // scene has not finished.
        CCLOG("You are capturing the transition scene!");
    }
    
    // Retrieve the win size.
    auto&& winSize = cocos2d::Director::getInstance()->getWinSize();
    
    // Calculate resulting image size.
    auto&& size = winSize * scale;
    
    int width = static_cast<int>(size.width);
    int height = static_cast<int>(size.height);
    
    // Create a renderer.
    auto renderer = cocos2d::RenderTexture::create(width, height);
    
    // Temporarily assign new anchor point and scale
    // for the current scene.
    scene->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    scene->setScale(scale);
    
    // Process rendering.
    renderer->begin();
    scene->visit();
    renderer->end();
    
    // Force to render immediately.
    cocos2d::Director::getInstance()->getRenderer()->render();
    
    // Retrieve the image.
    auto image = renderer->newImage();
    image->autorelease();
        
    // Reset scale and anchor point.
    scene->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    scene->setScale(1.0f);
    
    return image;
}

cocos2d::Sprite* captureBlurredScreenInPoints(float scale, int blurRadius) {
    auto image = captureScreenInPoints(scale);
    auto realRadius = static_cast<image::SizeType>(blurRadius * CC_CONTENT_SCALE_FACTOR());
    return createSpriteFromImage(image, std::bind(tentBlur1D,
                                                  std::placeholders::_1,
                                                  realRadius,
                                                  2));
}

cocos2d::Sprite* createSpriteFromImage(cocos2d::Image* image,
                                       const std::function<void(cocos2d::Image*)>& processor) {
    
    if (processor) {
        processor(image);
    }
    
    auto texture = new cocos2d::Texture2D();
    texture->initWithImage(image);
    
    // Create the resulting sprite.
    auto sprite = cocos2d::Sprite::createWithTexture(texture);
    
    // Release resource.
    texture->release();
    
    return sprite;
}

void downloadImage(const std::string& imageUrl,
                   const std::function<void(cocos2d::Texture2D*)>& afterDownloaded) {
    // Retrieve the texture in the texture cache.
    auto cachedTexture = cocos2d::Director::getInstance()
    ->getTextureCache()
    ->getTextureForKey(imageUrl);
    
    if (cachedTexture != nullptr) {
        // Image is already in the cache.
        return afterDownloaded(cachedTexture);
    }
    
    // Image is not in the cache.
    // Attempt to download the image from the given url.
    
    // Can not use std::unique_ptr here because cocos2d::network::HttpClient
    // will retain it later.
    auto request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(imageUrl.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    
    auto callback = [imageUrl, afterDownloaded](cocos2d::network::HttpClient* client,
                                                cocos2d::network::HttpResponse* response) {
        cocos2d::Image* image = nullptr;
        do {
            CC_BREAK_IF(response == nullptr || response->isSucceed() == false);
            CC_BREAK_IF(response->getResponseCode() != 200);
            
            auto buffer = response->getResponseData();
            CC_BREAK_IF(buffer == nullptr);
            CC_BREAK_IF(buffer->size() == 0);
            
            // GIF images are not supported by cocos2d-x.
            CC_BREAK_IF(buffer->size() < 3);
            CC_BREAK_IF((*buffer)[0] == 'G' && (*buffer)[1] == 'I' && (*buffer)[2] == 'F');
            
            auto data = reinterpret_cast<unsigned char*>(&buffer->front());
            auto dataLen = static_cast<ssize_t>(buffer->size());
            
            image = new (std::nothrow) cocos2d::Image();
            image->initWithImageData(data, dataLen);
        } while (false);
        
        if (afterDownloaded) {
            // Current thread is not cocos thread,
            // we should invoke the callback on cocos thread.
            cocos2d::Director::getInstance()
            ->getScheduler()
            ->performFunctionInCocosThread([afterDownloaded, imageUrl, image] {
                if (image != nullptr) {
                    // Create new texture with the given image and url.
                    auto newTexture = cocos2d::Director::getInstance()
                    ->getTextureCache()
                    ->addImage(image, imageUrl);
                    
                    // Invoke the callback.
                    afterDownloaded(newTexture);
                    
                    // Release resources.
                    image->release();
                } else {
                    afterDownloaded(nullptr);
                }
            });
        }
    };
    request->setResponseCallback(callback);
    cocos2d::network::HttpClient::getInstance()->sendImmediate(request);
    
    // Release resources.
    request->release();
}
NS_EE_END
