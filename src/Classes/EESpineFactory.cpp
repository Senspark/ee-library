//
//  EESpineFactory.cpp
//  ee-library
//
//  Created by Zinge on 1/9/17.
//
//

#include <cassert>

#include "EESpineFactory.hpp"

#include <spine/Cocos2dAttachmentLoader.h>
#include <spine/SkeletonAnimation.h>

namespace ee {
namespace detail {
struct SpineData {
    explicit SpineData(spSkeletonData* data_, spAttachmentLoader* loader_)
        : data(data_)
        , loader(loader_) {}

    spSkeletonData* data;
    spAttachmentLoader* loader;
};
} // namespace

namespace {
const detail::SpineAtlasDeleter& getAtlasDeleter() {
    static detail::SpineAtlasDeleter deleter =
        [](spAtlas* atlas) { spAtlas_dispose(atlas); };
    return deleter;
}

const detail::SpineDataDeleter& getSkeletonDataDeleter() {
    static detail::SpineDataDeleter deleter = [](detail::SpineData* data) {
        spSkeletonData_dispose(data->data);
        spAttachmentLoader_dispose(data->loader);
    };
    return deleter;
}

SpineFactory* instance_;
} // namespace

SpineFactory* SpineFactory::getInstance() {
    static SpineFactory sharedInstance;
    return &sharedInstance;
}

spAtlas* SpineFactory::getAtlas(const std::string& atlasName) {
    auto iter = cachedAtlas_.find(atlasName);
    if (iter == cachedAtlas_.cend()) {
        auto atlas = spAtlas_createFromFile(atlasName.c_str(), nullptr);
        assert(atlas != nullptr);

        auto ptr = detail::SpineAtlasPtr(atlas, getAtlasDeleter());
        iter = cachedAtlas_.emplace(atlasName, std::move(ptr)).first;
    }
    return iter->second.get();
}

spSkeletonData* SpineFactory::getSkeletonData(const std::string& dataName,
                                              const std::string& atlasName,
                                              float scale) {
    auto iter = cachedSkeletonData_.find(std::make_pair(dataName, scale));
    if (iter == cachedSkeletonData_.cend()) {
        auto&& atlas = getAtlas(atlasName);

        auto attachmentLoader = &Cocos2dAttachmentLoader_create(atlas)->super;
        auto json = spSkeletonJson_createWithLoader(attachmentLoader);
        json->scale = scale;

        auto skeletonData =
            spSkeletonJson_readSkeletonDataFile(json, dataName.c_str());
        auto ptr = detail::SpineDataPtr(
            new detail::SpineData(skeletonData, attachmentLoader),
            getSkeletonDataDeleter());
        assert(ptr);

        spSkeletonJson_dispose(json);

        iter =
            cachedSkeletonData_.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(dataName, scale),
                                        std::forward_as_tuple(std::move(ptr)))
                .first;
    }
    return iter->second->data;
}

spine::SkeletonAnimation*
SpineFactory::createAnimation(const std::string& dataFile,
                              const std::string& atlasFile, float scale) {
    auto&& data = getSkeletonData(dataFile, atlasFile, scale);
    auto skeleton = spine::SkeletonAnimation::createWithData(data);
    return skeleton;
}
} // namespace ee