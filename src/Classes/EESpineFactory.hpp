//
//  EESpineFactory.hpp
//  ee-library
//
//  Created by Zinge on 1/9/17.
//
//

#ifndef EE_LIBRARY_SPINE_FACTORY_HPP_
#define EE_LIBRARY_SPINE_FACTORY_HPP_

#include <functional>
#include <string>
#include <map>
#include <unordered_map>

#include "EEMacro.hpp"

struct spSkeletonJson;
struct spSkeletonData;

namespace spine {
class SkeletonAnimation;
} // namespace spine

NS_EE_BEGIN
using SpineAtlasName = std::string;
using SpineJsonDeleter = std::function<void(spSkeletonJson* json)>;
using SpineJsonPtr = std::unique_ptr<spSkeletonJson, SpineJsonDeleter>;

using SpineDataName = std::string;
using SpineDataDeleter = std::function<void(spSkeletonData* data)>;
using SpineDataPtr = std::unique_ptr<spSkeletonData, SpineDataDeleter>;

class SpineFactory final {
public:
    static SpineFactory* getInstance();

    spine::SkeletonAnimation* createAnimation(const SpineDataName& dataFile,
                                              const SpineAtlasName& atlasFile,
                                              float scale);

    /// Gets (or creates if not cached) skeleton data for the specified data
    /// filename, atlas filename and scale.
    /// @return Reference to the cached skeleton data.
    const SpineDataPtr& getSkeletonData(const SpineDataName& dataName,
                                        const SpineAtlasName& atlasName,
                                        float scale);

    /// Gets (or creates if not cached) skeleton json for the specified atlas
    /// filename and scale.
    /// @return Reference to the cached skeleton json.
    const SpineJsonPtr& getSkeletonJson(const SpineAtlasName& atlasName,
                                        float scale);

private:
    SpineFactory() = default;
    ~SpineFactory() = default;

    /// Stores (atlas filename, skeleton json).
    std::unordered_map<SpineAtlasName, SpineJsonPtr> cachedSkeletonJson_;

    /// Stores (data filename, skeleton data).
    std::map<std::pair<SpineDataName, float>, SpineDataPtr> cachedSkeletonData_;
};
NS_EE_END

#endif /* EE_LIBRARY_SPINE_FACTORY_HPP_ */
