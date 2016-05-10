//
//  EEState.hpp
//  ee-library
//
//  Created by enrevol on 11/30/15.
//
//

#ifndef EE_LIBRARY_FLAGS_HPP_
#define EE_LIBRARY_FLAGS_HPP_

#include "EEMacro.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
template<class EnumType>
constexpr
std::enable_if_t<
    std::is_enum<EnumType>::value,
    std::underlying_type_t<EnumType>
>
convert_enum_to_underlying_type(EnumType value) noexcept {
    return static_cast<std::underlying_type_t<EnumType>>(value);
}

template<class T>
struct has_bitwise_operators : public std::false_type {};

template<class EnumType, class Type>
using enable_if_enum_has_bitwise_operators = std::enable_if_t<
    std::is_enum<EnumType>::value &&
    has_bitwise_operators<EnumType>::value,
    Type
>;

template<class EnumType, class BinOp>
constexpr EnumType apply_binary_operator(EnumType lhs, EnumType rhs,
                                         BinOp op) noexcept {
    return static_cast<EnumType>(op(convert_enum_to_underlying_type(lhs),
                                    convert_enum_to_underlying_type(rhs)));
}

template<class EnumType, class UnaryOp>
constexpr EnumType apply_unary_operator(EnumType value, UnaryOp op) noexcept {
    return static_cast<EnumType>(op(convert_enum_to_underlying_type(value)));
}

NS_DETAIL_END
NS_EE_END

#define EE_ENABLE_BITWISE_OPERATORS_FOR_ENUM(EnumType) \
    namespace ee::detail { \
    template<> \
    struct has_bitwise_operators<EnumType> : public std::true_type {}; \
    } // namespace ee::detail

/// Bitwise or operator.
template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator|(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_or<>());
}

template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator|=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs | rhs;
}

/// Bitwise and operator.
template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator&(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_and<>());
}

template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator&=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs & rhs;
}

/// Bitwise xor operator.
template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator^(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_xor<>());
}

template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator^=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs ^ rhs;
}

/// Bitwise not operator.
template<class EnumType>
constexpr
ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator~(EnumType value) noexcept {
    return ee::detail::apply_unary_operator(value, std::bit_not<>());
}

#endif /* EE_LIBRARY_FLAGS_HPP_ */
