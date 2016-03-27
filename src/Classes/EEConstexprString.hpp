//
//  EEConstexprString.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_CONSTEXPR_STRING_HPP
#define EE_LIBRARY_CONSTEXPR_STRING_HPP

#include "EEMacro.hpp"

NS_EE_BEGIN
template<char... Cs>
using TString = std::integer_sequence<char, Cs...>;

template<class T, T... Cs>
constexpr TString<Cs...> operator""_tstr() {
    return {};
}

// Compile-time string.
template<char... Cs>
struct CompileTimeString {
    static const char* Value() {
        static const char a[sizeof...(Cs)] = { Cs... };
        return a;
    }
};

// Concatenates 2 strings.
template<class L, class R>
struct ConcatenateTwo;

template<char... Ls, char... Rs>
struct ConcatenateTwo<CompileTimeString<Ls...>, CompileTimeString<Rs...>> {
    using Result = CompileTimeString<Ls..., Rs...>;
};

// Concatenates N strings.
template<class... Cs>
struct Concatenate;

template<class C>
struct Concatenate<C> {
    using Type = C;
};

template<class C>
using ConcatenateType = typename Concatenate<C>::Type;

template<class C1, class C2>
struct Concatenate<C1, C2> {
    using Type = ConcatenateType<C1, C2>;
};

template<class C, class ...Cs>
struct Concatenate<C, Cs...> {
    using Result = typename ConcatenateTwo<C, typename Concatenate<Cs...>::Result>::Result;
};
NS_EE_END

#endif /* EE_LIBRARY_CONSTEXPR_STRING_HPP */
