//
//  EECppToJniConverter.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_CONVERTER_HPP
#define EE_LIBRARY_JNI_CONVERTER_HPP

#include "EEMacro.hpp"
#include "EEConstexprString.hpp"
#include "EEJniUtils.hpp"
#include "EEJniBoxedType.hpp"

#include <string>
#include <vector>

namespace_ee_begin
class CppToJniConverter {
public:
    /// Converts const char* to String.
    static jstring toString(const char* str);
    
    /// Converts std::string to String.
    static jstring toString(const std::string& str) {
        return toString(str.c_str());
    }
    
    /// Converts std::vector<std::string> to String[].
    static jobjectArray toStringArray(const std::vector<std::string>& data);
    
    /// Converts std::vector<jobject> to Object[].
    static jobjectArray toObjectArray(const std::vector<jobject>& data);
    
    /// Converts to wrapper classes in Java.
    static jobject toObject(std::int8_t value);         ///< Converts std::int8_t to Byte.
    static jobject toObject(std::uint8_t value);        ///< Converts std::uint8_t to Boolean.
    static jobject toObject(std::int16_t value);        ///< Converts std::int16_t to Short.
    static jobject toObject(std::uint16_t value);       ///< Converts std::uint16_t to Character.
    static jobject toObject(std::int32_t value);        ///< Converts std::int32_t to Integer.
    static jobject toObject(std::int64_t value);        ///< Converts std::int64_t to Long.
    static jobject toObject(float value);               ///< Converts float to Float.
    static jobject toObject(double value);              ///< Converts double to Double.
    
    static jobjectArray toObjectArray(const std::vector<std::int8_t>& data);        ///< Converts std::vector<std::int8_t> to Byte[].
    static jobjectArray toObjectArray(const std::vector<std::uint8_t>& data);       ///< Converts std::vector<std::uint8_t> to Boolean[].
    static jobjectArray toObjectArray(const std::vector<std::int16_t>& data);       ///< Converts std::vector<std::int16_t> to Short[].
    static jobjectArray toObjectArray(const std::vector<std::uint16_t>& data);      ///< Converts std::vector<std::uint16_t> to Character[].
    static jobjectArray toObjectArray(const std::vector<std::int32_t>& data);       ///< Converts std::vector<std::int32_t> to Integer[].
    static jobjectArray toObjectArray(const std::vector<std::int64_t>& data);       ///< Converts std::vector<std::int64_t> to Long[].
    static jobjectArray toObjectArray(const std::vector<float>& data);              ///< Converts std::vector<float> to Float[].
    static jobjectArray toObjectArray(const std::vector<double>& data);             ///< Converts std::vector<double> to Double[].
    
    /// Converts to primitive types in Java.
    static jbyte    toByte(std::int8_t value);          ///< Converts std::int8_t to byte.
    static jboolean toBoolean(std::uint8_t value);      ///< Converts std::uint8_t to boolean.
    static jshort   toShort(std::int16_t value);        ///< Converts std::int16_t to short.
    static jchar    toChar(std::uint16_t value);        ///< Converts std::uint16_t to char.
    static jint     toInt(std::int32_t value);          ///< Converts std::int32_t to int.
    static jlong    toLong(std::int64_t value);         ///< Converts std::int64_t to long.
    static jfloat   toFloat(float value);               ///< Converts float to float.
    static jdouble  toDouble(double value);             ///< Converts double to double.
    
    static jbyteArray    toByteArray(const std::vector<std::int8_t>& data);         /// Converts std::vector<std::int8_t> to byte[].
    static jbooleanArray toBooleanArray(const std::vector<std::uint8_t>& data);     /// Converts std::vector<std::uint8_t> to boolean[].
    static jshortArray   toShortArray(const std::vector<std::int16_t>& data);       /// Converts std::vector<std::int16_t> to short[].
    static jcharArray    toCharArray(const std::vector<std::uint16_t>& data);       /// Converts std::vector<std::uint16_t> to char[].
    static jintArray     toIntArray(const std::vector<std::int32_t>& data);         /// Converts std::vector<std::int32_t> to int[].
    static jlongArray    toLongArray(const std::vector<std::int64_t>& data);        /// Converts std::vector<std::int64_t> to long[].
    static jfloatArray   toFloatArray(const std::vector<float>& data);              /// Converts std::vector<float> to float[].
    static jdoubleArray  toDoubleArray(const std::vector<double>& data);            /// Converts std::vector<double> to double[].
};

