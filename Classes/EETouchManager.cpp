//
//  EETouchManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/4/15.
//
//

#include "EETouchManager-Impl.h"
#include "EEMacro.hpp"
#include "cocos2d.h"

namespace_ee_begin
TouchManager* TouchManager::getInstance() {
    static Impl sharedInstance;
    return &sharedInstance;
};
    
void TouchManager::lock(cocos2d::Touch* touch) {
    LOG_FUNC();
    auto ptr = static_cast<Impl*>(this);
    touch->retain();
    ptr->_activeTouch = touch;
}
    
void TouchManager::unlock() {
    LOG_FUNC();
    auto ptr = static_cast<Impl*>(this);
    if (ptr->_activeTouch != nullptr) {
        ptr->_activeTouch->release();
        ptr->_activeTouch = nullptr;
    }
}

bool TouchManager::isLocked() {
    auto ptr = static_cast<Impl*>(this);
    if (ptr->_activeTouch != nullptr && ptr->_activeTouch->getReferenceCount() == 1) {
        unlock();
    }
    return ptr->_activeTouch != nullptr;
}
namespace_ee_end