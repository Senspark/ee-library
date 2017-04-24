//
//  EESceneSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 4/21/17.
//
//

#include "EESceneSwitcher.hpp"
#include "EEManagedScene.hpp"

#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <2d/CCNode.h>
#include <2d/CCSpriteFrameCache.h>
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
    loadedImageCount_ = 0;
    loadedAtlasCount_ = 0;
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

void SceneSwitcher::visit(cocos2d::Renderer* renderer,
                          const cocos2d::Mat4& transform, std::uint32_t flags) {
    if (phase_ == Phase::None) {
        CC_ASSERT(false);
        return;
    }
    if (phase_ == Phase::Pre) {
        _outScene->visit(renderer, transform, flags);
    } else if (phase_ == Phase::Post) {
        _inScene->visit(renderer, transform, flags);
    }
    Super::visit(renderer, transform, flags);
}

void SceneSwitcher::draw(cocos2d::Renderer* renderer,
                         const cocos2d::Mat4& transform, std::uint32_t flags) {
    Super::draw(renderer, transform, flags);
}

SceneSwitcher*
SceneSwitcher::setInSceneConstructor(const SceneConstructor& constructor) {
    inSceneConstructor_ = constructor;
    inLayerConstructor_ = nullptr;
    return this;
}

SceneSwitcher*
SceneSwitcher::setInLayerConstructor(const LayerConstructor& constructor) {
    inLayerConstructor_ = constructor;
    inSceneConstructor_ = nullptr;
    return this;
}

SceneSwitcher* SceneSwitcher::addImage(const std::string& imageName) {
    images_.push_back(imageName);
    return this;
}

SceneSwitcher* SceneSwitcher::addAtlas(const std::string& plistName,
                                       const std::string& imageName) {
    atlases_.emplace_back(plistName, imageName);
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
SceneSwitcher::addPostPhaseAction(cocos2d::FiniteTimeAction* action) {
    postActions_.pushBack(action);
    return this;
}

void SceneSwitcher::run() {
    _outScene = _director->getRunningScene();
    CC_ASSERT(_outScene != nullptr);
    CC_ASSERT(inSceneConstructor_ || inLayerConstructor_);
    _outScene->retain();
    _director->replaceScene(this);
}

cocos2d::Scene* SceneSwitcher::createInScene() const {
    if (inSceneConstructor_) {
        return inSceneConstructor_();
    }
    auto layer = inLayerConstructor_();
    auto scene = ManagedScene::create();
    scene->addChild(layer);
    return scene;
}

void SceneSwitcher::onPhaseBegan(Phase phase) {
    phase_ = phase;
    if (phase == Phase::Pre) {
        _outScene->onExitTransitionDidStart();
        _eventDispatcher->setEnabled(false);
        preActions_.pushBack(cocos2d::CallFunc::create(
            std::bind(&SceneSwitcher::onPhaseEnded, this, phase)));
        actor_->runAction(cocos2d::Sequence::create(preActions_));
    }
    if (phase == Phase::In) {
        imagesLoaded_ = false;
        inActionsDone_ = false;
        loadNextItem();
        inActions_.pushBack(cocos2d::CallFunc::create([this, phase] {
            inActionsDone_ = true;
            if (imagesLoaded_) {
                onPhaseEnded(phase);
            }
        }));
        actor_->runAction(cocos2d::Sequence::create(inActions_));
    }
    if (phase == Phase::Post) {
        _inScene = createInScene();
        _inScene->retain();
        _inScene->setVisible(true);
        _inScene->onEnter();
        postActions_.pushBack(
            cocos2d::CallFunc::create(std::bind(&SceneSwitcher::finish, this)));
        actor_->runAction(cocos2d::Sequence::create(postActions_));
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
    }
}

bool SceneSwitcher::loadNextItem() {
    if (loadNextImage() || loadNextAtlas()) {
        return true;
    }
    imagesLoaded_ = true;
    if (inActionsDone_) {
        onPhaseEnded(Phase::In);
    }
    return false;
}

bool SceneSwitcher::loadNextImage() {
    if (loadedImageCount_ == images_.size()) {
        return false;
    }
    auto textureCache = _director->getTextureCache();
    auto imageName = images_.at(loadedImageCount_++);
    CC_ASSERT(textureCache->getTextureForKey(imageName) == nullptr);
    textureCache->addImageAsync(imageName,
                                std::bind(&SceneSwitcher::onImageLoaded, this,
                                          std::placeholders::_1, imageName));
    return true;
}

bool SceneSwitcher::loadNextAtlas() {
    if (loadedAtlasCount_ == atlases_.size()) {
        return false;
    }
    auto textureCache = _director->getTextureCache();
    std::string plistName;
    std::string imageName;
    std::tie(plistName, imageName) = atlases_.at(loadedAtlasCount_++);
    CC_ASSERT(textureCache->getTextureForKey(imageName) == nullptr);
    textureCache->addImageAsync(
        imageName, std::bind(&SceneSwitcher::onAtlasLoaded, this,
                             std::placeholders::_1, plistName, imageName));
    return true;
}

void SceneSwitcher::onImageLoaded(cocos2d::Texture2D* texture,
                                  const std::string& imageName) {
    loadNextItem();
}

void SceneSwitcher::onAtlasLoaded(cocos2d::Texture2D* texture,
                                  const std::string& plistName,
                                  const std::string& imageName) {
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    CC_ASSERT(not spriteFrameCache->isSpriteFramesWithFileLoaded(plistName));
    spriteFrameCache->addSpriteFramesWithFile(plistName, texture);
    loadNextItem();
}
} // namespace ee