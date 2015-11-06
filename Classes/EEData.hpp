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

// Single argument.
template<class T>
std::string createKey(T&& value) {
    return toString("___", std::forward<T>(value));
};
    
// Multiple arguments.
template<class T, class... Args>
std::string createKey(T&& value, Args&&... args) {
    return toString("___", std::forward<T>(value), createKey(std::forward<Args>(args)...));
}
    
template<class T>
void setInfoHelper(const char* key, const T& value);
    
template<class T>
T getInfoHelper(const char* key, const T& defaultValue);

class DataInfoBase : public NonCopyable {
public:
    const std::string& getKey() const {
        return _key;
    }
    
protected:
    DataInfoBase(std::string key) : _key(std::move(key)) {}
    
    DataInfoBase(DataInfoBase&&) = default;
    DataInfoBase& operator=(DataInfoBase&&) = delete;
    
    std::string _key;
};

template<class... Args>
class DataInfo;

// Zero format argument.
template<class T>
class DataInfo<T> : public DataInfoBase {
public:
    using ValueType = typename std::decay<T>::type;
    using FormatArgs = std::tuple<>;
    
    explicit DataInfo(std::string key, T otherValue = T())
        : DataInfoBase(std::move(key))
        , _defaultValue(std::move(otherValue)) {}

    void set(T value) const {
        setInfoHelper<ValueType>(_key.c_str(), std::forward<T>(value));
    }
    
    ValueType get() const {
        return getInfoHelper<ValueType>(_key.c_str(), _defaultValue);
    }
    
    const DataInfo<T>& add(T value) const {
        set(get() + std::forward<T>(value));
        return *this;
    }
    
    operator ValueType() const {
        return get();
    }
    
    const DataInfo<T>& operator=(T value) const {
        set(std::forward<T>(value));
        return *this;
    }
    
    const DataInfo<T>& operator+=(T value) const {
        add(std::forward<T>(value));
        return *this;
    }

    const DataInfo<T>& operator-=(T value) const {
        add(-value);
        return *this;
    }
    
    ValueType operator++(int) const {
        auto ret = get();
        set(ret + 1);
        return ret;
    }
    
    ValueType operator--(int) const {
        auto ret = get();
        set(ret - 1);
        return ret;
    }
    
    const DataInfo<T>& operator++() const {
        add(+1);
        return *this;
    }
    
    const DataInfo<T>& operator--() const {
        add(-1);
        return *this;
    }
    
private:
    const T _defaultValue;
};

// Multiple format arguments.
template<class T, class... Args>
class DataInfo<T, Args...> : public DataInfoBase {
public:
    using ValueType = typename std::decay<T>::type;
    using FormatArgs = std::tuple<Args...>;
    
    explicit DataInfo(std::string key, T otherValue = T())
        : DataInfoBase(std::move(key))
        , _defaultValue(std::move(otherValue)) {}
    
    void set(T value, Args... args) const {
        setInfoHelper<T>(toString(_key, createKey(std::forward<Args>(args)...)).c_str(), std::forward<T>(value));
    }
    
    ValueType get(Args... args) const {
        return getInfoHelper<ValueType>(toString(_key, createKey(std::forward<Args>(args)...)).c_str(), _defaultValue);
    }
    
    void add(T value, Args... args) const {
        auto&& current = get(args...);
        set(current + std::forward<T>(value), std::forward<Args>(args)...);
    }
    
    DataInfo<T> at(Args... args) const {
        return DataInfo<T>(toString(_key, createKey(std::forward<Args>(args)...)).c_str(), _defaultValue);
    }
    
private:
    const T _defaultValue;
};
namespace_ee_end

#endif // EE_LIBRARY_DATA_HPP