class JniToCppConverter {
public:
    /// Converts String to std::string.
    static std::string toString(jstring str);
    
    /// Converts String[] to std::vector<std::string>.
    static std::vector<std::string> toVectorString(jobjectArray array);
    
    /// Converts Object[] to std::vector<jobject>.
    static std::vector<jobject> toVectorObject(jobjectArray array);
    
    /// Converts primitive types in Java to correspoding data types in C++.
    static std::int8_t   toInt8(jbyte value);           ///< Converts byte to std::int8_t.
    static std::uint8_t  toUInt8(jboolean value);       ///< Converts boolean to std::uint8_t.
    static std::int16_t  toInt16(jshort value);         ///< Converts short to std::int16_t.
    static std::uint16_t toUInt16(jchar value);         ///< Converts char to std::uint16_t.
    static std::int32_t  toInt32(jint value);           ///< Converts int to std::int32_t.
    static std::int64_t  toInt64(jlong value);          ///< Converts long to std::int64_t.
    static float         toFloat(jfloat value);         ///< Converts float to float.
    static double        toDouble(jdouble value);       ///< Converts double to double.
    
    static std::vector<std::int8_t>   toVectorInt8(jbyteArray array);
    static std::vector<std::uint8_t>  toVectorUInt8(jbyteArray array);
    static std::vector<std::int16_t>  toVectorInt16(jshortArray array);
    static std::vector<std::uint16_t> toVectorUInt16(jcharArray array);
    static std::vector<std::int32_t>  toVectorInt32(jintArray array);
    static std::vector<std::int64_t>  toVectorInt64(jlongArray array);
    static std::vector<float>         toVectorFloat(jfloatArray array);
    static std::vector<double>        toVectorDouble(jdoubleArray array);
    
    /// Converts wrapper classes in Java to correspoding data types in C++.
    static std::int8_t   toInt8(jobject obj);           ///< Converts Byte to std::int8_t.
    static std::uint8_t  toUInt8(jobject obj);          ///< Converts Boolean to std::uint8_t.
    static std::int16_t  toInt16(jobject obj);          ///< Converts Short to std::int16_t.
    static std::uint16_t toUInt16(jobject obj);         ///< Converts Char to std::uint16_t.
    static std::int32_t  toInt32(jobject obj);          ///< Converts Integer to std::int32_t.
    static std::int64_t  toInt64(jobject obj);          ///< Converts Long to std::int64_t.
    static float         toFloat(jobject obj);          ///< Converts Float to float.
    static double        toDouble(jobject obj);         ///< Converts Double to double.
    
    static std::vector<std::int8_t>   toVectorInt8(jobjectArray array);
    static std::vector<std::uint8_t>  toVectorUInt8(jobjectArray array);
    static std::vector<std::int16_t>  toVectorInt16(jobjectArray array);
    static std::vector<std::uint16_t> toVectorUInt16(jobjectArray array);
    static std::vector<std::int32_t>  toVectorInt32(jobjectArray array);
    static std::vector<std::int64_t>  toVectorInt64(jobjectArray array);
    static std::vector<float>         toVectorFloat(jobjectArray array);
    static std::vector<double>        toVectorDouble(jobjectArray array);
};

namespace_detail_begin
#pragma mark C++ To JNI conversion templates

// Default template.
template<class T>
struct CppToJni;

// Generic pointer implementation (using jlong type).
template<class T>
struct CppToJni<T*> {
    using Signature = CompileTimeString<'J'>;
    
    static jlong convert(T* obj) {
        return reinterpret_cast<jlong>(obj);
    }
};

// Object implementations.

/// Converts std::string to String.
template<>
struct CppToJni<std::string> {
    /// Ljava/lang/String;
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    inline static jstring convert(const std::string& str) {
        return CppToJniConverter::toString(str);
    }
};

