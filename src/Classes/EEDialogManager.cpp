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
#include "EEDialogComponent.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>

namespace ee {
namespace dialog {
namespace {
void pauseAllDialog(cocos2d::Node* node, Dialog* dialog) {
    doRecursively(node, [dialog](cocos2d::Node* currentNode) {
        auto component =
            currentNode->getComponent(DialogComponent::DefaultName);
        if (component == nullptr) {
            currentNode->pause();
        } else {
            auto dialogComponent = dynamic_cast<DialogComponent*>(component);
            dialogComponent->pause(dialog);
        }
    });
}

void resumeAllDialog(cocos2d::Node* node, Dialog* dialog) {
    doRecursively(node, [dialog](cocos2d::Node* currentNode) {
        auto component =
            currentNode->getComponent(DialogComponent::DefaultName);
        if (component == nullptr) {
            currentNode->resume();
        } else {
            auto dialogComponent = dynamic_cast<DialogComponent*>(component);
            dialogComponent->resume(dialog);
        }
    });
}
} // namespace

DialogManager* DialogManager::getInstance() {
    static DialogManager sharedInstance;
    return &sharedInstance;
}

void DialogManager::pushDialog(Dialog* dialog) {
    pushDialog(dialog, Dialog::TopLevel);
}

void DialogManager::pushDialog(Dialog* dialog, std::size_t level) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getContainer() != nullptr);
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);

    updateCurrentScene();
    commandQueue_.emplace_back(CommandType::Push, dialog, level);
    processCommandQueue();
}

void DialogManager::popDialog(Dialog* dialog) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getParent() != nullptr);

    auto level = dialog->getDialogLevel();
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);

    updateCurrentScene();
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

std::size_t DialogManager::getTopDialogLevel() {
    auto topDialog = getTopDialog();
    if (topDialog != nullptr) {
        return topDialog->getDialogLevel();
    }
    return 0;
}

void DialogManager::updateCurrentScene() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    auto transitionScene =
        dynamic_cast<cocos2d::TransitionScene*>(currentScene);
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
                commandQueue_.erase(
                    std::next(commandQueue_.begin(), static_cast<DiffType>(i)));
                pushDialogImmediately(command.dialog, command.level);
                break;
            }
        } else if (command.type == CommandType::Pop) {
            if ((currentLevel_ == command.level ||
                 command.level == Dialog::TopLevel) &&
                getTopDialog() == command.dialog) {
                commandQueue_.erase(
                    std::next(commandQueue_.begin(), static_cast<DiffType>(i)));
                popDialogImmediately(command.dialog);
                break;
            }
        } else {
            CC_ASSERT(false);
        }
    }
}

void DialogManager::pushDialogImmediately(Dialog* dialog, std::size_t level) {
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);

    lock(dialog);
    dialog->onDialogWillShow();

    auto parent = getRunningNode();
    pauseAllDialog(parent, dialog);

    ++currentLevel_;
    dialog->dialogLevel_ = currentLevel_;
    dialogStack_.emplace_back(dialog);
    parent->addChild(dialog->getContainer(), Dialog::ContainerLocalZOrder);

    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    for (auto&& action : dialog->getShowingTransitions()) {
        actions.pushBack(cocos2d::TargetedAction::create(dialog, action));
    }

    actions.pushBack(cocos2d::CallFunc::create([this, dialog] {
        unlock(dialog);
        dialog->setActive(true);
        dialog->onDialogDidShow();
    }));

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(cocos2d::Sequence::create(actions));
}

void DialogManager::popDialogImmediately(Dialog* dialog) {
    CC_ASSERT(dialog == getTopDialog());
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog,
                    dialog->getDialogLevel());

    dialog->setActive(false);
    lock(dialog);
    dialog->onDialogWillHide();

    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    for (auto&& action : dialog->getHidingTransitions()) {
        actions.pushBack(cocos2d::TargetedAction::create(dialog, action));
    }

    actions.pushBack(cocos2d::CallFunc::create([this, dialog] {
        dialog->getContainer()->removeFromParent();
        dialogStack_.pop_back();
        --currentLevel_;
        auto parent = getRunningNode();
        resumeAllDialog(parent, dialog);
        unlock(dialog);
        dialog->onDialogDidHide();
    }));

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(cocos2d::Sequence::create(actions));
}

cocos2d::Node* DialogManager::getRunningNode() {
    updateCurrentScene();
    auto dialog = getTopDialog();
    if (dialog != nullptr) {
        return dialog->getParent();
    }
    return currentScene_;
}

bool DialogManager::isLocked() {
    updateCurrentScene();
    return lockingDialog_ != nullptr;
}

void DialogManager::unlock(Dialog* dialog) {
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog,
                    dialog->getDialogLevel());
    CCASSERT(lockingDialog_ == dialog,
             "Attempted to unlock not the top dialog!");
    lockingDialog_ = nullptr;
}

void DialogManager::lock(Dialog* dialog) {
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog,
                    dialog->getDialogLevel());
    CC_ASSERT(lockingDialog_ == nullptr);
    updateCurrentScene();
    lockingDialog_ = dialog;
}
} // namespace dialog
} // namespac ee
