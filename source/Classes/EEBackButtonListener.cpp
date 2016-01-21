//
//  EEBackButtonListener.cpp
//  ee-library
//
//  Created by enrevol on 10/26/15.
//
//

#include "EEBackButtonListener.hpp"
#include "cocos2d.h"

namespace_ee_begin
void BackButtonListener::addBackButtonListener() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CC_ASSERT(_listener == nullptr);
    _listener = cocos2d::EventListenerKeyboard::create();
    _listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
            pressedBack();
        }
    };
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, getInstance());
#endif
}

void BackButtonListener::removeBackButtonListener() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CC_ASSERT(_listener != nullptr);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
    _listener = nullptr;
#endif
}

cocos2d::Node* BackButtonListener::getInstance() {
    auto result = dynamic_cast<cocos2d::Node*>(this);
    CC_ASSERT(result != nullptr);
    return result;
}
namespace_ee_end