//
//  DynamicValue.h
//  roll-eat
//
//  Created by Hoang Le Hai on 7/8/15.
//
//

#ifndef EE_LIBRARY_DYNAMIC_VALUE_H
#define EE_LIBRARY_DYNAMIC_VALUE_H

#include "EEMacro.hpp"

#include <memory>

namespace_ee_begin
template<class T>
class DynamicValue {
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
    void set(T value);
    void add(T amount);

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
    std::unique_ptr<unsigned> _value;
    std::unique_ptr<unsigned> _random;
};
namespace_ee_end

#endif /* EE_LIBRARY_DYNAMIC_VALUE_H */
