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
template<class T>
DynamicValue<T>::DynamicValue()
: value_(new StoreType())
, random_(new StoreType())
{}

template<class T>
DynamicValue<T>::DynamicValue(T value)
: DynamicValue() {
    set(value);
}
    
template<class T>
DynamicValue<T>::DynamicValue(const DynamicValue<T>& other)
: DynamicValue() {
    set(other);
}
    
template<class T>
DynamicValue<T>& DynamicValue<T>::operator=(const DynamicValue& other) {
    set(other);
    return *this;
}
    
template<class T>
T DynamicValue<T>::get() const {
    auto intValue = (*value_) ^ (*random_);
    auto ret = bit_cast<T>(intValue);
    return ret;
}

template<class T>
DynamicValue<T>& DynamicValue<T>::set(T value) {
    auto intValue = bit_cast<StoreType>(value);
    random_ = std::make_unique<StoreType>(cocos2d::random(std::numeric_limits<StoreType>::min(),
                                                          std::numeric_limits<StoreType>::max()));
    value_ = std::make_unique<std::uint32_t>((*random_) ^ intValue);
    return *this;
}

template<class T>
DynamicValue<T>& DynamicValue<T>::add(T amount) {
    return set(get() + amount);
}

template<class T>
DynamicValue<T>::operator T() const {
    return get();
}

template<class T>
DynamicValue<T>& DynamicValue<T>::operator=(T value) {
    set(value);
    return *this;
}
    
template<class T>
DynamicValue<T>& DynamicValue<T>::operator+=(T value) {
    add(value);
    return *this;
}
    
template<class T>
DynamicValue<T>& DynamicValue<T>::operator-=(T value) {
    add(-value);
    return *this;
}

template<class T>
DynamicValue<T> DynamicValue<T>::operator++(int) {
    auto result = *this;
    add(+1);
    return result;
}

template<class T>
DynamicValue<T> DynamicValue<T>::operator--(int) {
    auto result = *this;
    add(-1);
    return result;
}

template<class T>
DynamicValue<T>& DynamicValue<T>::operator++() {
    add(+1);
    return *this;
}

template<class T>
DynamicValue<T>& DynamicValue<T>::operator--() {
    add(-1);
    return *this;
}

template class DynamicValue<std::int32_t>;
template class DynamicValue<float>;
NS_EE_END