//
//  EEExtension.hpp
//  tienlen
//
//  Created by enrevol on 3/22/16.
//
//

#ifndef EE_LIBRARY_EXTENSION_HPP_
#define EE_LIBRARY_EXTENSION_HPP_

/// C++17 functions.

#include <functional>
#include <type_traits>

#include "EEMacro.hpp"

NS_EE_BEGIN
template<class T>
constexpr bool is_void_v = std::is_void<T>::value;

template<class T>
constexpr bool is_null_pointer_v = std::is_null_pointer<T>::value;

template<class T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<class T>
constexpr bool is_array_v = std::is_array<T>::value;

template<class T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;

template<class T>
constexpr bool is_lvalue_reference_v = std::is_lvalue_reference<T>::value;

template<class T>
constexpr bool is_rvalue_reference_v = std::is_rvalue_reference<T>::value;

template<class T>
constexpr bool is_member_object_pointer_v = std::is_member_object_pointer<T>::value;

template<class T>
constexpr bool is_member_function_pointer_v = std::is_member_function_pointer<T>::value;

template<class T>
constexpr bool is_enum_v = std::is_enum<T>::value;

template<class T>
constexpr bool is_union_v = std::is_union<T>::value;

template<class T>
constexpr bool is_class_v = std::is_class<T>::value;

template<class T>
constexpr bool is_function_v = std::is_function<T>::value;

template<class T>
constexpr bool is_reference_v = std::is_reference<T>::value;

template<class T>
constexpr bool is_member_pointer_v = std::is_member_pointer<T>::value;

template<class T>
constexpr bool is_const_v = std::is_const<T>::value;

template<class T>
constexpr bool is_pod_v = std::is_pod<T>::value;
    
template<class T, class U>
constexpr bool is_same_v = std::is_same<T, U>::value;
    
template<class T, class U>
constexpr bool is_base_of_v = std::is_base_of<T, U>::value;
    
template<class From, class To>
constexpr bool is_convertible_v = std::is_convertible<From, To>::value;

template<class T>
constexpr bool is_bind_expression_v = std::is_bind_expression<T>::value;

template<class T>
constexpr int is_placeholder_v = std::is_placeholder<T>::value;

/// is_reference_wrapper.
template<class T>
struct is_reference_wrapper {
    static constexpr bool value = false;
};

template<class T>
struct is_reference_wrapper<std::reference_wrapper<T>> {
    static constexpr bool value = true;
};

template<class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

template<class T>
constexpr std::size_t tuple_size_v = std::tuple_size<T>::value;

template<class T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
    return t;
}

template<class...> struct conjunction : std::true_type {};
template<class B> struct conjunction<B> : B {};
template<class B, class... Bs>
struct conjunction<B, Bs...> : std::conditional_t<B::value != false, conjunction<Bs...>, B> {};
    
template<class... Bs>
constexpr bool conjunction_v = conjunction<Bs...>::value;

template<class...> struct disjunction : std::false_type {};
template<class B> struct disjunction<B> : B {};
template<class B, class... Bs>
struct disjunction<B, Bs...> : std::conditional_t<B::value != false, B, disjunction<Bs...>> {};
    
template<class... Bs>
constexpr bool disjunction_v = disjunction<Bs...>::value;

template<bool B>
using bool_constant = std::integral_constant<bool, B>;

template<class B>
struct negation : bool_constant<!B::value> {};
    
template<class B>
constexpr bool negation_v = negation<B>::value;
NS_EE_END

#endif /* EE_LIBRARY_EXTENSION_HPP_ */
