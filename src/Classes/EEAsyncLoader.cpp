//
//  EEAsyncSpriteLoader.cpp
//  ee-library
//
//  Created by enrevol on 4/14/16.
//
//

#include <condition_variable>
#include <mutex>

#include "EEAsyncLoader.hpp"

#include <cocos2d.h>

NS_EE_BEGIN

template<class T, class F, class... Args>
void doAsync(T&& obj, F&& func, Args&&... args) {
    
}

cocos2d::Sprite* AsyncSpriteLoader::createNode(cocos2d::Node* parent,
                                               cocosbuilder::CCBReader* reader) {
    cocos2d::Sprite* result = nullptr;
    
    std::condition_variable cv;
    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    
    // Create sprite in cocos2d thread.
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([&cv, &result] {
        result = cocos2d::Sprite::create();
        result->retain();
        cv.notify_one();
    });
    
    // Wait for the cv.
    cv.wait(lock);
    
    
    return result;
}
NS_EE_END