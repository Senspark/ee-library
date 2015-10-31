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

namespace_ee_begin

#pragma mark C++ To JNI conversion templates

// Default template.
template<class T>
struct CppToJniConverter;

// Generic pointer implementation (using jlong type).
template<class T>
struct CppToJniConverter<T*> {
    using JniType = CompileTimeString<'J'>;
    
    static jlong convert(T* obj) {
        return reinterpret_cast<jlong>(obj);
    }
};

// Object implementations.

/// Converter for std::string.
template<>
struct CppToJniConverter<std::string> {
    /// Ljava/lang/String;
    using JniType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    static jstring convert(const std::string& obj) {
        return JniUtils::toJString(obj);
    }
};

/// Converter for const char*.
template<>
struct CppToJniConverter<const char*> {
    /// Ljava/lang/String;
    using JniType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    static jstring convert(const char* obj) {
        return JniUtils::toJString(obj);
    }
};

/// Converter for std::vector<std::string>.
template<>
struct CppToJniConverter<std::vector<std::string>> {
    /// [Ljava/lang/String;
    using JniType = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    
    static jobjectArray convert(const std::vector<std::string>& obj) {
        return JniUtils::toJObjectArray(obj);
    }
};

/// Converter for std::vector<unsigned char>.
template<>
struct CppToJniConverter<std::vector<uint8_t>> {
    using JniType = CompileTimeString<'[','B'>;
    
    static jbyteArray convert(const std::vector<uint8_t>& obj) {
        return JniUtils::toJObjectArray(obj);
    }
};

template<>
struct CppToJniConverter<std::vector<jobject>> {
    /// [Ljava/lang/Object;
    using JniType = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;
    
    static jbyteArray convert(const std::vector<uint8_t>& obj) {
        return JniUtils::toJObjectArray(obj);
    }
};

// Add more types here.

// Converter for void.
template<>
struct CppToJniConverter<void> {
    using JniType = CompileTimeString<'V'>;
};

// Primivite objects implementation.

/// Converter for int.
template<>
struct CppToJniConverter<int32_t> {
    using JniType = CompileTimeString<'I'>;
    
    static jint convert(int value) {
        return static_cast<jint>(value);
    }
};

/// Converter for long.
template<>
struct CppToJniConverter<int64_t> {
    using JniType = CompileTimeString<'J'>;
    
    inline static jlong convert(int64_t value) {
        return static_cast<jlong>(value);
    }
};

/// Converter for float.
template<>
struct CppToJniConverter<float> {
    using JniType = CompileTimeString<'F'>;
    
    static jfloat convert(float value) {
        return static_cast<jfloat>(value);
    }
};

/// Converter for double.
template<>
struct CppToJniConverter<double> {
    using JniType = CompileTimeString<'D'>;
    
    static jdouble convert(double value) {
        return static_cast<jdouble>(value);
    }
};

/// Converter for boolean.
template<>
struct CppToJniConverter<bool> {
    using JniType = CompileTimeString<'Z'>;
    
    static jboolean convert(bool value) {
        return static_cast<jboolean>(value);
    }
};

/// Converter for byte.
template<>
struct CppToJniConverter<int8_t> {
    using JniType = CompileTimeString<'B'>;
    
    static jbyte convert(int8_t value) {
        return static_cast<jbyte>(value);
    }
};

/// Converter for char.
template<>
struct CppToJniConverter<uint8_t> {
    using JniType = CompileTimeString<'C'>;
    
    static jchar convert(uint8_t value) {
        return static_cast<uint8_t>(value);
    }
};

/// Converter for short.
template<>
struct CppToJniConverter<int16_t> {
    using JniType = CompileTimeString<'S'>;
    
    static jshort convert(int16_t value) {
        return static_cast<jshort>(value);
    }
};

#pragma mark JNI To C++ conversion templates

template<class T>
struct JniToCppConverter;
// {
//     static void convert(T);
//     static const char* jniTypeName();
// };

/// Convert to std::string.
template<>
struct JniToCppConverter<std::string> {
    static std::string convert(jobject obj) {
        return JniUtils::toString((jstring) obj);
    }
};

/// Convert to std::vector<std::string>.
template<>
struct JniToCppConverter<std::vector<std::string>> {
    static std::vector<std::string> convert(jobject obj) {
        return JniUtils::toVectorString((jobjectArray) obj);
    }
};

/// Convert to std::vector<uint8_t>
template<>
struct JniToCppConverter<std::vector<uint8_t>> {
    static std::vector<uint8_t> convert(jobject obj) {
        return JniUtils::toVectorByte((jbyteArray) obj);
    }
};

/// Convert to std::vector<jobject>.
template<>
struct JniToCppConverter<std::vector<jobject>> {
    static std::vector<jobject> convert(jobject obj) {
        return JniUtils::toVectorJObject((jobjectArray) obj);
    }
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_CONVERTER_HPP */
