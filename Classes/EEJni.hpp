//
//  EESafeJni.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_HPP
#define EE_LIBRARY_JNI_HPP

/*
 * SafeJNI is licensed under MIT licensed. See LICENSE.md file for more information.
 * Copyright (c) 2014 MortimerGoro
 */

#include "EEMacro.hpp"
#include "EEConstexprString.hpp"
#include "EEJniCaller.hpp"
#include "EEJniConverter.hpp"
#include "EEJniDestructor.hpp"
#include "EEJniFieldInfo.hpp"
#include "EEJniMethodInfo.hpp"

#include <jni.h>

namespace_ee_begin
/**
 * Generic call to instance method.
 * @param instance The object instance.
 * @parem methodName The method name.
 * @param args Input arguments (if available).
 * @return result whose type is T.
 */
template<class T = void, class... Args>
T call(jobject instance, const std::string& methodName, const Args&... args) {
    EE_LOGD("call: methodName = %s", methodName.c_str());
    static constexpr uint8_t Arity = sizeof...(Args);
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    auto&& methodInfo = JniUtils::getMethodInfo(instance, methodName, getJniSignature<T, Args...>());
    JniParamDestructor<Arity> paramDestructor(env);
    return JniCaller<T, decltype(CppToJniConverter<Args>::convert(args))...>::call(env, instance, methodInfo->getMethodId(), jniParamConverter<Args>(args, paramDestructor)...);
}

/**
 * Generic call to static method.
 * @param className The class name.
 * @param methodName The method name.
 * @param args Input arguments (if available).
 * @return result whose type is T.
 *
 * callStatic<void>(...) or callStatic(...) will call a void function.
 * callStatic<int>(...) will call an int function and return an int result.
 */
template<class T = void, class... Args>
T callStatic(const std::string& className, const std::string& methodName, const Args&... args) {
    EE_LOGD("callStatic: className = %s methodName = %s", className.c_str(), methodName.c_str());
    static constexpr uint8_t Arity = sizeof...(Args);
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    auto&& methodInfo = JniUtils::getStaticMethodInfo(className, methodName, getJniSignature<T, Args...>());
    JniParamDestructor<Arity> paramDestructor(env);
    return JniCaller<T, decltype(CppToJniConverter<Args>::convert(args))...>::callStatic(env, methodInfo->getClass(), methodInfo->getMethodId(), jniParamConverter<Args>(args, paramDestructor)...);
}

/**
 * Retrieves a field in the given object instance.
 * @param instance The object instance.
 * @param fieldName The field name.
 * @return Value of the field.
 */
template<class T>
T getField(jobject instance, const std::string& fieldName) {
    EE_LOGD("getField: fieldName = %s", fieldName.c_str());
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    const char* signature = Concatenate<typename CppToJniConverter<T>::JniType, CompileTimeString<'\0'>>::Result::value();
    auto&& fieldInfo = JniUtils::getFieldInfo(instance, fieldName, signature);
    return JniCaller<T>::getField(env, instance, fieldInfo->getFieldId());
}

/**
 * Retrieves a static field in the given class.
 * @param className The class name.
 * @param fieldName The static field name.
 * @return Value of the static field.
 */
template<class T>
T getStaticField(const std::string& className, const std::string& fieldName) {
    EE_LOGD("getStaticField: className = %s fieldName = %s", className.c_str(), fieldName.c_str());
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    const char* signature = Concatenate<typename CppToJniConverter<T>::JniType, CompileTimeString<'\0'>>::Result::value();
    auto&& fieldInfo = JniUtils::getStaticFieldInfo(className, fieldName, signature);
    return JniCaller<T>::getStaticField(env, fieldInfo->getClass(), fieldInfo->getFieldId());
}

template<class T>
void setField(jobject instance, const std::string& fieldName, const T& value) {
    EE_LOGD("setField: fieldName = %s", fieldName.c_str());
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    const char* signature = Concatenate<typename CppToJniConverter<T>::JniType, CompileTimeString<'\0'>>::Result::value();
    auto&& fieldInfo = JniUtils::getFieldInfo(instance, fieldName, signature);
    JniParamDestructor<1> paramDestructor(env);
    JniCaller<T>::setField(env, instance, fieldInfo->getFieldId(), jniParamConverter<1>(value, paramDestructor));
}

template<class T>
void setStaticField(const std::string& className, const std::string& fieldName, const T& value) {
    EE_LOGD("setStaticField: className = %s fieldName = %s", className.c_str(), fieldName.c_str());
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    const char* signature = Concatenate<typename CppToJniConverter<T>::JniType, CompileTimeString<'\0'>>::Result::value();
    auto&& fieldInfo = JniUtils::getStaticFieldInfo(className, fieldName, signature);
    JniParamDestructor<1> paramDestructor(env);
    JniCaller<T>::setStaticField(env, fieldInfo->getClass(), fieldInfo->getFieldId(), jniParamConverter<1>(value, paramDestructor));
}

void setStaticField(const std::string& className, const std::string& fieldName, const char* signature, jobject value);
namespace_ee_end

#endif /* EE_LIBRARY_JNI_HPP */
