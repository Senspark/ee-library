//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by enrevol on 11/7/15.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP
#define EE_LIBRARY_DATA_INFO_HPP

#include <string>
#include <type_traits>

#include "EEMacro.hpp"
#include "EEForward.hpp"
#include "EEExtension.hpp"
#include "EEUtils.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
template<class T>
void setInfoHelper(DataHandler* handler, const std::string& key, std::conditional_t<is_pod_v<T>, T, std::add_const_t<std::add_lvalue_reference_t<T>>> value);

template<class T>
T getInfoHelper(DataHandler* handler, const std::string& key, std::conditional_t<is_pod_v<T>, T, std::add_const_t<std::add_lvalue_reference_t<T>>> value);

template<class... Args>
std::string createKey(Args&&... args) {
    return toString(toString("___", std::forward<Args>(args))...);
}

class DataInfoBase_ {
public:
    /// Permits copy constructors.
    DataInfoBase_(const DataInfoBase_&) = delete;
    DataInfoBase_& operator=(const DataInfoBase_&) = delete;
    
    DataInfoBase_(DataInfoBase_&&) = default;
    DataInfoBase_& operator=(DataInfoBase_&&) = delete;
    
    /// Retrieves the data key.
    const std::string& getKey() const {
        return _key;
    }
    
protected:
    explicit DataInfoBase_(std::string key)
    : _key(std::move(key))
    {}
    
    const std::string _key;
};

template<class T>
class DataInfoBase : public DataInfoBase_ {
public:
    using ValueType = std::decay_t<T>;
    
    /**
     * Constructor.
     * @param key Unique key to store in user defaults.
     * @param defaultValue The default value in user defaults.
     */
    explicit DataInfoBase(std::string key, ValueType defaultValue = ValueType(), DataHandler* handler = nullptr)
    : DataInfoBase_(std::move(key))
    , _defaultValue(std::move(defaultValue))
    , _handler(handler)
    {}
    
protected:
    DataHandler* _handler;
    
    const ValueType _defaultValue;
};
NS_DETAIL_END

template<class... Args>
class DataInfo;

// Zero format argument.
template<class T>
class DataInfo<T> final : public detail::DataInfoBase<T> {
public:
    using typename detail::DataInfoBase<T>::ValueType;
    
    using FormatType = std::tuple<>;
    
    using detail::DataInfoBase<T>::DataInfoBase;
    
    /**
     * Setter.
     * @param value The desired value to be set.
     */
    const DataInfo<T>& set(T value) const {
        detail::setInfoHelper<ValueType>(_handler, _key, std::forward<T>(value));
        return *this;
    }
    
    /**
     * Getter.
     * @return Retrieved value.
     */
    ValueType get() const {
        return detail::getInfoHelper<ValueType>(_handler, _key, _defaultValue);
    }
    
    /**
     * Combination of getter and setter.
     */
    const DataInfo<T>& add(T value) const {
        return set(get() + std::forward<T>(value));
    }
    
    /**
     * Implicit conversion.
     */
    operator ValueType() const {
        return get();
    }
    
    const DataInfo<T>& operator=(T value) const {
        return set(std::forward<T>(value));
    }
    
    const DataInfo<T>& operator+=(T value) const {
        return add(std::forward<T>(value));
    }
    
    const DataInfo<T>& operator-=(T value) const {
        return add(-value);
    }
    
    ValueType operator++(int) const {
        auto result = get();
        set(result + 1);
        return result;
    }
    
    ValueType operator--(int) const {
        auto result = get();
        set(result - 1);
        return result;
    }
    
    const DataInfo<T>& operator++() const {
        return add(+1);
    }
    
    const DataInfo<T>& operator--() const {
        return add(-1);
    }
    
private:
    using detail::DataInfoBase<T>::_key;
    using detail::DataInfoBase<T>::_defaultValue;
    using detail::DataInfoBase<T>::_handler;
};

