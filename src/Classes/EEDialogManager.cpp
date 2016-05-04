//
//  EEDialogManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#include "EEDialogManager.hpp"

#include "EEDialog.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
DialogManager* DialogManager::getInstance() {
    static DialogManager sharedInstance;
    return &sharedInstance;
}

bool DialogManager::show(const Command& command) {
    addCommand(CommandType::Show, command);
    processCommand();
    return _commandQueue.empty();
}

bool DialogManager::push(const Command& command) {
    addCommand(CommandType::Push, command);
    processCommand();
    return _commandQueue.empty();
}

bool DialogManager::hide(const Command& command) {
    addCommand(CommandType::Hide, command);
    processCommand();
    return _commandQueue.empty();
}

void DialogManager::addCommand(CommandType type, const Command& command) {
    _commandQueue.emplace(type, command);
    return processCommand();
}

void DialogManager::processCommand() {
    if (getLockingDialog() != nullptr) {
        return;
    }
    while (_commandQueue.empty() == false) {
        auto cmd = _commandQueue.front();
        _commandQueue.pop();
        auto dialog = cmd.second();
        switch (cmd.first) {
            case CommandType::Show: {
                
                break;
            }
            case CommandType::Push: {
                
                break;
            }
            case CommandType::Hide: {
                
                break;
            }
        }
    }
}

Dialog* DialogManager::getLockingDialog() const {
    return _lockingDialog;
}

Dialog* DialogManager::getCurrentDialog() const {
    CC_ASSERT(_dialogStack.empty() == false);
    return _dialogStack.back().dialog;
}

cocos2d::Scene* DialogManager::getCurrentScene() {
    updateCurrentScene();
    return _lastScene;
}
    
void DialogManager::hideDialog() {
    LOG_FUNC();
//     CC_ASSERT(impl->_dialogQueue.empty());
//     CC_ASSERT(impl->_dialogStack.empty() == false);
    _actionQueue.emplace_back([this] {
        decltype(_actionQueue) actions;
        actions.swap(_actionQueue);
        _dialogStack.back().dialog->hide();
        _actionQueue.insert(_actionQueue.cend(), actions.cbegin(), actions.cend());
        processActionQueue();
    });
    processActionQueue();
}
    
void DialogManager::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto parent = getRunningNode();
    // Pause parent.
    pauseAll(parent);
    // Push dialog to stack.
    _dialogStack.emplace_back(container, dialog);
    // Add dialog to scene.
    parent->addChild(container, localZOrder);
}

void DialogManager::popDialog(Dialog* dialog) {
    LOG_FUNC();
    // Retrieve impl pointer.
    auto parent = getParentNode();
    auto&& dialogInfo = _dialogStack.back();
    CC_ASSERT(dialog == dialogInfo.dialog);
    // Remove dialog from stack.
    dialogInfo.container->removeFromParent();
    // Pop dialog from stack.
    _dialogStack.pop_back();
    // Resume scene.
    resumeAll(parent);
}

void DialogManager::addToQueue(const std::function<void()>& callback) {
    _actionQueue.emplace_back(callback);
    processActionQueue();
}

void DialogManager::lock(Dialog* dialog) {
    LOG_FUNC();
    // Update current scene.
    updateCurrentScene();
    // Add to locking set.
    _lockingDialog = dialog;
}

void DialogManager::unlock(Dialog* dialog) {
    LOG_FUNC();
    // Remove from locking set.
    _lockingDialog = nullptr;
    // Process queue.
    processActionQueue();
}

cocos2d::Node* DialogManager::getRunningNode() {
    updateCurrentScene();
    cocos2d::Node* ret = nullptr;
    if (_dialogStack.empty() == false) {
        ret = _dialogStack.back().container;
    } else {
        ret = _lastScene;
    }
    return ret;
}
    
cocos2d::Node* DialogManager::getParentNode() {
    updateCurrentScene();
    cocos2d::Node* ret = nullptr;
    CC_ASSERT(_dialogStack.size() > 0);
    if (_dialogStack.size() >= 2) {
        ret = _dialogStack.at(_dialogStack.size() - 2).container;
    } else {
        ret = _lastScene;
    }
    return ret;
}
    
void DialogManager::updateCurrentScene() {
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

void DialogManager::processActionQueue() {
    if (_lockingDialog != nullptr) {
        return;
    }
    if (_actionQueue.empty() == false) {
        auto callback = _actionQueue.front();
        _actionQueue.pop_front();
        callback();
    }
}
NS_EE_END