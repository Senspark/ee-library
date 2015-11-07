#include "EEData.hpp"
#include "EEUtils.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_anonymous_begin
template<class DataType>
std::string getDescriptionHelper(DataType&& data) {
    std::string ret;
    switch (data.getType()) {
        case cocos2d::Value::Type::INTEGER: {
            ret += toString(data.asInt());
            break;
        }
        case cocos2d::Value::Type::STRING: {
            ret += data.asString();
            break;
        }
        case cocos2d::Value::Type::MAP: {
            std::map<std::string, cocos2d::Value> orderedMap(data.asValueMap().cbegin(), data.asValueMap().cend());
            for (auto&& pair : orderedMap) {
                if (pair.first != "hash") {
                    ret += pair.first + getDescriptionHelper(pair.second);
                }
            }
            break;
        }
        default: {
            CCASSERT(false, "Wrong type.");
        }
    }
    return ret;
}
namespace_anonymous_end

std::string getDescription(const cocos2d::ValueMap& data) {
    return getDescriptionHelper(cocos2d::Value(data));
}
namespace_ee_end