//
//  EEDialogManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#include "EEDialogManager.hpp"

#include "EEDialog.hpp"
#include "EEDialogCommand.hpp"
#include "EEDialogTransitionProtocol.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
namespace dialog {
DialogManager* DialogManager::getInstance() {
    static DialogManager sharedInstance;
    return &sharedInstance;
}
    
void DialogManager::pushDialog(Dialog* dialog) {
    pushDialog(dialog, Dialog::TopLevel);
}
    
void DialogManager::pushDialog(Dialog* dialog, std::size_t level) {
    updateCurrentScene();
    
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getContainer() != nullptr);
    commandQueue_.emplace_back(CommandType::Push, dialog, level);
    processCommandQueue();
}
    
void DialogManager::popDialog(Dialog* dialog) {
    updateCurrentScene();
    
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getParent() != nullptr);
    auto level = dialog->getDialogLevel();
    commandQueue_.emplace_back(CommandType::Pop, dialog, level);
    processCommandQueue();
}
    
void DialogManager::popDialog() {
    popDialog(getTopDialog());
}

Dialog* DialogManager::getDialog(std::size_t level) {
    updateCurrentScene();
    if (dialogStack_.empty()) {
        return nullptr;
    }
    if (level == Dialog::TopLevel) {
        level = dialogStack_.size();
    }
    CC_ASSERT(1 <= level && level <= dialogStack_.size());
    return dialogStack_.at(level - 1).dialog;
}

Dialog* DialogManager::getTopDialog() {
    return getDialog(Dialog::TopLevel);
}

void DialogManager::updateCurrentScene() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    auto transitionScene = dynamic_cast<cocos2d::TransitionScene*>(currentScene);
    if (transitionScene != nullptr) {
        CCASSERT(false, "The current transition scene is not finished!");
    }
    if (currentScene != currentScene_) {
        currentScene_ = currentScene;
        dialogStack_.clear();
        lockingDialog_ = nullptr;
        currentLevel_ = 0;
        commandQueue_.clear();
    }
}

void DialogManager::processCommandQueue() {
    if (isLocked()) {
        return;
    }
    using DiffType = decltype(commandQueue_)::difference_type;
    for (std::size_t i = 0; i < commandQueue_.size(); ++i) {
        auto command = commandQueue_.at(i);
        if (command.type == CommandType::Push) {
            if (currentLevel_ + 1 == command.level ||
                command.level == Dialog::TopLevel) {
                commandQueue_.erase(std::next(commandQueue_.begin(),
                                              static_cast<DiffType>(i)));
                pushDialogImmediately(command.dialog, command.level);
                break;
            }
        } else if (command.type == CommandType::Pop) {
            if ((currentLevel_ == command.level ||
                 command.level == Dialog::TopLevel) &&
                getTopDialog() == command.dialog) {
                commandQueue_.erase(std::next(commandQueue_.begin(),
                                              static_cast<DiffType>(i)));
                popDialogImmediately(command.dialog);
                break;
            }
        } else {
            CC_ASSERT(false);
        }
    }
}
    
void DialogManager::pushDialogImmediately(Dialog* dialog, std::size_t level) {
    LOG_FUNC();
    
    auto protocol = dynamic_cast<DialogTransitionProtocol*>(dialog);
    if (protocol != nullptr) {
        protocol->onShowBegan();
    }
    
    lock(dialog);
    
    auto parent = getRunningNode();
    pauseAll(parent);
    
    ++currentLevel_;
    dialog->dialogLevel_ = currentLevel_;
    dialogStack_.emplace_back(dialog);
    parent->addChild(dialog->getContainer(), Dialog::ContainerLocalZOrder);
    
    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    
    if (protocol != nullptr) {
        for (auto&& act : protocol->getShowingActions()) {
            actions.pushBack(act);
        }
    }
    actions.pushBack(cocos2d::CallFunc::create([this, dialog, protocol] {
        unlock(dialog);
        dialog->setActive(true);
        
        if (protocol != nullptr) {
            protocol->onShowEnded();
        }
    }));
    
    dialog->runAction(cocos2d::Sequence::create(actions));
}
    
void DialogManager::popDialogImmediately(Dialog* dialog) {
    LOG_FUNC();
    CC_ASSERT(dialog == getTopDialog());
    
    auto protocol = dynamic_cast<DialogTransitionProtocol*>(dialog);
    if (protocol != nullptr) {
        protocol->onHideBegan();
    }
    
    dialog->setActive(false);
    lock(dialog);
    
    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    
    if (protocol != nullptr) {
        for (auto&& act : protocol->getHidingActions()) {
            actions.pushBack(act);
        }
    }
    actions.pushBack(cocos2d::CallFunc::create([this, dialog, protocol] {
        dialog->getContainer()->removeFromParent();
        dialogStack_.pop_back();
        --currentLevel_;
        auto parent = getRunningNode();
        resumeAll(parent);
        unlock(dialog);
        if (protocol != nullptr) {
            protocol->onHideEnded();
        }
    }));
    
    dialog->runAction(cocos2d::Sequence::create(actions));
}
    
cocos2d::Node* DialogManager::getRunningNode() {
    updateCurrentScene();
    auto dialog = getTopDialog();
    if (dialog != nullptr) {
        return dialog->getParent();
    }
    return cocos2d::Director::getInstance()->getRunningScene();
}

bool DialogManager::isLocked() {
    updateCurrentScene();
    return lockingDialog_ != nullptr;
}

void DialogManager::unlock(Dialog* dialog) {
    LOG_FUNC();
    CCASSERT(lockingDialog_ == dialog,
             "Attempted to unlock not the top dialog!");
    lockingDialog_ = nullptr;
    processCommandQueue();
}

void DialogManager::lock(Dialog* dialog) {
    LOG_FUNC();
    CC_ASSERT(lockingDialog_ == nullptr);
    updateCurrentScene();
    lockingDialog_ = dialog;
}
} // namespace dialog
NS_EE_END