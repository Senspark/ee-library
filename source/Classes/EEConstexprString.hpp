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

namespace_ee_begin
// Compile-time string.
template<char... Cs>
struct CompileTimeString {
    static const char* value() {
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
    using Result = C;
};

template<class C1, class C2>
struct Concatenate<C1, C2> {
    using Result = typename ConcatenateTwo<C1, C2>::Result;
};

template<class C, class ...Cs>
struct Concatenate<C, Cs...> {
    using Result = typename ConcatenateTwo<C, typename Concatenate<Cs...>::Result>::Result;
};
namespace_ee_end

#endif /* EE_LIBRARY_CONSTEXPR_STRING_HPP */
