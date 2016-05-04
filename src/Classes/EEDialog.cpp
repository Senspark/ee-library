//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "EEDialog.hpp"
#include "EEDialogManager.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
const Dialog::Container* Dialog::getContainer() const {
    return getParent();
}

Dialog::Container* Dialog::getContainer() {
    return getParent();
}

//bool Dialog::init() {
//    if (ActiveNodeButton::init() == false) {
//        return false;
//    }
//    // Close the dialog when pressing its outer.
//    setTouchType(TouchType::Outside);
//    setTouchUpCallback(CC_CALLBACK_0(Dialog::hide, this));
//    return true;
//}
//    
//void Dialog::onEnter() {
//    ActiveNodeButton::onEnter();
//}
//    
//void Dialog::onExit() {
//    ActiveNodeButton::onExit();    
//}
//
//void Dialog::show(int localZOrder) {
//    if (_impl->_isShowing == false) {
//        _impl->_isShowing = true;
//        DialogManager::getInstance()->addToQueue(CC_CALLBACK_0(Dialog::internalShow, this, localZOrder));
//    }
//}
//
//void Dialog::hide() {
//    if (_impl->_isHiding == false) {
//        _impl->_isHiding = true;
//        DialogManager::getInstance()->addToQueue(CC_CALLBACK_0(Dialog::internalHide, this));
//    }
//}
//    
//void Dialog::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
//    DialogManager::getInstance()->pushDialog(container, dialog, localZOrder);
//}
//
//void Dialog::popDialog(Dialog* dialog) {
//    DialogManager::getInstance()->popDialog(dialog);
//}
//
//Dialog* Dialog::addOnShowBeganCallback(const Callback1& callback, int priority) {
//    _impl->_onShowBeganCallbacks.emplace_back(priority, callback);
//    return this;
//}
//
//Dialog* Dialog::addOnShowEndedCallback(const Callback2& callback, int priority) {
//    _impl->_onShowEndedCallbacks.emplace_back(priority, callback);
//    return this;
//}
//
//Dialog* Dialog::addOnHideBeganCallback(const Callback2& callback, int priority) {
//    _impl->_onHideBeganCallbacks.emplace_back(priority, callback);
//    return this;
//}
//
//Dialog* Dialog::addOnHideEndedCallback(const Callback1& callback, int priority) {
//    _impl->_onHideEndedCallbacks.emplace_back(priority, callback);
//    return this;
//}
//
//void Dialog::invokeOnShowBeganCallbacks() {
//    DialogManager::getInstance()->lock(this);
//    _impl->invokeCallback1(_impl->_onShowBeganCallbacks);
//}
//
//void Dialog::invokeOnShowEndedCallbacks() {
//    DialogManager::getInstance()->unlock(this);
//    _impl->invokeCallback2(_impl->_onShowEndedCallbacks);
//}
//
//void Dialog::invokeOnHideBeganCallbacks() {
//    DialogManager::getInstance()->lock(this);
//    _impl->invokeCallback2(_impl->_onHideBeganCallbacks);
//}
//
//void Dialog::invokeOnHideEndedCallbacks() {
//    DialogManager::getInstance()->unlock(this);
//    _impl->invokeCallback1(_impl->_onHideEndedCallbacks);
//}
//
//void Dialog::invokeCallback1(std::vector<Callback1Info>& infos) {
//    std::stable_sort(infos.begin(), infos.end(), [](const Callback1Info& lhs, const Callback1Info& rhs) {
//        return lhs.first < rhs.first;
//    });
//    for (auto&& info : infos) {
//        info.second();
//    }
//}
//
//void Dialog::Impl::invokeCallback2(std::vector<Callback2Info>& infos) {
//    std::stable_sort(infos.begin(), infos.end(), [](const Callback2Info& lhs, const Callback2Info& rhs) {
//        return lhs.first < rhs.first;
//    });
//    _base->retain();
//    for (auto&& info : infos) {
//        info.second(_base->getParent(), _base);
//    }
//    _base->release();
//}
NS_EE_END