//
//  EEManagedScene.cpp
//  ee-library
//
//  Created by Zinge on 4/24/17.
//
//

#include "EEManagedScene.hpp"
#include "EEImageBuilder.hpp"

#include <2d/CCSpriteFrameCache.h>
#include <base/CCDirector.h>
#include <renderer/CCTextureCache.h>

namespace ee {
bool ManagedScene::init() {
    if (not Super::init()) {
        return false;
    }
    unloaded_ = false;
    return true;
}

ManagedScene::~ManagedScene() {
    unloadItems();
}

void ManagedScene::onEnter() {
    Super::onEnter();
}

void ManagedScene::onExit() {
    Super::onExit();
}

void ManagedScene::unloadItems() {
    if (unloaded_) {
        return;
    }
    auto textureCache = _director->getTextureCache();
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    for (auto&& image : images_) {
        if (image.useAtlas_) {
            CC_ASSERT(spriteFrameCache->isSpriteFramesWithFileLoaded(
                image.atlasName_));
            spriteFrameCache->removeSpriteFramesFromFile(image.atlasName_);
        }
        auto texture = textureCache->getTextureForKey(image.imageName_);
        CC_ASSERT(texture != nullptr);
        textureCache->removeTextureForKey(image.imageName_);
    }
}
} // namespace ee
