//
//  EEEvent.cpp
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#include "EEEventInfo.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
EventInfoBase::EventInfoBase(std::string key) {
    static int counter = 0;

    // Each event info has an auxiliary id to
    // ensure that each event info is unique.
    int id = counter++;
    _key = std::to_string(id) + std::move(key);
}

void EventInfoBase::removeListeners() const {
    cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->removeCustomEventListeners(getKey());
}

const std::string& EventInfoBase::getKey() const {
    return _key;
}

void EventInfoBase::addListenerHelper(cocos2d::EventListener* listener) const {
    cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithFixedPriority(listener, 1);
}

void EventInfoBase::dispatchHelper(cocos2d::Event* event) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(
        event);
}
NS_DETAIL_END
NS_EE_END