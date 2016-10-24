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
namespace detail {
template <class T, std::size_t Size = sizeof(T)> struct dynamic_value_storage;

template <class T> struct dynamic_value_storage<T, 4> {
    using value_type = T;
    using storage_type = std::uint32_t;
};

template <class T> struct dynamic_value_storage<T, 8> {
    using value_type = T;
    using storage_type = std::uint64_t;
};
} // namespace detail

/// Utility class to store a value that dynamically changes.
///
/// Useful for anticheating.
template <class T> class DynamicValue final {
public:
    ~DynamicValue() = default;

    DynamicValue(T value = T());

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

    DynamicValue& subtract(T amount);

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
    using storage_type =
        typename detail::dynamic_value_storage<T>::storage_type;

    std::unique_ptr<storage_type> value_;
    std::unique_ptr<storage_type> random_;
};
NS_EE_END

#endif /* EE_LIBRARY_DYNAMIC_VALUE_HPP_ */
