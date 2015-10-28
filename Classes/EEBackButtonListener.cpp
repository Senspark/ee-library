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
    _backButtonListener = cocos2d::EventListenerKeyboard::create();
    _backButtonListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
            pressedBack();
        }
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(_backButtonListener, this);
}

void BackButtonListener::removeBackButtonListener() {
    getEventDispatcher()->removeEventListener(_backButtonListener);
}
namespace_ee_end