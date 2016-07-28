//
//  EEDataTraits.hpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#ifndef EE_DATA_TRAITS_HPP_
#define EE_DATA_TRAITS_HPP_

#include <string>

#include "EECocos2dxFwd.hpp"
#include "EEForward.hpp"

namespace ee {
template <> struct DataTraits<bool> {
    static cocos2d::Value set(bool value);
    static bool get(const cocos2d::Value& value);
};

template <> struct DataTraits<int> {
    static cocos2d::Value set(int value);
    static int get(const cocos2d::Value& value);
};

template <> struct DataTraits<float> {
    static cocos2d::Value set(float value);
    static float get(const cocos2d::Value& value);
};

template <> struct DataTraits<std::string> {
    static cocos2d::Value set(const std::string& value);
    static std::string get(const cocos2d::Value& value);
};

namespace detail {
template <class T, class U = void>
using enable_if_is_enum_and_inherit_from_int =
    std::enable_if_t<std::is_enum<T>::value &&
                         std::is_same<std::underlying_type_t<T>, int>::value,
                     U>;
} // namespace detail

/// Specializes data traits for enum classes (must inherit from int).
template <class T>
struct DataTraits<T, detail::enable_if_is_enum_and_inherit_from_int<T>> {
    static cocos2d::Value set(T value);
    static T get(const cocos2d::Value& value);
};
} // namespace ee

#endif /* EE_DATA_TRAITS_HPP_ */
