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

namespace ee {
template <class T> struct DataTraits;

template <> struct DataTraits<bool> {
    static cocos2d::Value save(bool value);
    static bool load(const cocos2d::Value& value);
};

template <> struct DataTraits<int> {
    static cocos2d::Value save(int value);
    static int load(const cocos2d::Value& value);
};

template <> struct DataTraits<float> {
    static cocos2d::Value save(float value);
    static float load(const cocos2d::Value& value);
};

template <> struct DataTraits<std::string> {
    static cocos2d::Value save(const std::string& value);
    static std::string load(const cocos2d::Value& value);
};
} // namespace ee

#endif /* EE_DATA_TRAITS_HPP_ */
