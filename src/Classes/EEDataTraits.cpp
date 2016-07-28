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
cocos2d::Value DataTraits<bool>::set(bool value) {
    return cocos2d::Value{value};
}

bool DataTraits<bool>::get(const cocos2d::Value& value) {
    return value.asBool();
}

cocos2d::Value DataTraits<int>::set(int value) { return cocos2d::Value{value}; }

int DataTraits<int>::get(const cocos2d::Value& value) { return value.asInt(); }

cocos2d::Value DataTraits<float>::set(float value) {
    return cocos2d::Value{value};
}

float DataTraits<float>::get(const cocos2d::Value& value) {
    return value.asFloat();
}

cocos2d::Value DataTraits<std::string>::set(const std::string& value) {
    return cocos2d::Value{value};
}

std::string DataTraits<std::string>::get(const cocos2d::Value& value) {
    return value.asString();
}

template <class T>
cocos2d::Value
DataTraits<T, detail::enable_if_is_enum_and_inherit_from_int<T>>::set(T value) {
    return DataTraits<int>::set(static_cast<int>(value));
}

template <class T>
T DataTraits<T, detail::enable_if_is_enum_and_inherit_from_int<T>>::get(
    const cocos2d::Value& value) {
    return static_cast<T>(DataTraits<int>::get(value));
}
} // namespace ee