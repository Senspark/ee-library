//
//  EEBackButtonListener.cpp
//  ee-library
//
//  Created by enrevol on 10/26/15.
//
//

#include "EEBackButtonListener.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
void BackButtonListenerBase::registerListener(cocos2d::Node* node) {
    CC_ASSERT(_listener == nullptr);
    
    _listener = cocos2d::EventListenerKeyboard::create();
    _listener->onKeyReleased = std::bind(&BackButtonListenerBase::onKeyReleased,
                                         this,
                                         node,
                                         std::placeholders::_1,
                                         std::placeholders::_2);
    
    cocos2d::Director::getInstance()
    ->getEventDispatcher()
    ->addEventListenerWithSceneGraphPriority(_listener, node);
}

void BackButtonListenerBase::unregisterListener() {
    CC_ASSERT(_listener != nullptr);
    
    cocos2d::Director::getInstance()
    ->getEventDispatcher()
    ->removeEventListener(_listener);
    
    _listener = nullptr;
}

void BackButtonListenerBase::onKeyReleased(cocos2d::Node* node,
                                           cocos2d::EventKeyboard::KeyCode keyCode,
                                           cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
        if (node->isRunning()) {
            onBackButtonPressed();
            
            // Allow only one event listener receives event.
            event->stopPropagation();
        }
    }
}
NS_DETAIL_END
NS_EE_END