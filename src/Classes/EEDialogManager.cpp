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
#include "EEAction.hpp"
#include "EEScopeGuard.hpp"

#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <2d/CCTransition.h>
#include <base/CCDirector.h>

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

bool isTransitionScene(cocos2d::Node* scene) {
    auto transitionScene = dynamic_cast<cocos2d::TransitionScene*>(scene);
    if (transitionScene != nullptr) {
        return true;
    }
    return false;
}
} // namespace

DialogManager* DialogManager::getInstance() {
    static DialogManager sharedInstance;
    return &sharedInstance;
}

void DialogManager::replaceScene(cocos2d::Scene* scene) {
    resetDialogGraph();
    cocos2d::Director::getInstance()->replaceScene(scene);
}

void DialogManager::pushDialog(Dialog* dialog) {
    pushDialog(dialog, Dialog::TopLevel);
}

void DialogManager::pushDialog(Dialog* dialog, std::size_t level) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getContainer() != nullptr);
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);

    updateCurrentScene();
    pushCommand(Command(CommandType::Push, dialog, level));
    processCommandQueue();
}

void DialogManager::popDialog(Dialog* dialog) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getParent() != nullptr);

    auto level = dialog->getDialogLevel();
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);

    updateCurrentScene();
    pushCommand(Command(CommandType::Pop, dialog, level));
    processCommandQueue();
}

void DialogManager::popDialog() {
    popDialog(getTopDialog());
}

Dialog* DialogManager::getDialog(std::size_t level) {
    updateCurrentScene();
    return getDialogWithoutUpdate(level);
}

Dialog* DialogManager::getTopDialog() {
    return getDialog(Dialog::TopLevel);
}

Dialog* DialogManager::getDialogWithoutUpdate(std::size_t level) const {
    if (dialogStack_.empty()) {
        return nullptr;
    }
    if (level == Dialog::TopLevel) {
        level = dialogStack_.size();
    }
    CC_ASSERT(1 <= level && level <= dialogStack_.size());
    return dialogStack_.at(level - 1).dialog;
}

Dialog* DialogManager::getTopDialogWithoutUpdate() const {
    return getDialogWithoutUpdate(Dialog::TopLevel);
}

std::size_t DialogManager::getTopDialogLevel() {
    auto topDialog = getTopDialog();
    if (topDialog != nullptr) {
        return topDialog->getDialogLevel();
    }
    return 0;
}

void DialogManager::resetDialogGraph() {
    dialogStack_.clear();
    lockingDialog_ = nullptr;
    currentLevel_ = 0;
    commandQueue_.clear();
}

bool DialogManager::updateCurrentScene() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    if (currentScene == currentScene_) {
        // Not reliable: system may use the same memory address for newly
        // created objects.
        return false;
    }

    LOG_FUNC();
    currentScene_ = currentScene;
    resetDialogGraph();
    return true;
}

bool DialogManager::processCommandQueue() {
    if (isLocked()) {
        return false;
    }
    using DiffType = decltype(commandQueue_)::difference_type;
    for (std::size_t i = 0; i < commandQueue_.size(); ++i) {
        if (processCommand(commandQueue_.at(i))) {
            commandQueue_.erase(
                std::next(commandQueue_.begin(), static_cast<DiffType>(i)));
            return true;
        }
    }
    return false;
}

bool DialogManager::processCommand(const Command& command) {
    if (command.type == CommandType::Push) {
        return processPushCommand(command.dialog, command.level);
    }
    if (command.type == CommandType::Pop) {
        return processPopCommand(command.dialog, command.level);
    }
    CC_ASSERT(false);
    return false;
}

bool DialogManager::processPushCommand(Dialog* dialog, std::size_t level) {
    if (level == Dialog::TopLevel || level == currentLevel_ + 1) {
        pushDialogImmediately(dialog, level);
        return true;
    }
    return false;
}