/// Converts const char* to String.
template<>
struct CppToJni<const char*> {
    /// Ljava/lang/String;
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    inline static jstring convert(const char* str) {
        return CppToJniConverter::toString(str);
    }
};

/// Converts std::vector<std::string> to String[].
template<>
struct CppToJni<std::vector<std::string>> {
    /// [Ljava/lang/String;
    using Signature = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    inline static jobjectArray convert(const std::vector<std::string>& data) {
        return CppToJniConverter::toStringArray(data);
    }
};

/// Converts std::vector<jobject> to Object[].
template<>
struct CppToJni<std::vector<jobject>> {
    /// [Ljava/lang/Object;
    using Signature = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;
    
    inline static jobjectArray convert(const std::vector<jobject>& data) {
        return CppToJniConverter::toObjectArray(data);
    }
};

/// Converts signed char to Byte.
template<>
struct CppToJni<Byte> {
    /// Ljava/lang/Byte;
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'B', 'y', 't', 'e', ';'>;
    
    inline static jobject convert(Byte value) {
        return CppToJniConverter::toObject(static_cast<std::int8_t>(value));
    }
};

/// Converts bool to Boolean.
template<>
struct CppToJni<Boolean> {
    /// Ljava/lang/Boolean;
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'B', 'o', 'o', 'l', 'e', 'a', 'n', ';'>;
    
    inline static jobject convert(Boolean value) {
        return CppToJniConverter::toObject(static_cast<std::uint8_t>(value));
    }
};

/// Converts short to Short.
template<>
struct CppToJni<Short> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 'h', 'o', 'r', 't', ';'>;
    
    inline static jobject convert(Short value) {
        return CppToJniConverter::toObject(static_cast<std::int16_t>(value));
    }
};

/// Converts unsigned char to Character.
template<>
struct CppToJni<Character> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'C', 'h', 'a', 'r', 'a', 'c', 't', 'e', 'r', ';'>;
    
    inline static jobject convert(Character value) {
        return CppToJniConverter::toObject(static_cast<std::uint16_t>(value));
    }
};

/// Converts int to Integer.
template<>
struct CppToJni<Integer> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'I', 'n', 't', 'e', 'g', 'e', 'r', ';'>;
    
    inline static jobject convert(Integer value) {
        return CppToJniConverter::toObject(static_cast<std::int32_t>(value));
    }
};

/// Converts long long to Long.
template<>
struct CppToJni<Long> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'L', 'o', 'n', 'g', ';'>;
    
    inline static jobject convert(Long value) {
        return CppToJniConverter::toObject(static_cast<std::int64_t>(value));
    }
};

/// Converts float to Float.
template<>
struct CppToJni<Float> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'F', 'l', 'o', 'a', 't', ';'>;
    
    inline static jobject convert(Float value) {
        return CppToJniConverter::toObject(static_cast<float>(value));
    }
};

/// Converts double to Double.
template<>
struct CppToJni<Double> {
    using Signature = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'D', 'o', 'u', 'b', 'l', 'e', ';'>;
    
    inline static jobject convert(Double value) {
        return CppToJniConverter::toObject(static_cast<double>(value));
    }
};

// Converter for void.
template<>
struct CppToJni<void> {
    using Signature = CompileTimeString<'V'>;
};

// Primivite objects implementation.

/// Converter for byte.
template<>
struct CppToJni<std::int8_t> {
    using Signature = CompileTimeString<'B'>;
    
    inline static jbyte convert(std::int8_t value) {
        return CppToJniConverter::toByte(value);
    }
};

/// Converter for boolean.
template<>
struct CppToJni<bool> {
    using Signature = CompileTimeString<'Z'>;
    
    inline static jboolean convert(bool value) {
        return CppToJniConverter::toBoolean(value);
    }
};

/// Converter for short.
template<>
struct CppToJni<std::int16_t> {
    using Signature = CompileTimeString<'S'>;
    
    inline static jshort convert(std::int16_t value) {
        return CppToJniConverter::toShort(value);
    }
};

