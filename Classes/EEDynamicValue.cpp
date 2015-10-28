//
//  EEDynamicValue.cpp
//  roll-eat
//
//  Created by enrevol on 9/2/15.
//
//

#include "EEDynamicValue.hpp"
#include "base/ccRandom.h"

#include <memory>

namespace_ee_begin
template<class T>
DynamicValue<T>::DynamicValue()
    : _value(new unsigned())
    , _random(new unsigned()) {}

template<class T>
DynamicValue<T>::~DynamicValue() {}
    
template<class T>
DynamicValue<T>::DynamicValue(T value) : DynamicValue() {
    set(value);
}
    
template<class T>
DynamicValue<T>::DynamicValue(const DynamicValue<T>& other) : DynamicValue() {
    set(other);
}
    
template<class T>
DynamicValue<T>::DynamicValue(DynamicValue<T>&& other)
    : _value(std::move(other._value))
    , _random(std::move(other._random)) {}
    
template<class T>
DynamicValue<T>& DynamicValue<T>::operator=(const DynamicValue& other) {
    set(other);
    return *this;
}
    
template<class T>
DynamicValue<T>& DynamicValue<T>::operator=(DynamicValue&& other) {
    _value = std::move(other._value);
    _random = std::move(other._random);
    return *this;
}
    
template<class T>
T DynamicValue<T>::get() const {
    unsigned intValue = (*_value) ^ (*_random);
    T ret = *reinterpret_cast<T*>(&intValue);
    return ret;
}

template<class T>
void DynamicValue<T>::set(T value) {
    _value.release();
    _random.release();
    const unsigned intValue = *reinterpret_cast<const unsigned*>(&value);
    _random = std::unique_ptr<unsigned>(new unsigned(cocos2d::random<unsigned>(0, 0xFFFFFFFF)));
    _value = std::unique_ptr<unsigned>(new unsigned((*_random) ^ intValue));
}

template<class T>
void DynamicValue<T>::add(T amount) {
    set(get() + amount);
}

// Implicit conversion.
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

// Post-increment operator.
template<class T>
DynamicValue<T> DynamicValue<T>::operator++(int) {
    auto ret = DynamicValue(get() + 1);
    return ret;
}

// Post-decrement operator.
template<class T>
DynamicValue<T> DynamicValue<T>::operator--(int) {
    auto ret = DynamicValue<T>(get() - 1);
    return ret;
}

// Pre-increment operator.
template<class T>
DynamicValue<T>& DynamicValue<T>::operator++() {
    add(+1);
    return *this;
}

// Pre-decrement operator.
template<class T>
DynamicValue<T>& DynamicValue<T>::operator--() {
    add(-1);
    return *this;
}

template class DynamicValue<int>;
template class DynamicValue<float>;
namespace_ee_end