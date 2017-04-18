//
//  DialogBase.cpp
//  ee-library-test
//
//  Created by Zinge on 4/18/17.
//
//

#include "DialogBase.hpp"

#include <EEHeader.hpp>

DialogBase* DialogBase::openNode() {
    auto library =
        cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    library->registerNodeLoader("DialogBase", Loader::loader());

    auto reader = std::make_unique<cocosbuilder::CCBReader>(library);
    auto node = reader->readNodeGraphFromFile("scenes/dialog_base");

    auto result = dynamic_cast<DialogBase*>(node);
    return result;
}

bool DialogBase::init() {
    if (not Super::init()) {
        return false;
    }
    setPosition(_director->getWinSize() / 2);
    animateShow_ = false;
    animateHide_ = false;
    return true;
}

void DialogBase::onEnter() {
    Super::onEnter();
}

void DialogBase::onExit() {
    Super::onExit();
}

void DialogBase::onDialogWillShow() {
    Super::onDialogWillShow();

    auto layerColor = cocos2d::LayerColor::create(cocos2d::Color4B::BLACK);
    layerColor->setName("layer_color");
    getContainer()->addChild(layerColor, -1);

    if (animateShow_) {
        setScale(0.0f);
        runAction(cocos2d::EaseElasticOut::create(
            cocos2d::ScaleTo::create(showingDuration_, 1.0f), 0.6f));

        layerColor->setOpacity(0);
        layerColor->runAction(cocos2d::FadeTo::create(showingDuration_, 10));
    } else {
        layerColor->setOpacity(10);
    }
}

void DialogBase::onDialogWillHide() {
    Super::onDialogWillHide();

    if (animateHide_) {
        runAction(cocos2d::EaseElasticIn::create(
            cocos2d::ScaleTo::create(hidingDuration_, 0.0f), 0.6f));

        auto layerColor = getContainer()->getChildByName("layer_color");
        layerColor->runAction(cocos2d::FadeOut::create(hidingDuration_));
    }
}

void DialogBase::onNodeLoaded(cocos2d::Node* node,
                              cocosbuilder::NodeLoader* nodeLoader) {
    background_->setContentSize(getContentSize());
}

bool DialogBase::onAssignCCBMemberVariable(cocos2d::Ref* pTarget,
                                           const char* pMemberVariableName,
                                           cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(background_);
    return false;
}

DialogBase* DialogBase::addShowAnimation(float duration) {
    addShowingTransitions(cocos2d::DelayTime::create(duration));
    animateShow_ = true;
    showingDuration_ = duration;
    return this;
}

DialogBase* DialogBase::addHideAnimation(float duration) {
    addHidingTransitions(cocos2d::DelayTime::create(duration));
    animateHide_ = true;
    hidingDuration_ = duration;
    return this;
}

void DialogBase::setContentSize(const cocos2d::Size& size) {
    Super::setContentSize(size);
    if (background_ != nullptr) {
        background_->setContentSize(size);
    }
}