NS_DETAIL_BEGIN
template<class... Args>
class DataInfoHolder;

template<class T, class U>
class DataInfoHolder<T, U> : public DataInfoBase<T> {
public:
    using typename DataInfoBase<T>::ValueType;
    
    using FormatType = std::tuple<U>;
    using FirstFormatType = U;
    
    using DataInfoBase<T>::DataInfoBase;
    
    DataInfo<T> at(U arg) const {
        return DataInfo<T>(toString(_key, detail::createKey(std::forward<U>(arg))), _defaultValue);
    }
    
    DataInfo<T> operator[](U arg) const {
        return at(std::forward<U>(arg));
    }
    
protected:
    using DataInfoBase<T>::_key;
    using DataInfoBase<T>::_defaultValue;
};

template<class T, class U, class... Args>
class DataInfoHolder<T, U, Args...> : public DataInfoBase<T> {
public:
    using typename DataInfoBase<T>::ValueType;
    
    using FormatType = std::tuple<U, Args...>;
    using FirstFormatType = U;
    
    using DataInfoBase<T>::DataInfoBase;
    
    DataInfo<T> at(U arg, Args... args) const {
        return DataInfo<T>(toString(_key, detail::createKey(std::forward<U>(arg), std::forward<Args>(args)...)), _defaultValue);
    }
    
    DataInfoHolder<T, Args...> at(U arg) const {
        return DataInfoHolder<T, Args...>(toString(_key, detail::createKey(std::forward<U>(arg))), _defaultValue);
    }
    
    DataInfoHolder<T, Args...> operator[](U arg) const {
        return at(std::forward<U>(arg));
    }
    
protected:
    using DataInfoBase<T>::_key;
    using DataInfoBase<T>::_defaultValue;
};
NS_DETAIL_END

/// Multiple format arguments.
template<class T, class... Args>
class DataInfo<T, Args...> final : public detail::DataInfoHolder<T, Args...> {
public:
    using typename detail::DataInfoHolder<T, Args...>::ValueType;
    using typename detail::DataInfoHolder<T, Args...>::FormatType;
    
    using detail::DataInfoHolder<T, Args...>::DataInfoHolder;
    
    /**
     * Setter.
     * @param value The desired value to be set.
     */
    const DataInfo<T>& set(T value) const {
        _handler->template setWithKey<ValueType>(_key, std::forward<T>(value));
        return *this;
    }
    
    const DataInfo<T, Args...>& set(T value, Args... args) const {
        _handler->template setWithKey<ValueType>(toString(_key, detail::createKey(std::forward<Args>(args)...)), std::forward<T>(value));
        return *this;
    }
    
    ValueType get(Args... args) const {
        return _handler->template getWithKey<ValueType>(toString(_key, detail::createKey(std::forward<Args>(args)...)), _defaultValue);
    }
    
    const DataInfo<T, Args...>& add(T value, Args... args) const {
        auto current = get(args...);
        return set(std::move(current) + std::forward<T>(value), std::forward<Args>(args)...);
    }
    
private:
    using detail::DataInfoHolder<T, Args...>::_key;
    using detail::DataInfoHolder<T, Args...>::_defaultValue;
    using detail::DataInfoHolder<T, Args...>::_handler;
};

#define CREATE_DATA_INFO(...)                                   GET_FUNCTION(CREATE_DATA_INFO_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define CREATE_DATA_INFO_1(variableName)                        variableName(# variableName)
#define CREATE_DATA_INFO_2(variableName, defaultValue)          variableName(# variableName, defaultValue)
#define CREATE_DATA_INFO_3(variableName, defaultValue, handler) variableName(# variableName, defaultValue, handler)
#define CREATE_DATA_INFO_EX(...)                                decltype(GET_FIRST_ARG(__VA_ARGS__)) CREATE_DATA_INFO(__VA_ARGS__)
NS_EE_END

#endif /* EE_LIBRARY_DATA_INFO_HPP */
