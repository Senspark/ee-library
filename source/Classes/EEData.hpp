#ifndef EE_LIBRARY_DATA_HPP
#define EE_LIBRARY_DATA_HPP

#include "EEMacro.hpp"
#include "EEFunctionTraits.hpp"
#include "EEUtils.hpp"
#include "EENonCopyable.hpp"

#include "base/CCValue.h"
#include "deprecated/CCString.h"

#include <string>

namespace_ee_begin
/**
 * Gets the unique description string for the specified value map.
 * @param data is the input.
 * @return The description string.
 */
std::string getDescription(const cocos2d::ValueMap& data);

template<class ValueType>
struct GetReturnType {
    /// All available value types in cocos2d::Value.
    using ValueTypes = std::tuple<int, float, bool, std::string, cocos2d::ValueMap>;
    /// All corresponding return types in cocos2d::Value.
    using ReturnTypes = std::tuple<int, float, bool, std::string, const cocos2d::ValueMap&>;
    /// Index of ValueType in ValueTypes.
    enum { Index = GetTupleIndex<ValueType, ValueTypes>::Index };
    /// Corresponding return type to ValueType.
    using Type = typename std::tuple_element<Index, ReturnTypes>::type;
};

/// Helper functions to deduce return type.
template<class ValueType>
typename GetReturnType<ValueType>::Type getReturnValue(const cocos2d::Value& data) {
    using ValueMapFunctionType = const cocos2d::ValueMap&(cocos2d::Value::*)() const;
    const auto Functions = std::make_tuple(&cocos2d::Value::asInt,
                                           &cocos2d::Value::asFloat,
                                           &cocos2d::Value::asBool,
                                           &cocos2d::Value::asString,
                                           static_cast<ValueMapFunctionType>(&cocos2d::Value::asValueMap));
    auto function = std::get<GetReturnType<ValueType>::Index>(Functions);
    return (data.*function)();
}
    
#pragma mark - GET VALUE

/// Retrieves a specified return type value in the specified value map with given keys.
template<class ValueType, class KeyType>
ValueType getValue(const cocos2d::ValueMap& data, KeyType&& key) {
    cocos2d::Value ret((ValueType()));
    auto iter = data.find(key);
    if (iter != data.end()) {
        ret = iter->second;
    }
    return getReturnValue<ValueType>(ret);
}

/// Overload for pod type with default value.
template<class ValueType, ValueType defaultValue, typename std::enable_if<std::is_pod<ValueType>::value, int>::type = 0, class KeyType>
ValueType getValue(const cocos2d::ValueMap& data, KeyType&& key) {
    cocos2d::Value ret(defaultValue);
    auto iter = data.find(key);
    if (iter != data.end()) {
        ret = iter->second;
    }
    return getReturnValue<ValueType>(ret);
}

/// Retrieves a specified return type value in the specified value map with given keys.
template<class ValueType, class KeyType, class... Args>
ValueType getValue(const cocos2d::ValueMap& data, KeyType&& key, Args&&... args) {
    cocos2d::Value ret((ValueType()));
    auto iter = data.find(key);
    if (iter != data.end()) {
        ret = getValue<ValueType>(iter->second.asValueMap(), std::forward<Args>(args)...);
    }
    return getReturnValue<ValueType>(ret);
}

/// Overload for pod type with default value.
template<class ValueType, ValueType defaultValue, typename std::enable_if<std::is_pod<ValueType>::value, int>::type = 0, class KeyType, class... Args>
ValueType getValue(const cocos2d::ValueMap& data, KeyType&& key, Args&&... args) {
    cocos2d::Value ret(defaultValue);
    auto iter = data.find(key);
    if (iter != data.end()) {
        ret = getValue<ValueType, defaultValue>(iter->second.asValueMap(), std::forward<Args>(args)...);
    }
    return getReturnValue<ValueType>(ret);
}
    
#pragma mark - SET VALUE

/// Sets a value in the specified value map with given keys.
template<class ValueType, class KeyType>
void setValue(cocos2d::ValueMap& data, ValueType&& value, KeyType&& key) {
    data[key] = cocos2d::Value(value);
}

/// Sets a value in the specified value map with given keys.
template<class ValueType, class KeyType, class... Args>
void setValue(cocos2d::ValueMap& data, ValueType&& value, KeyType&& key, Args&&... args) {
    auto iter = data.find(key);
    if (iter == data.end()) {
        iter = data.emplace(key, cocos2d::Value(cocos2d::ValueMap())).first;
    }
    setValue(iter->second.asValueMap(), std::forward<ValueType>(value), std::forward<Args>(args)...);
}

namespace_ee_end

#endif // EE_LIBRARY_DATA_HPP