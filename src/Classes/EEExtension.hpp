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

#include <cstring>
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
    
/// invoke c++17.
NS_DETAIL_BEGIN
template<class Base, class T, class Derived, class... Args>
auto invoke_impl(T Base::*pmf, Derived&& ref, Args&&... args)
    noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<is_function_v<T> &&
                        is_base_of_v<Base, std::decay_t<Derived>>,
                        decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))> {
    return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template<class Base, class T, class RefWrap, class... Args>
auto invoke_impl(T Base::*pmf, RefWrap&& ref, Args&&... args)
    noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<is_function_v<T> &&
                        is_reference_wrapper_v<std::decay_t<RefWrap>>,
                        decltype((ref.get().*pmf)(std::forward<Args>(args)...))> {
    return (ref.get().*pmf)(std::forward<Args>(args)...);
}

template<class Base, class T, class Pointer, class... Args>
auto invoke_impl(T Base::*pmf, Pointer&& ptr, Args&&... args)
    noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<is_function_v<T> &&
                        !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                        !is_base_of_v<Base, std::decay_t<Pointer>>,
                        decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))> {
    return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
}

template <class Base, class T, class Derived>
auto invoke_impl(T Base::*pmd, Derived&& ref)
    noexcept(noexcept(std::forward<Derived>(ref).*pmd))
    -> std::enable_if_t<!is_function_v<T> &&
                        is_base_of_v<Base, std::decay_t<Derived>>,
                        decltype(std::forward<Derived>(ref).*pmd)> {
    return std::forward<Derived>(ref).*pmd;
}

template <class Base, class T, class RefWrap>
auto invoke_impl(T Base::*pmd, RefWrap&& ref)
    noexcept(noexcept(ref.get().*pmd))
    -> std::enable_if_t<!is_function_v<T> &&
                        is_reference_wrapper_v<std::decay_t<RefWrap>>,
                        decltype(ref.get().*pmd)> {
    return ref.get().*pmd;
}

template <class Base, class T, class Pointer>
auto invoke_impl(T Base::*pmd, Pointer&& ptr)
    noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
    -> std::enable_if_t<!is_function_v<T> &&
                        !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                        !is_base_of_v<Base, std::decay_t<Pointer>>,
                        decltype((*std::forward<Pointer>(ptr)).*pmd)> {
    return (*std::forward<Pointer>(ptr)).*pmd;
}

template<class F, class... Args>
auto invoke_impl(F&& f, Args&&... args)
    noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
    -> std::enable_if_t<!is_member_pointer_v<std::decay_t<F>>,
                        decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
NS_DETAIL_END
    
template<class F, class... Args>
auto invoke(F&& f, Args&&... args)
    noexcept(noexcept(detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...)))
    -> decltype(detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...)) {
    return detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...);
}
    
/// apply c++17.
NS_DETAIL_BEGIN
template<class F, class Tuple, std::size_t... Is>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<Is...>) {
    return invoke(std::forward<F>(f), std::get<Is>(std::forward<Tuple>(t))...);
}
NS_DETAIL_END

template<class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t) {
    return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                              std::make_index_sequence<tuple_size_v<std::decay_t<Tuple>>>());
}

/// bit_cast.
///
/// https://gist.github.com/socantre/3472964
template<class Dest, class Source>
inline Dest bit_cast(const Source& source) {
    static_assert(sizeof(Dest) == sizeof(Source),
                  "size of destination and source objects must be equal.");
    
    static_assert(std::is_trivially_copyable<Dest>::value,
                  "destination type must be trivially copyable.");
    
    static_assert(std::is_trivially_copyable<Source>::value,
                  "source type must be trivially copyable.");
    Dest dest;
    std::memcpy(&dest, &source, sizeof(dest));
    return dest;
}
NS_EE_END

#endif /* EE_LIBRARY_EXTENSION_HPP_ */
