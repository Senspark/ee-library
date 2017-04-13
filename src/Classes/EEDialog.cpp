//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "EEDialog.hpp"
#include "EEDialogManager.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>

namespace ee {
namespace dialog {
const int Dialog::ContainerLocalZOrder = 123456;
const std::size_t Dialog::TopLevel = 123456;

bool Dialog::init() {
    if (not Super::init()) {
        return false;
    }

    transitionAction_ = cocos2d::Node::create();
    transitionAction_->setVisible(false);
    addProtectedChild(transitionAction_);

    setActive(false);
    addClickEventListener(CC_CALLBACK_0(Dialog::onClickedOutside, this));
    return true;
}

void Dialog::onEnter() {
    Super::onEnter();
}

void Dialog::onExit() {
    Super::onExit();
}

const cocos2d::Node* Dialog::getContainer() const {
    return getParent();
}

cocos2d::Node* Dialog::getContainer() {
    return getParent();
}

void Dialog::show(std::size_t level) {
    if (getContainer() == nullptr) {
        auto container = cocos2d::Node::create();
        container->setContentSize(_director->getWinSize());
        container->addChild(this);
    }

    dialogLevel_ = level;
    DialogManager::getInstance()->pushDialog(this, level);
}

void Dialog::hide() {
    DialogManager::getInstance()->popDialog(this);
}

Dialog* Dialog::addDialogWillShowCallback(const CallbackType& callback,
                                          int priority) {
    onDialogWillShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogDidShowCallback(const CallbackType& callback,
                                         int priority) {
    onDialogDidShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogWillHideCallback(const CallbackType& callback,
                                          int priority) {
    onDialogWillHideCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogDidHideCallback(const CallbackType& callback,
                                         int priority) {
    onDialogDidHideCallbacks_.emplace_back(callback, priority);
    return this;
}

std::size_t Dialog::getDialogLevel() const noexcept {
    return dialogLevel_;
}

void Dialog::setActive(bool active) {
    isActive_ = active;
}

bool Dialog::isActive() const noexcept {
    return isActive_;
}

bool Dialog::hitTest(const cocos2d::Point& pt, const cocos2d::Camera* camera,
                     cocos2d::Vec3* p) const {
    // Test outside.
    return not Super::hitTest(pt, camera, p);
}

void Dialog::onDialogWillShow() {
    invokeCallbacks(onDialogWillShowCallbacks_);
}

void Dialog::onDialogDidShow() {
    invokeCallbacks(onDialogDidShowCallbacks_);
}

void Dialog::onDialogWillHide() {
    invokeCallbacks(onDialogWillHideCallbacks_);
}

void Dialog::onDialogDidHide() {
    invokeCallbacks(onDialogDidHideCallbacks_);
}

void Dialog::invokeCallbacks(std::vector<CallbackInfo>& callbacks) {
    std::stable_sort(callbacks.begin(), callbacks.end(), Compare2nd<>());

    RefGuard guard(this);

    for (auto&& info : callbacks) {
        info.first(this);
    }
}

void Dialog::onClickedOutside() {
    if (isActive()) {
        hide();
    }
}

Dialog*
Dialog::setShowingTransitions(const std::vector<TransitionRef>& transitions) {
    showingTransitions_ = transitions;
    return this;
}

Dialog*
Dialog::setHidingTransitions(const std::vector<TransitionRef>& transitions) {
    hidingTransitions_ = transitions;
    return this;
}

Dialog*
Dialog::addShowingTransitions(const std::vector<TransitionRef>& transitions) {
    showingTransitions_.insert(showingTransitions_.cend(), transitions.cbegin(),
                               transitions.cend());
    return this;
}

Dialog*
Dialog::addHidingTransitions(const std::vector<TransitionRef>& transitions) {
    hidingTransitions_.insert(hidingTransitions_.cend(), transitions.cbegin(),
                              transitions.cend());
    return this;
}

auto Dialog::getShowingTransitions() const
    -> const std::vector<TransitionRef> & {
    return showingTransitions_;
}

auto Dialog::getHidingTransitions() const
    -> const std::vector<TransitionRef> & {
    return hidingTransitions_;
}
} // namespace dialog
} // namespace ee
