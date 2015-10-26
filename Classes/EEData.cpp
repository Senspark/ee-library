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

#pragma mark - SET INFO HELPER

template<> void setInfoHelper(const char* key, const bool& value) {
    cocos2d::UserDefault::getInstance()->setBoolForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const int& value) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const std::string& value) {
    cocos2d::UserDefault::getInstance()->setStringForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

template<> void setInfoHelper(const char* key, const float& value) {
    cocos2d::UserDefault::getInstance()->setFloatForKey(key, value);
    cocos2d::UserDefault::getInstance()->flush();
}

#pragma mark - GET INFO HELPER

template<> bool getInfoHelper(const char* key, const bool& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getBoolForKey(key, defaultValue);
}

template<> int getInfoHelper(const char* key, const int& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key, defaultValue);
}

template<> std::string getInfoHelper(const char* key, const std::string& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getStringForKey(key, defaultValue);
}

template<> float getInfoHelper(const char* key, const float& defaultValue) {
    return cocos2d::UserDefault::getInstance()->getFloatForKey(key, defaultValue);
}
namespace_ee_end