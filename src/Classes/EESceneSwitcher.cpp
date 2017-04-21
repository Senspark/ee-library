//
//  EESceneSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 4/21/17.
//
//

#include "EESceneSwitcher.hpp"

#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <2d/CCNode.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <renderer/CCTextureCache.h>

namespace ee {
bool SceneSwitcher::init() {
    if (not Super::init()) {
        return false;
    }
    phase_ = Phase::None;
    imagesLoaded_ = true;
    inActionsDone_ = true;
    actor_ = cocos2d::Node::create();
    actor_->setVisible(false);
    addChild(actor_);
    return true;
}

void SceneSwitcher::onEnter() {
    Super::onEnter();
    onPhaseBegan(Phase::Pre);
}

void SceneSwitcher::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
}

void SceneSwitcher::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

void SceneSwitcher::onExit() {
    Super::onExit();
    onPhaseEnded(Phase::Post);
}

void SceneSwitcher::draw(cocos2d::Renderer* renderer,
                         const cocos2d::Mat4& transform, std::uint32_t flags) {
    if (phase_ == Phase::None) {
        CC_ASSERT(false);
        return;
    }
    if (phase_ == Phase::Pre) {
        _inScene->visit(renderer, transform, flags);
    } else if (phase_ == Phase::Post) {
        _outScene->visit(renderer, transform, flags);
    }
    Super::draw(renderer, transform, flags);
}

SceneSwitcher*
SceneSwitcher::setInSceneConstructor(const SceneConstructor& constructor) {
    inSceneConstructor_ = constructor;
    return this;
}

SceneSwitcher* SceneSwitcher::addImage(const std::string& imageName) {
    imageNames_.push(imageName);
    return this;
}

SceneSwitcher*
SceneSwitcher::addPrePhaseAction(cocos2d::FiniteTimeAction* action) {
    preActions_.pushBack(action);
    return this;
}

SceneSwitcher*
SceneSwitcher::addInPhaseAction(cocos2d::FiniteTimeAction* action) {
    inActions_.pushBack(action);
    return this;
}

SceneSwitcher*
SceneSwitcher::addPosePhaseAction(cocos2d::FiniteTimeAction* action) {
    postActions_.pushBack(action);
    return this;
}

void SceneSwitcher::run() {
    _outScene = _director->getRunningScene();
    CC_ASSERT(_outScene != nullptr);
    _outScene->retain();
    _director->replaceScene(this);
}

void SceneSwitcher::onPhaseBegan(Phase phase) {
    if (phase == Phase::Pre) {
        _outScene->onExitTransitionDidStart();
        _eventDispatcher->setEnabled(false);
        actor_->runAction(cocos2d::Sequence::create(
            cocos2d::Sequence::create(preActions_),
            cocos2d::CallFunc::create(
                std::bind(&SceneSwitcher::onPhaseEnded, this, phase)),
            nullptr));
    }
    if (phase == Phase::In) {
        imagesLoaded_ = false;
        inActionsDone_ = false;
        loadNextImage();
        actor_->runAction(
            cocos2d::Sequence::create(cocos2d::Sequence::create(preActions_),
                                      cocos2d::CallFunc::create([this, phase] {
                                          inActionsDone_ = true;
                                          if (imagesLoaded_) {
                                              onPhaseEnded(phase);
                                          }
                                      }),
                                      nullptr));
    }
    if (phase == Phase::Post) {
        _inScene = inSceneConstructor_();
        _inScene->onEnter();
        actor_->runAction(cocos2d::Sequence::createWithTwoActions(
            cocos2d::Sequence::create(postActions_),
            cocos2d::CallFunc::create(
                std::bind(&SceneSwitcher::onPhaseEnded, this, phase))));
    }
}

void SceneSwitcher::onPhaseEnded(Phase phase) {
    if (phase == Phase::Pre) {
        _outScene->onExit();
        onPhaseBegan(Phase::In);
    }
    if (phase == Phase::In) {
        onPhaseBegan(Phase::Post);
    }
    if (phase == Phase::Post) {
        _eventDispatcher->setEnabled(true);
        _inScene->onEnterTransitionDidFinish();
        actor_->runAction(
            cocos2d::CallFunc::create(std::bind(&SceneSwitcher::finish, this)));
    }
}

bool SceneSwitcher::loadNextImage() {
    if (imageNames_.empty()) {
        imagesLoaded_ = true;
        if (inActionsDone_) {
            onPhaseEnded(Phase::In);
        }
        return false;
    }
    auto imageName = imageNames_.front();
    imageNames_.pop();
    auto textureCache = _director->getTextureCache();
    textureCache->addImageAsync(imageName,
                                std::bind(&SceneSwitcher::onImageLoaded, this,
                                          std::placeholders::_1, imageName));
    return true;
}

void SceneSwitcher::onImageLoaded(cocos2d::Texture2D* texture,
                                  const std::string& imageName) {
    loadNextImage();
}
} // namespace ee
