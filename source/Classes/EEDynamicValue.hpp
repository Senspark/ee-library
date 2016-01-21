//
//  DynamicValue.h
//  roll-eat
//
//  Created by Hoang Le Hai on 7/8/15.
//
//

#ifndef EE_LIBRARY_DYNAMIC_VALUE_HPP_
#define EE_LIBRARY_DYNAMIC_VALUE_HPP_

#include "EEMacro.hpp"

#include <cstdint>
#include <memory>

namespace_ee_begin
template<class T>
class DynamicValue final {
public:
    DynamicValue();
    ~DynamicValue();
    
    DynamicValue(T value);
    DynamicValue(const DynamicValue& other);
    DynamicValue(DynamicValue&& other);
    
    DynamicValue& operator=(const DynamicValue& other);
    DynamicValue& operator=(DynamicValue&& other);

    // Getter, setter.
    T get() const;
    DynamicValue& set(T value);
    DynamicValue& add(T amount);

    // Implicit conversion.
    operator T() const;
    
    DynamicValue& operator=(T value);
    DynamicValue& operator+=(T value);
    DynamicValue& operator-=(T value);

    // Post-increment operator.
    DynamicValue operator++(int);
    // Post-decrement operator.
    DynamicValue operator--(int);

    // Pre-increment operator.
    DynamicValue& operator++();
    // Pre-decrement operator.
    DynamicValue& operator--();

private:
    std::unique_ptr<std::uint32_t> _value;
    std::unique_ptr<std::uint32_t> _random;
};
namespace_ee_end

#endif /* EE_LIBRARY_DYNAMIC_VALUE_HPP_ */
