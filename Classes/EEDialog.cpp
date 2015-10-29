//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "EEDialog-Impl.hpp"
#include "EEDialogManager.hpp"
#include "cocos2d.h"

namespace_ee_begin
Dialog::Dialog()
: _impl(new Impl(this))
{}

Dialog::~Dialog() {}

bool Dialog::init() {
    if (ActiveNodeButton::init() == false) {
        return false;
    }
    // Close the dialog when pressing its outer.
    setTouchType(TouchType::Outside);
    setTouchUpCallback(CC_CALLBACK_0(Dialog::hide, this));
    return true;
}
    
void Dialog::onEnter() {
    NodeButton::onEnter();
}
    
void Dialog::onExit() {
    NodeButton::onExit();    
}
    
void Dialog::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
    DialogManager::getInstance()->pushDialog(container, dialog, localZOrder);
}

void Dialog::popDialog(Dialog* dialog) {
    DialogManager::getInstance()->popDialog(dialog);
}

void Dialog::invokeOnShowBeganCallbacks() {
    _impl->invokeCallback1(_impl->_onShowBeganCallbacks);
}

void Dialog::invokeOnShowEndedCallbacks() {
    _impl->invokeCallback2(_impl->_onShowEndedCallbacks);
}

void Dialog::invokeOnHideBeganCallbacks() {
    _impl->invokeCallback2(_impl->_onHideBeganCallbacks);
}

void Dialog::invokeOnHideEndedCallbacks() {
    _impl->invokeCallback1(_impl->_onHideEndedCallbacks);
}

Dialog::Impl::Impl(Dialog* base)
: _base(base)
{}

void Dialog::Impl::invokeCallback1(std::vector<Callback1Info>& infos) {
    std::stable_sort(infos.begin(), infos.end(), [](const Callback1Info& lhs, const Callback1Info& rhs) {
        return lhs.first < rhs.first;
    });
    _base->retain();
    for (auto&& info : infos) {
        info.second();
    }
    _base->release();
}

void Dialog::Impl::invokeCallback2(std::vector<Callback2Info>& infos) {
    std::stable_sort(infos.begin(), infos.end(), [](const Callback2Info& lhs, const Callback2Info& rhs) {
        return lhs.first < rhs.first;
    });
    _base->retain();
    for (auto&& info : infos) {
        info.second(_base->getParent(), _base);
    }
    _base->release();
}
namespace_ee_end