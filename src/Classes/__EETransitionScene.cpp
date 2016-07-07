//
//  EETransitionDialog.cpp
//  ee-library
//
//  Created by enrevol on 12/1/15.
//
//

#include "__EETransitionScene.hpp"
#include "cocos2d.h"

namespace_ee_begin
TransitionScene::~TransitionScene() {
    CC_ASSERT(_filenames.empty());
}

void TransitionScene::onEnter() {
    Scene::onEnter();
    _outScene->onExitTransitionDidStart();
    _eventDispatcher->setEnabled(false);
}

void TransitionScene::onExit() {
    Scene::onExit();
    _eventDispatcher->setEnabled(true);
    _inScene->onEnterTransitionDidFinish();
}

void TransitionScene::run() {
    _outScene = cocos2d::Director::getInstance()->getRunningScene();
    CC_ASSERT(_outScene != nullptr);
    _outScene->retain();
    _currentPhase = Phase::Pre;
    onPhaseChanged(_currentPhase);
    cocos2d::Director::getInstance()->replaceScene(this);
}

void TransitionScene::loadTexture() {
    CC_ASSERT(_currentPhase == Phase::Pre);
    _currentPhase = Phase::In;
    onPhaseChanged(_currentPhase);
    _outScene->onExit();
    onLoadingTextureBegan();
    loadNextTexture();
}

const std::queue<std::string>& TransitionScene::getFilenames() const {
    return _filenames;
}

auto TransitionScene::getCurrentPhase() const -> Phase {
    return _currentPhase;
}

void TransitionScene::loadNextTexture() {
    CC_ASSERT(_currentPhase == Phase::In);
    if (_filenames.empty() == false) {
        auto filename = _filenames.front();
        _filenames.pop();
        cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(filename, std::bind(&TransitionScene::onTextureLoadedCallback, this, filename, std::placeholders::_1));
    } else {
        onLoadingTextureEndedCallback();
    }
}

void TransitionScene::onTextureLoadedCallback(const std::string& filename, cocos2d::Texture2D* texture) {
    CC_ASSERT(_currentPhase == Phase::In);
    onTextureLoaded(filename, texture);
    float delay = (float) cocos2d::Director::getInstance()->getAnimationInterval();
    auto sequence = cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create(CC_CALLBACK_0(TransitionScene::loadNextTexture, this)));
    runAction(sequence);
}

void TransitionScene::onLoadingTextureEndedCallback() {
    CC_ASSERT(_inSceneCallback);
    _inScene = _inSceneCallback();
    _inScene->retain();
    _inScene->onEnter();
    onLoadingTextureEnded();
    _currentPhase = Phase::Post;
    onPhaseChanged(_currentPhase);
}

void TransitionScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, std::uint32_t flags) {
    if (_currentPhase == Phase::Pre) {
        CC_ASSERT(_outScene != nullptr);
        _outScene->visit(renderer, transform, flags);
    } else if (_currentPhase == Phase::Post) {
        CC_ASSERT(_inScene != nullptr);
        _inScene->visit(renderer, transform, flags);
    }
    Scene::draw(renderer, transform, flags);
}

TransitionScene::Builder::~Builder() {}

auto TransitionScene::Builder::setInSceneCallback(const std::function<cocos2d::Scene*()>& callback) -> Builder& {
    _inSceneCallback = callback;
    return *this;
}

auto TransitionScene::Builder::addImageFilename(const std::string& filename) -> Builder& {
    _filenames.push(filename);
    return *this;
}

auto TransitionScene::Builder::addImageFilenames(const std::vector<std::string>& filenames) -> Builder& {
    for (auto&& filename : filenames) {
        addImageFilename(filename);
    }
    return *this;
}

TransitionScene* TransitionScene::Builder::build() const {
    auto result = create();
    result->init();
    result->autorelease();
    result->_inSceneCallback = _inSceneCallback;
    result->_filenames = _filenames;
    return result;
}

TransitionScene* TransitionScene::Builder::create() const {
    return new TransitionScene();
}
namespace_ee_end