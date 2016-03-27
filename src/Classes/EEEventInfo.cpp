//
//  EEEvent.cpp
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#include "EEEventInfo.hpp"

#include "cocos2d.h"

NS_EE_BEGIN
NS_DETAIL_BEGIN
int EventInfoBase::counter = 0;

EventInfoBase::EventInfoBase(std::string key)
: _id(counter++)
, _key(std::move(key))
{}

void EventInfoBase::removeListeners() const {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(_key);
}

const std::string& EventInfoBase::getKey() const {
    return _key;
}
    
void EventInfoBase::addListenerHelper(cocos2d::EventListener* listener) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void EventInfoBase::dispatchHelper(cocos2d::Event* event) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(event);
}
NS_DETAIL_END
NS_EE_END