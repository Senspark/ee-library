//
//  DialogTestScene.cpp
//  ee-library-test
//
//  Created by Zinge on 4/18/17.
//
//

#include "DialogTestScene.hpp"
#include "DialogBase.hpp"

#include <EEHeader.hpp>

namespace {
constexpr auto switch_scene_duration = 1.0f;
} // namespace

void DialogTestScene::openScene() {
    auto library =
        cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    library->registerNodeLoader("DialogTestScene", Loader::loader());
    library->registerNodeLoader("CCButton", ee::UiButtonLoader::loader());

    auto reader = std::make_unique<cocosbuilder::CCBReader>(library);
    auto scene =
        reader->createSceneWithNodeGraphFromFile("scenes/dialog_test_scene");

    cocos2d::Director::getInstance()->replaceScene(
        cocos2d::TransitionFade::create(switch_scene_duration, scene,
                                        cocos2d::Color3B::BLACK));
}

bool DialogTestScene::init() {
    if (not Super::init()) {
        return false;
    }
    pushMode_ = AnimationMode::Instant;
    popMode_ = AnimationMode::Instant;
    return true;
}

void DialogTestScene::onEnter() {
    Super::onEnter();
}

void DialogTestScene::onExit() {
    Super::onExit();
}

void DialogTestScene::onNodeLoaded(cocos2d::Node* node,
                                   cocosbuilder::NodeLoader* nodeLoader) {
    pushModeButton_->addComponent(ee::DialogComponent::create());
    popModeButton_->addComponent(ee::DialogComponent::create());
    pushButton_->addComponent(ee::DialogComponent::create());
    popButton_->addComponent(ee::DialogComponent::create());
    pushAndPopButton_->addComponent(ee::DialogComponent::create());
    popAndPushButton_->addComponent(ee::DialogComponent::create());
    changeButton_->addComponent(ee::DialogComponent::create());
}

bool DialogTestScene::onAssignCCBMemberVariable(cocos2d::Ref* pTarget,
                                                const char* pMemberVariableName,
                                                cocos2d::Node* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(pushModeButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(popModeButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(pushButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(popButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(pushAndPopButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(popAndPushButton_);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(changeButton_);
    return false;
}

cocos2d::SEL_MenuHandler
DialogTestScene::onResolveCCBCCMenuItemSelector(cocos2d::Ref* pTarget,
                                                const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPushModeButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPopModeButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPushButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPopButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPushAndPopButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onPopAndPushButtonPressed);
    CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(
        DialogTestScene::onChangeButtonPressed);
    return nullptr;
}

cocos2d::extension::Control::Handler
DialogTestScene::onResolveCCBCCControlSelector(cocos2d::Ref* pTarget,
                                               const char* pSelectorName) {
    return nullptr;
}

void DialogTestScene::resume() {
    Super::resume();
}

void DialogTestScene::pause() {
    Super::pause();
}

void DialogTestScene::onPushModeButtonPressed() {
    if (pushMode_ == AnimationMode::Delayed) {
        pushMode_ = AnimationMode::Instant;
        pushModeButton_->setTitleText("PUSH:INSTANT");
    } else {
        pushMode_ = AnimationMode::Delayed;
        pushModeButton_->setTitleText("PUSH:DELAYED");
    }
}

void DialogTestScene::onPopModeButtonPressed() {
    if (popMode_ == AnimationMode::Delayed) {
        popMode_ = AnimationMode::Instant;
        popModeButton_->setTitleText("POP:INSTANT");
    } else {
        popMode_ = AnimationMode::Delayed;
        popModeButton_->setTitleText("POP:DELAYED");
    }
}

void DialogTestScene::onPushButtonPressed() {
    push();
}

void DialogTestScene::onPopButtonPressed() {
    pop();
}

void DialogTestScene::onPushAndPopButtonPressed() {
    push();
    pop();
}

void DialogTestScene::onPopAndPushButtonPressed() {
    pop();
    push();
}

void DialogTestScene::onChangeButtonPressed() {
    change();
}

void DialogTestScene::push() {
    auto dialog = DialogBase::openNode();
    auto width = cocos2d::random<float>(100, 200);
    auto height = cocos2d::random<float>(60, 120);
    dialog->setContentSize(cocos2d::Size(width, height));
    if (pushMode_ == AnimationMode::Delayed) {
        dialog->addShowAnimation(1.0f);
    }
    dialog->show();
}

void DialogTestScene::pop() {
    auto dialog = ee::DialogManager::getInstance()->getTopDialog();
    if (dialog != nullptr) {
        if (popMode_ == AnimationMode::Delayed) {
            auto base = dynamic_cast<DialogBase*>(dialog);
            base->addHideAnimation(1.0f);
        }
        dialog->hide();
    }
}

void DialogTestScene::change() {
    openScene();
}
