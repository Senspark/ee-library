//
//  EEJniConverter.cpp
//  ee-library
//
//  Created by enrevol on 11/11/15.
//
//

#include "EEJniConverter.hpp"
#include "EEJni.hpp"

namespace_ee_begin
jstring CppToJniConverter::toString(const char* str) {
    JNIEnv* env = JniUtils::getJNIEnv();
    return env->NewStringUTF(str);
}

namespace_detail_begin
namespace_anonymous_begin
template<class T>
jobject create_boxed_type_object(const std::string& className, const char* signature, T value) {
    JNIEnv* env = JniUtils::getJNIEnv();
    auto&& methodInfo = JniUtils::getConstructorInfo(className, signature);
    return env->NewObject(methodInfo->getClass(), methodInfo->getMethodId(), value);
}
namespace_anonymous_end
namespace_detail_end

jobject CppToJniConverter::toObject(std::int8_t value) {
    return detail::create_boxed_type_object("java/lang/Byte", "(B)V", value);
}

jobject CppToJniConverter::toObject(std::uint8_t value) {
    return detail::create_boxed_type_object("java/lang/Boolean", "(Z)V", value);
}

jobject CppToJniConverter::toObject(std::int16_t value) {
    return detail::create_boxed_type_object("java/lang/Short", "(S)V", value);
}

jobject CppToJniConverter::toObject(std::uint16_t value) {
    return detail::create_boxed_type_object("java/lang/Character", "(C)V", value);
}

jobject CppToJniConverter::toObject(std::int32_t value) {
    return detail::create_boxed_type_object("java/lang/Integer", "(I)V", value);
}

jobject CppToJniConverter::toObject(std::int64_t value) {
    return detail::create_boxed_type_object("java/lang/Long", "(J)V", value);
}

jobject CppToJniConverter::toObject(float value) {
    return detail::create_boxed_type_object("java/lang/Float", "(F)V", value);
}

jobject CppToJniConverter::toObject(double value) {
    return detail::create_boxed_type_object("java/lang/Double", "(D)V", value);
}

jobjectArray CppToJniConverter::toStringArray(const std::vector<std::string>& data) {
    JNIEnv* env = JniUtils::getJNIEnv();
    jclass clazz = env->FindClass("java/lang/String");
    jsize size = data.size();
    jobjectArray result = env->NewObjectArray(size, clazz, nullptr);
    for (jsize index = 0; index < size; ++index) {
        jstring str = toString(data.at(index));
        env->SetObjectArrayElement(result, index, str);
    }
    env->DeleteLocalRef(clazz);
    JniUtils::checkException();
    return result;
}

jobjectArray CppToJniConverter::toObjectArray(const std::vector<jobject>& data) {
    JNIEnv* env = JniUtils::getJNIEnv();
    jclass clazz = env->FindClass("java/lang/Object");
    jsize size = data.size();
    jobjectArray result = env->NewObjectArray(size, clazz, nullptr);
    for (jsize index = 0; index < size; ++index) {
        jobject obj = data.at(index);
        env->SetObjectArrayElement(result, index, obj);
    }
    env->DeleteLocalRef(clazz);
    JniUtils::checkException();
    return result;
}

jbyte    CppToJniConverter::toByte(std::int8_t value)     { return static_cast<jbyte>(value); }
jboolean CppToJniConverter::toBoolean(std::uint8_t value) { return static_cast<jboolean>(value); }
jshort   CppToJniConverter::toShort(std::int16_t value)   { return static_cast<jshort>(value); }
jchar    CppToJniConverter::toChar(std::uint16_t value)   { return static_cast<jchar>(value); }
jint     CppToJniConverter::toInt(std::int32_t value)     { return static_cast<jint>(value); }
jlong    CppToJniConverter::toLong(std::int64_t value)    { return static_cast<jlong>(value); }
jfloat   CppToJniConverter::toFloat(float value)          { return static_cast<jfloat>(value); }
jdouble  CppToJniConverter::toDouble(double value)        { return static_cast<jdouble>(value); }

jbyteArray CppToJniConverter::toByteArray(const std::vector<std::int8_t>& data) {
    JNIEnv* env = JniUtils::getJNIEnv();
    jsize size = data.size();
    jbyteArray result = env->NewByteArray(size);
    env->SetByteArrayRegion(result, 0, size, static_cast<const jbyte*>(&data[0]));
    JniUtils::checkException();
    return result;
}

std::string JniToCppConverter::toString(jstring str) {
    JNIEnv* env = JniUtils::getJNIEnv();
    jboolean isCopy;
    const char* chars = env->GetStringUTFChars(str, &isCopy);
    std::string result(chars);
    env->ReleaseStringUTFChars(str, chars);
    JniUtils::checkException();
    return result;
}

std::vector<std::string> JniToCppConverter::toVectorString(jobjectArray array) {
    JNIEnv* env = JniUtils::getJNIEnv();
    std::vector<std::string> result;
    jsize size = env->GetArrayLength(array);
    result.reserve(size);
    for (jsize index = 0; index < size; ++index) {
        jstring str = reinterpret_cast<jstring>(env->GetObjectArrayElement(array, index));
        result.push_back(toString(str));
        env->DeleteLocalRef(str);
    }
    JniUtils::checkException();
    return result;
}

std::vector<jobject> JniToCppConverter::toVectorObject(jobjectArray array) {
    JNIEnv* env = JniUtils::getJNIEnv();
    std::vector<jobject> result;
    jsize size = env->GetArrayLength(array);
    result.reserve(size);
    for (jsize index = 0; index < size; ++index) {
        jobject obj = env->GetObjectArrayElement(array, index);
        result.push_back(obj);
    }
    JniUtils::checkException();
    return result;
}

std::int8_t   JniToCppConverter::toInt8(jbyte value)     { return static_cast<std::int8_t>(value); }
std::uint8_t  JniToCppConverter::toUInt8(jboolean value) { return static_cast<std::uint8_t>(value); }
std::int16_t  JniToCppConverter::toInt16(jshort value)   { return static_cast<std::int16_t>(value); }
std::uint16_t JniToCppConverter::toUInt16(jchar value)   { return static_cast<std::uint16_t>(value); }
std::int32_t  JniToCppConverter::toInt32(jint value)     { return static_cast<std::int32_t>(value); }
std::int64_t  JniToCppConverter::toInt64(jlong value)    { return static_cast<std::int64_t>(value); }
float         JniToCppConverter::toFloat(jfloat value)   { return static_cast<float>(value); }
double        JniToCppConverter::toDouble(jdouble value) { return static_cast<double>(value); }

std::vector<std::int8_t> JniToCppConverter::toVectorInt8(jbyteArray array) {
    JNIEnv* env = JniUtils::getJNIEnv();
    jsize size = env->GetArrayLength(array);
    std::vector<std::int8_t> result(size);
    env->GetByteArrayRegion(array, 0, size, static_cast<jbyte*>(&result[0]));
    JniUtils::checkException();
    return result;
}

std::uint8_t  JniToCppConverter::toUInt8(jobject obj)  { return call<bool>(obj, "booleanValue"); }
std::int32_t  JniToCppConverter::toInt32(jobject obj)  { return call<std::int32_t>(obj, "intValue"); }
float         JniToCppConverter::toFloat(jobject obj)  { return call<float>(obj, "floatValue"); }
double        JniToCppConverter::toDouble(jobject obj) { return call<double>(obj, "doubleValue"); }
namespace_ee_end
