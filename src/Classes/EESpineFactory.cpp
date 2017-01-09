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
const SpineJsonDeleter& getSkeletonJsonDeleter() {
    static SpineJsonDeleter deleter =
        [](spSkeletonJson* json) { spSkeletonJson_dispose(json); };
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

const SpineJsonPtr&
SpineFactory::getSkeletonJson(const SpineAtlasName& atlasName, float scale) {
    auto iter = cachedSkeletonJson_.find(atlasName);
    if (iter == cachedSkeletonJson_.cend()) {
        auto atlas = spAtlas_createFromFile(atlasName.c_str(), nullptr);
        CCASSERT(atlas != nullptr, "Error reading atlas file.");

        auto ptr = SpineJsonPtr(spSkeletonJson_create(atlas),
                                getSkeletonJsonDeleter());
        iter = cachedSkeletonJson_.emplace(atlasName, std::move(ptr)).first;
    }
    auto&& result = iter->second;
    result->scale = scale;
    return result;
}

const SpineDataPtr&
SpineFactory::getSkeletonData(const SpineDataName& dataName,
                              const SpineAtlasName& atlasName, float scale) {
    auto iter = cachedSkeletonData_.find(std::make_pair(dataName, scale));
    if (iter == cachedSkeletonData_.cend()) {
        auto&& json = getSkeletonJson(atlasName, scale);
        auto ptr = SpineDataPtr(
            spSkeletonJson_readSkeletonDataFile(json.get(), dataName.c_str()),
            getSkeletonDataDeleter());
        CCASSERT(ptr, "Error reading skeleton data file.");
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