/// Converter for char.
template<>
struct CppToJni<std::uint8_t> {
    using Signature = CompileTimeString<'C'>;
    
    inline static jchar convert(std::uint8_t value) {
        return CppToJniConverter::toChar(value);
    }
};

/// Converter for int.
template<>
struct CppToJni<std::int32_t> {
    using Signature = CompileTimeString<'I'>;
    
    inline static jint convert(std::int32_t value) {
        return CppToJniConverter::toInt(value);
    }
};

/// Converter for long.
template<>
struct CppToJni<std::int64_t> {
    using Signature = CompileTimeString<'J'>;
    
    inline static jlong convert(std::int64_t value) {
        return CppToJniConverter::toLong(value);
    }
};

/// Converter for float.
template<>
struct CppToJni<float> {
    using Signature = CompileTimeString<'F'>;
    
    inline static jfloat convert(float value) {
        return CppToJniConverter::toFloat(value);
    }
};

/// Converter for double.
template<>
struct CppToJni<double> {
    using Signature = CompileTimeString<'D'>;
    
    inline static jdouble convert(double value) {
        return CppToJniConverter::toDouble(value);
    }
};

/// Converts std::vector<std::int8_t> to byte[].
template<>
struct CppToJni<std::vector<std::int8_t>> {
    using Signature = CompileTimeString<'[','B'>;
    
    inline static jbyteArray convert(const std::vector<std::int8_t>& data) {
        return CppToJniConverter::toByteArray(data);
    }
};

/// Converts std::vector<std::int32_t> to int[].
template<>
struct CppToJni<std::vector<std::int32_t>> {
    using Signature = CompileTimeString<'[', 'I'>;
    
    inline static jintArray convert(const std::vector<std::int32_t>& data) {
        return CppToJniConverter::toIntArray(data);
    }
};

#pragma mark JNI To C++ conversion templates

template<class T>
struct JniToCpp;
// {
//     static void convert(T);
//     static const char* jniTypeName();
// };

/// Converts to std::string.
template<>
struct JniToCpp<std::string> {
    inline static std::string convert(jobject str) {
        return JniToCppConverter::toString(static_cast<jstring>(str));
    }
};

template<>
struct JniToCpp<std::int8_t> {
    inline static std::int8_t convert(jbyte value) {
        return JniToCppConverter::toInt8(value);
    }
};

template<>
struct JniToCpp<bool> {
    /// Converts Boolean to bool.
    inline static bool convert(jobject obj) {
        return JniToCppConverter::toUInt8(obj);
    }

    /// Converts boolean to bool.
    inline static bool convert(jboolean value) {
        return JniToCppConverter::toUInt8(value);
    }
};

template<>
struct JniToCpp<std::int32_t> {
    /// Converts Integer to int.
    inline static int convert(jobject obj) {
        return JniToCppConverter::toInt32(obj);
    }
    
    /// Converts int to int.
    inline static int convert(jint value) {
        return JniToCppConverter::toInt32(value);
    }
};

/// Converts String[] to std::vector<std::string>.
template<>
struct JniToCpp<std::vector<std::string>> {
    inline static std::vector<std::string> convert(jobject obj) {
        return JniToCppConverter::toVectorString(static_cast<jobjectArray>(obj));
    }
};

/// Converts Object[] to std::vector<jobject>.
template<>
struct JniToCpp<std::vector<jobject>> {
    inline static std::vector<jobject> convert(jobject obj) {
        return JniToCppConverter::toVectorObject(static_cast<jobjectArray>(obj));
    }
};
namespace_detail_end

/**
 * Deduces the signature of a JNI method according to the variadic params and the return type.
 */
template<class T, class... Args>
const char* getJniSignature() {
    return Concatenate<
            CompileTimeString<'('>, // Left parenthesis.
            typename detail::CppToJni<Args>::Signature..., // Params signature.
            CompileTimeString<')'>, // Right parenthesis.
            typename detail::CppToJni<T>::Signature, // Return type signature.
            CompileTimeString<'\0'>>
            ::Result::value();
}
namespace_ee_end

#endif /* EE_LIBRARY_JNI_CONVERTER_HPP */
