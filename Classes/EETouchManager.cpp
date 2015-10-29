//
//  EETouchManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/4/15.
//
//

#include "EETouchManager-Impl.hpp"
#include "EEMacro.hpp"
#include "cocos2d.h"

namespace_ee_begin
TouchManager* TouchManager::getInstance() {
    static Impl sharedInstance;
    return &sharedInstance;
};
    
void TouchManager::lock(cocos2d::Touch* touch) {
    LOG_FUNC();
    auto impl = static_cast<Impl*>(this);
    touch->retain();
    impl->_activeTouch = touch;
}
    
void TouchManager::unlock(cocos2d::Touch* touch) {
    LOG_FUNC();
    auto impl = static_cast<Impl*>(this);
    if (touch == impl->_activeTouch) {
        impl->_activeTouch->release();
        impl->_activeTouch = nullptr;
    }
}

bool TouchManager::isLocked() {
    auto impl = static_cast<Impl*>(this);
    if (impl->_activeTouch != nullptr && impl->_activeTouch->getReferenceCount() == 1) {
        unlock(impl->_activeTouch);
    }
    return impl->_activeTouch != nullptr;
}
namespace_ee_end