//
//  DynamicValue.h
//  roll-eat
//
//  Created by Hoang Le Hai on 7/8/15.
//
//

#ifndef EE_LIBRARY_DYNAMIC_VALUE_HPP_
#define EE_LIBRARY_DYNAMIC_VALUE_HPP_

#include <cstdint>
#include <memory>

#include "EEMacro.hpp"

NS_EE_BEGIN
/// Utility class to store a value that dynamically changes.
///
/// Useful for anticheating.
///
/// Currently supports @c std::int32_t and @c float.
template <class T> class DynamicValue final {
public:
    DynamicValue();
    ~DynamicValue() = default;

    DynamicValue(T value);

    DynamicValue(const DynamicValue& other);
    DynamicValue(DynamicValue&& other) = default;

    DynamicValue& operator=(const DynamicValue& other);
    DynamicValue& operator=(DynamicValue&& other) = default;

    /// Retrieves the value.
    T get() const;

    /// Assigns value.
    DynamicValue& set(T value);

    /// Add a value.
    DynamicValue& add(T amount);

    /// Implicit conversion.
    operator T() const;

    DynamicValue& operator=(T value);
    DynamicValue& operator+=(T value);
    DynamicValue& operator-=(T value);

    /// Post-increment operator.
    DynamicValue operator++(int);

    /// Post-decrement operator.
    DynamicValue operator--(int);

    /// Pre-increment operator.
    DynamicValue& operator++();

    /// Pre-decrement operator.
    DynamicValue& operator--();

private:
    using StoreType = std::uint32_t;

    std::unique_ptr<StoreType> value_;
    std::unique_ptr<StoreType> random_;
};
NS_EE_END

#endif /* EE_LIBRARY_DYNAMIC_VALUE_HPP_ */
