//
//  EEEvent.cpp
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#include "EEEvent.h"
#include "cocos2d.h"

namespace_ee_begin
int EventBase::counter = 0;

void EventBase::removeListeners() const {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_key);
}
    
cocos2d::EventListenerCustom* EventBase::addListenerHelper(const std::function<void(cocos2d::EventCustom*)>& callback) const {
    return cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(_key, callback);
}

void EventBase::dispatchHelper(void* data) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_key, data);
}
namespace_ee_end