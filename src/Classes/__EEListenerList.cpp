//
//  EEEventVector.cpp
//  ee-library
//
//  Created by Zinge on 5/8/16.
//
//

#include "EEListenerList.hpp"

NS_EE_BEGIN
void ListenerList::push_back(cocos2d::EventListener* listener) {
    listeners_.push_back(make_unique_listener(listener));
}

void ListenerList::clear() noexcept {
    listeners_.clear();
}
NS_EE_END