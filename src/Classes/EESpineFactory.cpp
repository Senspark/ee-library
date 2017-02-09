//
//  EESpineFactory.cpp
//  ee-library
//
//  Created by Zinge on 1/9/17.
//
//

#include "EESpineFactory.hpp"

#include <spine/spine-cocos2dx.h>

NS_EE_BEGIN
namespace {
const SpineAtlasDeleter& getAtlasDeleter() {
    static SpineAtlasDeleter deleter =
        [](spAtlas* atlas) { spAtlas_dispose(atlas); };
    return deleter;
}

const SpineDataDeleter& getSkeletonDataDeleter() {
    static SpineDataDeleter deleter =
        [](spSkeletonData* data) { spSkeletonData_dispose(data); };
    return deleter;
}
} // namespace

SpineFactory* SpineFactory::getInstance() {
    static SpineFactory sharedInstance;
    return &sharedInstance;
}

const SpineAtlasPtr& SpineFactory::getAtlas(const SpineAtlasName& atlasName) {
    auto iter = cachedAtlas_.find(atlasName);
    if (iter == cachedAtlas_.cend()) {
        auto atlas = spAtlas_createFromFile(atlasName.c_str(), nullptr);
        CCASSERT(atlas != nullptr, "Error reading atlas file.");

        auto ptr = SpineAtlasPtr(atlas, getAtlasDeleter());
        iter = cachedAtlas_.emplace(atlasName, std::move(ptr)).first;
    }
    return iter->second;
}

const SpineDataPtr&
SpineFactory::getSkeletonData(const SpineDataName& dataName,
                              const SpineAtlasName& atlasName, float scale) {
    auto iter = cachedSkeletonData_.find(std::make_pair(dataName, scale));
    if (iter == cachedSkeletonData_.cend()) {
        auto&& atlas = getAtlas(atlasName);

        auto attachmentLoader =
            &Cocos2dAttachmentLoader_create(atlas.get())->super;
        auto json = spSkeletonJson_createWithLoader(attachmentLoader);
        json->scale = scale;

        auto skeletonData =
            spSkeletonJson_readSkeletonDataFile(json, dataName.c_str());
        auto ptr = SpineDataPtr(skeletonData, getSkeletonDataDeleter());
        CCASSERT(ptr, "Error reading skeleton data file.");

        spSkeletonJson_dispose(json);
        spAttachmentLoader_dispose(attachmentLoader);

        iter =
            cachedSkeletonData_.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(dataName, scale),
                                        std::forward_as_tuple(std::move(ptr)))
                .first;
    }
    return iter->second;
}

spine::SkeletonAnimation*
SpineFactory::createAnimation(const std::string& dataFile,
                              const std::string& atlasFile, float scale) {
    auto&& data = getSkeletonData(dataFile, atlasFile, scale);
    auto skeleton = spine::SkeletonAnimation::createWithData(data.get());
    return skeleton;
}
NS_EE_END
