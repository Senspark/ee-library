//
//  EEExtension.hpp
//  tienlen
//
//  Created by enrevol on 3/22/16.
//
//

#ifndef EE_LIBRARY_EXTENSION_HPP_
#define EE_LIBRARY_EXTENSION_HPP_

#include <type_traits>

#include "EEMacro.hpp"

NS_EE_BEGIN
/// is_same_v N3854.
template<class T, class U>
constexpr auto is_same_v = std::is_same<T, U>::value;
    
/// is_function_v.
template<class T>
constexpr bool is_function_v = std::is_function<T>::value;
    
/// is_base_of_v.
template<class T, class U>
constexpr bool is_base_of_v = std::is_base_of<T, U>::value;

/// is_reference_v.
template<class T>
constexpr bool is_reference_v = std::is_reference<T>::value;

template<class T>
constexpr bool is_pod_v = std::is_pod<T>::value;

/// is_const_v.
template<class T>
constexpr bool is_const_v = std::is_const<T>::value;
    
/// is_reference_wrapper.
template<class T>
struct is_reference_wrapper {
    static constexpr bool value = false;
};

template<class T>
struct is_reference_wrapper<std::reference_wrapper<T>> {
    static constexpr bool value = true;
};
    
/// is_reference_wrapper_v.
template<class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;
    
/// is_member_pointer_v.
template<class T>
constexpr bool is_member_pointer_v = std::is_member_pointer<T>::value;
    
/// tuple_size_v c++17.
template<class T>
constexpr std::size_t tuple_size_v = std::tuple_size<T>::value;
    
/// as_const c++17.
template<class T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
    return t;
}
    
// conjunction c++17.
template<class...> struct conjunction : std::true_type {};
template<class B> struct conjunction<B> : B {};
template<class B, class... Bs>
struct conjunction<B, Bs...> : std::conditional_t<B::value != false, conjunction<Bs...>, B> {};
    
template<class... Bs>
constexpr bool conjunction_v = conjunction<Bs...>::value;
    
// disjunction c++17.
template<class...> struct disjunction : std::false_type {};
template<class B> struct disjunction<B> : B {};
template<class B, class... Bs>
struct disjunction<B, Bs...> : std::conditional_t<B::value != false, B, disjunction<Bs...>> {};
    
template<class... Bs>
constexpr bool disjunction_v = disjunction<Bs...>::value;
    
// bool_constant c++17.
template<bool B>
using bool_constant = std::integral_constant<bool, B>;
    
// negation c++17.
template<class B>
struct negation : bool_constant<!B::value> {};
    
template<class B>
constexpr bool negation_v = negation<B>::value;
    
/// bool_sequence.
template<bool... Bs>
using bool_sequence = std::integer_sequence<bool, Bs...>;

/// bool_and.
template<bool... Bs>
using bool_and = std::is_same<bool_sequence<Bs...>, bool_sequence<(Bs || true)...>>;
    
template<bool... Bs>
constexpr auto bool_and_v = bool_and<Bs...>::value;

/// bool_or.
template<bool... Bs>
using bool_or = std::integral_constant<bool, !bool_and_v<!Bs...>>;

template<bool... Bs>
constexpr auto bool_or_v = bool_or<Bs...>::value;
    
/// enable_if_any.
template<class R, bool... Bs>
using enable_if_any = std::enable_if<bool_or_v<Bs...>, R>;
    
template<class R, bool... Bs>
using enable_if_any_t = typename enable_if_any<R, Bs...>::type;

/// enable_if_all.
template<class R, bool... Bs>
using enable_if_all = std::enable_if<bool_and_v<Bs...>, R>;

template<class R, bool... Bs>
using enable_if_all_t = typename enable_if_all<R, Bs...>::type;
    
/// are_same.
template<class T, class... Ts>
using are_same = bool_and<is_same_v<T, Ts>...>;

template<class T, class... Ts>
constexpr auto are_same_v = are_same<T, Ts...>::value;

/// one_same
template<class T, class... Ts>
using one_same = disjunction<std::is_same<T, Ts>...>;

template<class T, class... Ts>
constexpr auto one_same_v = one_same<T, Ts...>::value;
    
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
template<class Dest, class Source>
inline Dest bit_cast(const Source& source) {
    static_assert(sizeof(Dest) == sizeof(Source), "size of destination and source objects must be equal.");
    static_assert(std::is_trivially_copyable<Dest>::value, "destination type must be trivially copyable.");
    static_assert(std::is_trivially_copyable<Source>::value, "source type must be trivially copyable.");
    Dest dest;
    std::memcpy(&dest, &source, sizeof(dest));
    return dest;
}
NS_EE_END

#endif /* EE_LIBRARY_EXTENSION_HPP_ */