bool DialogManager::processPopCommand(Dialog* dialog, std::size_t level) {
    if (level == Dialog::TopLevel || level == currentLevel_) {
        if (dialog != getTopDialogWithoutUpdate()) {
            // Attempted to pop not the top dialog!!!
            return false;
        }
        popDialogImmediately(dialog);
        return true;
    }
    return false;
}

bool DialogManager::pushCommand(const Command& command) {
    for (auto&& cmd : commandQueue_) {
        if (cmd.type == command.type && cmd.dialog == command.dialog) {
            // Exist.
            return false;
        }
    }
    commandQueue_.push_back(command);
    return true;
}

void DialogManager::pushDialogImmediately(Dialog* dialog, std::size_t level) {
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog, level);
    CC_ASSERT(not updateCurrentScene());

    lock(dialog);
    dialog->onDialogWillShow();

    auto parent = getRunningNode();
    pauseAllDialog(parent, dialog);

    ++currentLevel_;
    dialog->dialogLevel_ = currentLevel_;
    dialogStack_.emplace_back(dialog);
    parent->addChild(dialog->getContainer(), Dialog::ContainerLocalZOrder);

    auto sequence = Sequence::create();
    for (auto&& action : dialog->showingTransitions_) {
        sequence->then(cocos2d::TargetedAction::create(dialog, action));
    }

    auto unlocker = std::make_shared<ScopeGuard>([this, dialog] {
        LOG_FUNC_FORMAT("unlocker: dialog = %p level = %zu", dialog,
                        dialog->getDialogLevel());
        unlock(dialog);
        processCommandQueue();
    });

    sequence->then([this, dialog, unlocker] {
        if (updateCurrentScene()) {
            unlocker->dismiss();
        } else {
            dialog->onDialogDidShow();
            unlocker->invoke();
            dialog->setActive(true);
        }
    });

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(sequence);
}

void DialogManager::popDialogImmediately(Dialog* dialog) {
    CC_ASSERT(dialog == getTopDialog());
    CC_ASSERT(not updateCurrentScene());
    LOG_FUNC_FORMAT("dialog = %p level = %zu", dialog,
                    dialog->getDialogLevel());

    dialog->setActive(false);
    lock(dialog);
    dialog->onDialogWillHide();

    auto sequence = Sequence::create();
    for (auto&& action : dialog->hidingTransitions_) {
        sequence->then(cocos2d::TargetedAction::create(dialog, action));
    }

    auto unlocker = std::make_shared<ScopeGuard>([this, dialog] {
        LOG_FUNC_FORMAT("unlocker: dialog = %p level = %zu", dialog,
                        dialog->getDialogLevel());
        unlock(dialog);
        processCommandQueue();
    });

    sequence->then([this, dialog, unlocker] {
        auto container = dialog->getContainer();
        auto realParent = container->getParent();
        container->removeFromParent();

        if (updateCurrentScene()) {
            unlocker->dismiss();
        } else {
            // When the dialog doesn't have any hiding transition, i.e.
            // transitionAction_ won't run any TargetedAction, so the dialog
            // will not be guarded by the TargetedAction (TargetedAction will
            // keep a reference to the targeted node).
            // We have to manually guard the dialog here.
            auto guard = ee::make_ref_guard(dialog);

            dialogStack_.pop_back();
            --currentLevel_;

            auto parent = getRunningNode();
            CC_ASSERT(parent == realParent);
            resumeAllDialog(parent, dialog);

            dialog->onDialogDidHide();
            unlocker->invoke();
        }
    });

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(sequence);
}

cocos2d::Node* DialogManager::getRunningNode() const {
    auto dialog = getTopDialogWithoutUpdate();
    if (dialog != nullptr) {
        return dialog->getParent();
    }
    return currentScene_;
}

bool DialogManager::isLocked() const {
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
    lockingDialog_ = dialog;
}
} // namespace dialog
} // namespac ee
