//
//  EEFunctionTraits.h
//  roll-eat
//
//  Created by Hoang Le Hai on 7/22/15.
//
//

#ifndef EE_LIBRARY_FUNCTION_TRAITS_HPP
#define EE_LIBRARY_FUNCTION_TRAITS_HPP

#include "EEMacro.hpp"

#include <tuple>

namespace_ee_begin
namespace_detail_begin
/// This is a Dummy class.
struct Dummy {};
namespace_detail_end

/// Lambda.
template<class T>
struct FunctionTraits : FunctionTraits<decltype(&T::operator())> {};

/// Base function traits: non-const member function pointer.
template<class R, class T, class... Args>
struct FunctionTraits<R(T::*)(Args...)> {
    using ReturnType = R;

    /// ClassType = Dummy means the function is static.
    using ClassType = T;

    /// Number of arguments.
    enum { Arity = sizeof...(Args) };

    /// FirstArgType = Dummy means the function has zero argument.
    template<std::size_t N>
    struct Argument {
        static_assert(N < Arity, "Invalid index.");
        using Type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
};

/// Const member function pointer.
template<class R, class T, class... Args>
struct FunctionTraits<R(T::*)(Args...) const> : FunctionTraits<R(T::*)(Args...)>{};

/// Static function pointer.
template<class R, class... Args>
struct FunctionTraits<R(*)(Args...)> : FunctionTraits<R(detail::Dummy::*)(Args...)>{};

/// Non-const member function pointer with zero argument.
template<class R, class T>
struct FunctionTraits<R(T::*)()> : FunctionTraits<R(T::*)(detail::Dummy)>{};

/// Const member function pointer with zero argument.
template<class R, class T>
struct FunctionTraits<R(T::*)() const> : FunctionTraits<R(T::*)()>{};

/// Static function pointer with zero argument.
template<class R>
struct FunctionTraits<R(*)> : FunctionTraits<R(detail::Dummy::*)()>{};

/// Resolve overloaded member function pointer.
template<class... Args>
struct FunctionResolver {
    /// Const.
    template<class R, class T>
    auto operator()(R(T::*f)(Args...) const) -> decltype(f) {
        return f;
    }

    /// Non-const.
    template<class R, class T>
    auto operator()(R(T::*f)(Args...)) -> decltype(f) {
        return f;
    }
};

/// Resolves overloaded member function pointer with zero argument.
template<>
struct FunctionResolver<> {
    /// Const.
    template<class R, class T>
    auto operator()(R(T::*f)() const) -> decltype(f) {
        return f;
    }

    /// Non-const.
    template<class R, class T>
    auto operator()(R(T::*f)()) -> decltype(f) {
        return f;
    }
};

/// Retrieves index of a specified type in a specified tuple.
template<class T, class Tuple>
struct GetTupleIndex;

template<class T, class... Ts>
struct GetTupleIndex<T, std::tuple<T, Ts...>> {
    enum { Index = 0 };
};

template<class T, class U, class... Ts>
struct GetTupleIndex<T, std::tuple<U, Ts...>> {
    enum { Index = 1 + GetTupleIndex<T, std::tuple<Ts...>>::Index };
};
    
/**
 * Sequence builder.
 *
 * Example:
 * - SequenceGenerator<3>::Type is Sequence<0, 1, 2>.
 * - SequenceGenerator<0>::Type is Sequence<>.
 */
template<std::size_t... S>
struct Sequence {
    static constexpr int Arity = sizeof...(S);
};

template<std::size_t N, std::size_t... S>
struct SequenceGenerator : SequenceGenerator<N - 1, N - 1, S...> {};
    
template<std::size_t... S>
struct SequenceGenerator<0, S...> {
    using Type = Sequence<S...>;
};

namespace_detail_begin
/// Runtime index for tuple.
template<std::size_t Index, class Tuple, class Functor>
void applyOne(Tuple& t, Functor f) {
    f(std::get<Index>(t));
}
namespace_detail_end

template<class Tuple, class Functor, std::size_t... Indices>
void apply(Tuple& t, std::size_t index, Functor f, Sequence<Indices...>) {
    using FuncType = void(Tuple&, Functor);
    static constexpr FuncType* Functions[] = { &detail::applyOne<Indices, Tuple, Functor>... };
    Functions[index](t, f);
}

template<class Tuple, class Functor>
void apply(Tuple& t, std::size_t index, Functor f) {
    apply(t, index, f, typename SequenceGenerator<std::tuple_size<Tuple>::value>::Type());
}

namespace_detail_begin
/// Reverse range for loop.
template<class T>
class ReverseWrapper {
public:
    ReverseWrapper(T& container) : _container(container) {}
    typename T::reverse_iterator begin() const { return _container.rbegin(); }
    typename T::reverse_iterator end() const { return _container.rend(); }
    
private:
    T& _container;
};

template<class T>
class ReverseWrapper<const T> {
public:
    ReverseWrapper(const T& container) : _container(container) {}
    typename T::const_reverse_iterator begin() const { return _container.crbegin(); }
    typename T::const_reverse_iterator end() const { return _container.crend(); }
    
private:
    const T& _container;
};
namespace_detail_end
    
template<class T>
detail::ReverseWrapper<T> reverse(T& container) {
    return detail::ReverseWrapper<T>(container);
}
    
template<class T>
detail::ReverseWrapper<const T> reverse(const T& container) {
    return detail::ReverseWrapper<const T>(container);
}
namespace_ee_end

#endif // EE_LIBRARY_FUNCTION_TRAITS_HPP
