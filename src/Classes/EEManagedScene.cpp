//
//  EEManagedScene.cpp
//  ee-library
//
//  Created by Zinge on 4/24/17.
//
//

#include "EEManagedScene.hpp"

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
    unloadImages();
    unloadAtlases();
    unloaded_ = true;
}

void ManagedScene::unloadImages() {
    auto textureCache = _director->getTextureCache();
    for (auto&& imageName : images_) {
        CC_ASSERT(textureCache->getTextureForKey(imageName) != nullptr);
        textureCache->removeTextureForKey(imageName);
    }
}

void ManagedScene::unloadAtlases() {
    auto textureCache = _director->getTextureCache();
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    for (auto&& elt : atlases_) {
        std::string plistName;
        std::string imageName;
        std::tie(plistName, imageName) = elt;
        CC_ASSERT(spriteFrameCache->isSpriteFramesWithFileLoaded(plistName));
        spriteFrameCache->removeSpriteFramesFromFile(plistName);
        auto texture = textureCache->getTextureForKey(imageName);
        CC_ASSERT(texture != nullptr);
        CC_ASSERT(texture->getReferenceCount() == 1);
        textureCache->removeTextureForKey(imageName);
    }
}
} // namespace ee
