//
//  EEJniBoxedType.hpp
//  ee-library
//
//  Created by enrevol on 11/12/15.
//
//

#ifndef EE_LIBRARY_JNI_BOXED_TYPE_HPP
#define EE_LIBRARY_JNI_BOXED_TYPE_HPP

#include "EEMacro.hpp"

#include <cstdint>
#include <string>

namespace_ee_begin
template<class T>
class BoxedType {
public:
    using PrimitiveType = T;
    
    constexpr BoxedType(PrimitiveType value = PrimitiveType()) : _value(value) {}
    
    constexpr PrimitiveType getValue() const {
        return _value;
    }
    
    constexpr operator PrimitiveType() const {
        return getValue();
    }
    
private:
    const PrimitiveType _value;
};

template<class T>
struct MakeBoxedType {
    using Type = std::conditional_t<
        std::is_same<T, std::int8_t>::value ||
        std::is_same<T, bool>::value ||
        std::is_same<T, std::int16_t>::value ||
        std::is_same<T, std::uint8_t>::value ||
        std::is_same<T, std::int32_t>::value ||
        std::is_same<T, std::int64_t>::value ||
        std::is_same<T, float>::value ||
        std::is_same<T, double>::value,
        BoxedType<T>,
        T>;
};

template<class T>
using MakeBoxedTypeT = typename MakeBoxedType<T>::Type;

extern template class BoxedType<std::int8_t>;
extern template class BoxedType<bool>;
extern template class BoxedType<std::int16_t>;
extern template class BoxedType<std::uint8_t>;
extern template class BoxedType<std::int32_t>;
extern template class BoxedType<std::int64_t>;
extern template class BoxedType<float>;
extern template class BoxedType<double>;

using Byte      = BoxedType<std::int8_t>;
using Boolean   = BoxedType<bool>;
using Short     = BoxedType<std::int16_t>;
using Character = BoxedType<std::uint8_t>;
using Integer   = BoxedType<std::int32_t>;
using Long      = BoxedType<std::int64_t>;
using Float     = BoxedType<float>;
using Double    = BoxedType<double>;
namespace_ee_end

#endif /* EE_LIBRARY_JNI_BOXED_TYPE_HPP */
