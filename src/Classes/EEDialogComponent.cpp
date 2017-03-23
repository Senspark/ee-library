//
//  EEDialogComponent.cpp
//  ee-library
//
//  Created by Zinge on 3/23/17.
//
//

#include "EEDialogComponent.hpp"
#include "EEUtils.hpp"

#include <2d/CCNode.h>

namespace ee {
namespace dialog {
const std::string DialogComponent::DefaultName =
    "___ee_library_dialog_component_";

DialogComponent* DialogComponent::create() {
    auto result = new DialogComponent();
    result->init();
    result->autorelease();
    return result;
}

bool DialogComponent::init() {
    if (not Super::init()) {
        return false;
    }
    setName(DefaultName);
    setPauseCallback([this] { ee::pauseAll(getOwner()); });
    setResumeCallback([this] { ee::resumeAll(getOwner()); });
    return true;
}

DialogComponent*
DialogComponent::setPauseCallback(const PauseCallback& callback) {
    pauseCallback_ = callback;
    return this;
}

DialogComponent*
DialogComponent::setResumeCallback(const ResumeCallback& callback) {
    resumeCallback_ = callback;
    return this;
}

void DialogComponent::resume() {
    if (resumeCallback_) {
        resumeCallback_();
    }
}

void DialogComponent::pause() {
    if (pauseCallback_) {
        pauseCallback_();
    }
}
} // namespace dialog
} // namespace ee
