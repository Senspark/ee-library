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

cocos2d::Scene* DialogManager::getCurrentScene() {
    auto impl = static_cast<Impl*>(this);
    impl->updateCurrentScene();
    return impl->_lastScene;
}
    
void DialogManager::hideDialog() {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto impl = static_cast<Impl*>(this);
//     CC_ASSERT(impl->_dialogQueue.empty());
//     CC_ASSERT(impl->_dialogStack.empty() == false);
    impl->_actionQueue.emplace_back([impl] {
        decltype(impl->_actionQueue) actions;
        actions.swap(impl->_actionQueue);
        impl->_dialogStack.back().dialog->hide();
        impl->_actionQueue.insert(impl->_actionQueue.cend(), actions.cbegin(), actions.cend());
        impl->processActionQueue();
    });
    impl->processActionQueue();
}
    
void DialogManager::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto impl = static_cast<Impl*>(this);
    auto parent = getRunningNode();
    // Pause parent.
    pauseAll(parent);
    // Push dialog to stack.
    impl->_dialogStack.emplace_back(container, dialog);
    // Add dialog to scene.
    parent->addChild(container, localZOrder);
}

void DialogManager::popDialog(Dialog* dialog) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto impl = static_cast<Impl*>(this);
    auto parent = getParentNode();
    auto&& dialogInfo = impl->_dialogStack.back();
    CC_ASSERT(dialog == dialogInfo.dialog);
    // Remove dialog from stack.
    dialogInfo.container->removeFromParent();
    // Pop dialog from stack.
    impl->_dialogStack.pop_back();
    // Resume scene.
    resumeAll(parent);
}

void DialogManager::addToQueue(const std::function<void()>& callback) {
    auto impl = static_cast<Impl*>(this);
    impl->_actionQueue.emplace_back(callback);
    impl->processActionQueue();
}

void DialogManager::lock(Dialog* dialog) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto impl = static_cast<Impl*>(this);
    // Update current scene.
    impl->updateCurrentScene();
    // Add to locking set.
    impl->_lockingDialog = dialog;
}

void DialogManager::unlock(Dialog* dialog) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto impl = static_cast<Impl*>(this);
    // Remove from locking set.
    impl->_lockingDialog = nullptr;
    // Process queue.
    impl->processActionQueue();
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
    auto transition = dynamic_cast<cocos2d::TransitionScene*>(currentScene);
    if (transition != nullptr) {
        currentScene = transition->getInScene();
    }
    if (currentScene != _lastScene) {
        _lastScene = currentScene;
        while (_dialogStack.empty() == false) {
            auto container = _dialogStack.back().container;
            container->removeFromParent();
            _dialogStack.pop_back();
        }
        _lockingDialog = nullptr;
    }
}

void DialogManager::Impl::processActionQueue() {
    if (_lockingDialog != nullptr) {
        return;
    }
    if (_actionQueue.empty() == false) {
        auto callback = _actionQueue.front();
        _actionQueue.pop_front();
        callback();
    }
}
namespace_ee_end