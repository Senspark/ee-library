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

#include "EEForward.hpp"
#include "EEUtils.hpp"
#include "EEMeta.hpp"
#include "EEDataTraits.hpp"

namespace ee {
namespace detail {
/// Checks whether the specified type is a DataInfo.
template <class T> struct is_data_info : std::false_type {};

template <std::size_t DataId, class Value, class... Keys>
struct is_data_info<DataInfo<DataId, Value, Keys...>> : std::true_type {};

template <class T> constexpr bool is_data_info_v = is_data_info<T>::value;

/// Checks whether it is possible to convert/construct from Args to Keys.
template <class Formatter, class... Args> struct is_formattable;

template <class... Keys, class... Args>
struct is_formattable<DataFormat<Keys...>, Args...> {
    static constexpr bool value = ((std::is_convertible<Args, Keys>::value ||
                                    std::is_constructible<Keys, Args>::value) &&
                                   ...);
};

template <class... Keys, class... Args>
struct is_formattable<DataFormat<std::tuple<Keys...>>, Args...>
    : is_formattable<DataFormat<Keys...>, Args...> {};

template <class Formatter, class... Args>
constexpr bool is_formattable_v = is_formattable<Formatter, Args...>::value;

/// Gets the last type in a parameter pack.
template <class... Args> struct last {
    using type =
        std::tuple_element_t<(sizeof...(Args)) - 1, std::tuple<Args...>>;
};

template <class... Args> using last_t = typename last<Args...>::type;

/// Gets all types except the last type in a parameter pack.
template <class... Args> struct exclude_last;

template <std::size_t... Is, class... Ts>
struct exclude_last<std::index_sequence<Is...>, Ts...> {
    using type =
        std::tuple<typename std::tuple_element<Is, std::tuple<Ts...>>::type...>;
};

template <class... Args>
using exclude_last_t =
    typename exclude_last<std::make_index_sequence<(sizeof...(Args)) - 1>,
                          Args...>::type;

template <class Value, class... Args> struct get_traits {
    using type = std::conditional_t<
        sizeof...(Args) == 0, DataTraits<Value>,
        std::conditional_t<is_traits_v<last_t<Args...>, Value>, last_t<Args...>,
                           DataTraits<Value>>>;
};

template <class Value, class... Args>
using get_traits_t = typename get_traits<Value, Args...>::type;

template <class Value, class... Args> struct get_format {
    using type = std::conditional_t<
        sizeof...(Args) == 0, DataFormat<>,
        std::conditional_t<is_traits_v<last_t<Args...>, Value>,
                           DataFormat<exclude_last_t<Args...>>,
                           DataFormat<Args...>>>;
};

template <class Value, class... Args>
using get_format_t = typename get_format<Value, Args...>::type;

template <class T, EE_REQUIRES(not std::is_enum<T>::value)>
const T& convert_enum_to_integer(const T& arg) {
    return arg;
}

template <class T, EE_REQUIRES(std::is_enum<T>::value)>
std::underlying_type_t<T> convert_enum_to_integer(const T& arg) {
    return static_cast<std::underlying_type_t<T>>(arg);
}
} // namespace detail

/// @param Keys The format types.
template <class... Keys> struct DataFormat {
public:
    using KeyTypes = std::tuple<Keys...>;

    static std::string createKey(const Keys&... keys) {
        return toString(
            toString("___", detail::convert_enum_to_integer(keys))...);
    }
};

template <class... Keys>
struct DataFormat<std::tuple<Keys...>> : DataFormat<Keys...> {};

/// Use nil format.
/// Use default traits.
template <std::size_t DataId, class Value>
struct DataInfo<DataId, Value>
    : DataInfo<DataId, Value, DataFormat<>, DataTraits<Value>, void> {};

/// Use custom format.
/// Use default traits.
template <std::size_t DataId, class Value, class... Keys>
struct DataInfo<DataId, Value, DataFormat<Keys...>>
    : DataInfo<DataId, Value, DataFormat<Keys...>, DataTraits<Value>, void> {};

/// Use custom traits.
template <std::size_t DataId, class Value, class Traits>
struct DataInfo<DataId, Value, Traits,
                std::enable_if_t<detail::is_traits_v<Traits, Value>>>
    : DataInfo<DataId, Value, DataFormat<>, Traits, void> {};

/// Backward compability.
template <std::size_t DataId, class Value, class... Args>
struct DataInfo : DataInfo<DataId, Value, detail::get_format_t<Value, Args...>,
                           detail::get_traits_t<Value, Args...>, void> {};

/// Use custom format.
/// Use custom traits.
template <std::size_t DataId, class Value, class... Keys, class Traits>
struct DataInfo<DataId, Value, DataFormat<Keys...>, Traits,
                std::enable_if_t<detail::is_traits_v<Traits, Value>>>
    : DataFormat<Keys...>, Traits {
public:
    static_assert(detail::is_traits_v<Traits, Value>,
                  "Traits type is not a valid data traits!");

    static_assert(not std::is_reference<Value>::value,
                  "Value type must not be a reference type!");

    static_assert(std::is_same<std::decay_t<Value>, Value>::value,
                  "Value type must be a decayed type!");

    using ValueType = Value;
    using FormatType = DataFormat<Keys...>;
    using TraitsType = Traits;

    using SetterType = std::function<void(ValueType& value)>;

    enum { Id = DataId };

    using FormatType::createKey;
    using TraitsType::store;
    using TraitsType::load;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_INFO_HPP_ */
