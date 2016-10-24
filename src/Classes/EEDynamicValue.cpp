//
//  EEDynamicValue.cpp
//  roll-eat
//
//  Created by enrevol on 9/2/15.
//
//

#include "EEDynamicValue.hpp"
#include "EEUtils.hpp"

#include <base/ccRandom.h>

NS_EE_BEGIN
template <class T> DynamicValue<T>::DynamicValue(T value) { set(value); }

template <class T>
DynamicValue<T>::DynamicValue(const DynamicValue<T>& other)
    : DynamicValue() {
    set(other);
}

template <class T>
DynamicValue<T>& DynamicValue<T>::operator=(const DynamicValue& other) {
    set(other);
    return *this;
}

template <class T> T DynamicValue<T>::get() const {
    auto intValue = (*value_) ^ (*random_);
    auto ret = bit_cast<T>(intValue);
    return ret;
}

template <class T> DynamicValue<T>& DynamicValue<T>::set(T value) {
    auto intValue = bit_cast<storage_type>(value);
    random_ = std::make_unique<storage_type>(
        cocos2d::random(std::numeric_limits<storage_type>::min(),
                        std::numeric_limits<storage_type>::max()));
    value_ = std::make_unique<storage_type>((*random_) ^ intValue);
    return *this;
}

template <class T> DynamicValue<T>& DynamicValue<T>::add(T amount) {
    return set(get() + amount);
}

template <class T> DynamicValue<T>& DynamicValue<T>::subtract(T amount) {
    return set(get() - amount);
}

template <class T> DynamicValue<T>::operator T() const { return get(); }

template <class T> DynamicValue<T>& DynamicValue<T>::operator=(T value) {
    return set(value);
}

template <class T> DynamicValue<T>& DynamicValue<T>::operator+=(T value) {
    return add(value);
}

template <class T> DynamicValue<T>& DynamicValue<T>::operator-=(T value) {
    return subtract(value);
}

template <class T> DynamicValue<T> DynamicValue<T>::operator++(int) {
    auto result = *this;
    add(1);
    return result;
}

template <class T> DynamicValue<T> DynamicValue<T>::operator--(int) {
    auto result = *this;
    subtract(1);
    return result;
}

template <class T> DynamicValue<T>& DynamicValue<T>::operator++() {
    return add(1);
}

template <class T> DynamicValue<T>& DynamicValue<T>::operator--() {
    return subtract(1);
}

template class DynamicValue<std::int32_t>;
template class DynamicValue<std::int64_t>;
template class DynamicValue<std::uint32_t>;
template class DynamicValue<std::uint64_t>;
template class DynamicValue<float>;
template class DynamicValue<double>;
NS_EE_END
