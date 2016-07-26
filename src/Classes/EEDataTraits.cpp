//
//  EEDataTraits.cpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#include "EEDataTraits.hpp"

#include <base/CCValue.h>

namespace ee {
cocos2d::Value DataTraits<bool>::save(bool value) {
    return cocos2d::Value{value};
}

bool DataTraits<bool>::load(const cocos2d::Value& value) {
    return value.asBool();
}

cocos2d::Value DataTraits<int>::save(int value) {
    return cocos2d::Value{value};
}

int DataTraits<int>::load(const cocos2d::Value& value) { return value.asInt(); }

cocos2d::Value DataTraits<float>::save(float value) {
    return cocos2d::Value{value};
}

float DataTraits<float>::load(const cocos2d::Value& value) {
    return value.asFloat();
}

cocos2d::Value DataTraits<std::string>::save(const std::string& value) {
    return cocos2d::Value{value};
}

std::string DataTraits<std::string>::load(const cocos2d::Value& value) {
    return value.asString();
}
} // namespace ee