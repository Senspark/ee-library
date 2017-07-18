//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP_
#define EE_LIBRARY_DATA_INFO_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "EEUtils.hpp"

namespace ee {
namespace detail {
class DataInfoBase {};

template <class T, class = std::enable_if_t<not std::is_enum<T>::value>>
const T& convert_enum_to_integer(const T& arg) {
    return arg;
}

template <class T, class = std::enable_if_t<std::is_enum<T>::value>>
std::underlying_type_t<T> convert_enum_to_integer(const T& arg) {
    return static_cast<std::underlying_type_t<T>>(arg);
}
} // namespace detail

/// @param DataId The data unique ID.
/// @param Value The returned value type.
/// @param Keys The format types.
template <std::size_t DataId, class Value, class... Keys>
class DataInfo final : public detail::DataInfoBase {
public:
    static_assert(not std::is_reference<Value>::value,
                  "Value type must not be a reference type!");

    static_assert(std::is_same<std::decay_t<Value>, Value>::value,
                  "Value type must be a decayed type!");

    using ValueType = Value;
    using KeyTypes = std::tuple<Keys...>;

    using SetterType = std::function<void(ValueType& value)>;

    enum { Id = DataId };

    static std::string createKey(const Keys&... keys) {
        return toString(
            toString("___", detail::convert_enum_to_integer(keys))...);
    }
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_INFO_HPP_ */
