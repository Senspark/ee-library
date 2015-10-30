//
//  EEDialogManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#include "EEDialogManager-Impl.hpp"

#include "EEDialog.hpp"
#include "EECocosUtils.hpp"
#include "cocos2d.h"

namespace_ee_begin
DialogManager* DialogManager::getInstance() {
    static Impl sharedInstance;
    return &sharedInstance;
}

Dialog* DialogManager::getCurrentDialog() const {
    auto impl = static_cast<const Impl*>(this);
    CC_ASSERT(impl->_dialogStack.empty() == false);
    return impl->_dialogStack.back().dialog;
}
    
void DialogManager::hideDialog() {
    auto impl = static_cast<Impl*>(this);
    CC_ASSERT(impl->_dialogStack.empty() == false);
    impl->_dialogStack.back().dialog->hide();
}
    
void DialogManager::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
    LOG_FUNC();
    auto impl = static_cast<Impl*>(this);
    container->retain();
    dialog->retain();
    impl->_dialogQueue.emplace(container, dialog, localZOrder);
    impl->processDialogQueue();
}

void DialogManager::popDialog(Dialog* dialog) {
    LOG_FUNC();
    auto parent = getParentNode();
    auto ptr = static_cast<Impl*>(this);
    auto&& info = ptr->_dialogStack.back();
    // Check whether it is still in stack.
    if (info.dialog == dialog) {
        // Remove dialog from stack.
        info.container->removeFromParent();
        info.container->release();
        // Pop dialog from stack.
        ptr->_dialogStack.pop_back();
        // Resume scene.
        resumeAll(parent);
    }
}

void DialogManager::lock(Dialog* dialog) {
    LOG_FUNC();
    auto impl = static_cast<Impl*>(this);
    impl->updateCurrentScene();
    impl->_lockingDialog.insert(dialog);
}

void DialogManager::unlock(Dialog* dialog) {
    LOG_FUNC();
    auto impl = static_cast<Impl*>(this);
    impl->_lockingDialog.erase(dialog);
    impl->processDialogQueue();
}

cocos2d::Node* DialogManager::getRunningNode() {
    auto ptr = static_cast<Impl*>(this);
    ptr->updateCurrentScene();
    cocos2d::Node* ret = nullptr;
    if (ptr->_dialogStack.empty() == false) {
        ret = ptr->_dialogStack.back().container;
    } else {
        ret = ptr->_lastScene;
    }
    return ret;
}
    
cocos2d::Node* DialogManager::getParentNode() {
    auto ptr = static_cast<Impl*>(this);
    ptr->updateCurrentScene();
    cocos2d::Node* ret = nullptr;
    CC_ASSERT(ptr->_dialogStack.size() > 0);
    if (ptr->_dialogStack.size() >= 2) {
        ret = ptr->_dialogStack.at(ptr->_dialogStack.size() - 2).container;
    } else {
        ret = ptr->_lastScene;
    }
    return ret;
}
    
void DialogManager::Impl::updateCurrentScene() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    if (currentScene != _lastScene) {
        _lastScene = currentScene;
        while (_dialogStack.empty() == false) {
            auto container = _dialogStack.back().container;
            container->removeFromParent();
            container->release();
            _dialogStack.pop_back();
        }
        _lockingDialog.clear();
    }
}

void DialogManager::Impl::internalPushDialog(const PushDialogInfo& info) {
    LOG_FUNC();
    auto parent = getRunningNode();
    // Pause parent.
    pauseAll(parent);
    // Push dialog to stack.
    info.container->retain();
    _dialogStack.emplace_back(info.container, info.dialog);
    // Add dialog to scene.
    parent->addChild(info.container, info.localZOrder);
}

void DialogManager::Impl::processDialogQueue() {
    if (_lockingDialog.empty()) {
        if (_dialogQueue.empty() == false) {
            auto&& front = _dialogQueue.front();
            internalPushDialog(front);
            front.container->release();
            front.dialog->release();
            _dialogQueue.pop();
        }
    }
}
namespace_ee_end