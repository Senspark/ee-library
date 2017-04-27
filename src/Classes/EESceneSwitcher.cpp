//
//  EESceneSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 4/21/17.
//
//

#include "EESceneSwitcher.hpp"
#include "EEManagedScene.hpp"
#include "EEImageBuilder.hpp"

#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSpriteFrameCache.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <renderer/CCTextureCache.h>

namespace ee {
SceneSwitcher::~SceneSwitcher() {
    CCLOG("%s: %p", __PRETTY_FUNCTION__, this);
}

bool SceneSwitcher::init() {
    if (not Super::init()) {
        return false;
    }
    phase_ = Phase::None;
    imagesLoaded_ = true;
    inActionsDone_ = true;
    loadedImageCount_ = 0;
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

void SceneSwitcher::cleanup() {
    Super::cleanup();

    if (phase_ == Phase::Pre) {
        CC_ASSERT(_outScene != nullptr);
        if (_outScene != nullptr && _director->isSendCleanupToScene()) {
            _outScene->cleanup();
        }
    }
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

SceneSwitcher* SceneSwitcher::addImage(const ImageBuilder& builder) {
    images_.push_back(builder);
    return this;
}

SceneSwitcher* SceneSwitcher::addImage(const std::string& imageName) {
    return addImage(ImageBuilder().setImageName(imageName));
}

SceneSwitcher* SceneSwitcher::addAtlas(const std::string& plistName,
                                       const std::string& imageName) {
    return addImage(
        ImageBuilder().setAtlasName(plistName).setImageName(imageName));
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
    scene->images_ = images_;
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
        loadNextImage();
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
        postActions_.pushBack(cocos2d::CallFunc::create(
            std::bind(&SceneSwitcher::finish2, this)));
        actor_->runAction(cocos2d::Sequence::create(postActions_));
    }
}

void SceneSwitcher::onPhaseEnded(Phase phase) {
    if (phase == Phase::Pre) {
        // Cleanup the outScene.
        _outScene->onExit();

        if (_director->isSendCleanupToScene()) {
            _outScene->cleanup();
        }

        // Actually outScene should be released here instead of in the
        // destructor.
        _outScene->release();
        _outScene = nullptr;

        // cocos2d::Ref::printLeaks();

        actor_->runAction(cocos2d::CallFunc::create(
            std::bind(&SceneSwitcher::onPhaseBegan, this, Phase::In)));
    }
    if (phase == Phase::In) {
        onPhaseBegan(Phase::Post);
    }
    if (phase == Phase::Post) {
        _eventDispatcher->setEnabled(true);
        _inScene->onEnterTransitionDidFinish();
    }
}

bool SceneSwitcher::loadNextImage() {
    if (loadedImageCount_ == images_.size()) {
        imagesLoaded_ = true;
        if (inActionsDone_) {
            onPhaseEnded(Phase::In);
        }
        return false;
    }
    CCLOG("%s", __PRETTY_FUNCTION__);
    auto textureCache = _director->getTextureCache();
    auto&& image = images_.at(loadedImageCount_++);
    CC_ASSERT(textureCache->getTextureForKey(image.imageName_) == nullptr);
    textureCache->addImageAsync(image.imageName_,
                                std::bind(&SceneSwitcher::onImageLoaded, this,
                                          std::placeholders::_1, image));
    return true;
}

void SceneSwitcher::onImageLoaded(cocos2d::Texture2D* texture,
                                  const ImageBuilder& image) {
    if (image.mipmapEnabled_) {
        texture->generateMipmap();
    }

    auto params = cocos2d::Texture2D::TexParams();
    params.minFilter = image.minFilter_;
    params.magFilter = image.magFilter_;
    params.wrapS = image.wrapS_;
    params.wrapT = image.wrapT_;
    texture->setTexParameters(params);

    if (image.useAtlas_) {
        auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
        CC_ASSERT(not spriteFrameCache->isSpriteFramesWithFileLoaded(
            image.atlasName_));
        spriteFrameCache->addSpriteFramesWithFile(image.atlasName_, texture);
    }
    loadNextImage();
}

void SceneSwitcher::finish2() {
    // clean up
    _inScene->setVisible(true);
    _inScene->setPosition(0, 0);
    _inScene->setScale(1.0f);
    _inScene->setRotation(0.0f);
    _inScene->setAdditionalTransform(nullptr);

    // outScene is already released.

    scheduleOnce(std::bind(&SceneSwitcher::setNewScene2, this), 0.0f,
                 "___new_scene");
}

void SceneSwitcher::setNewScene2() {
    _director->replaceScene(_inScene);
}
} // namespace ee
