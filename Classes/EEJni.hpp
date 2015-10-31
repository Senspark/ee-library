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
#include "EEJniMethodInfo.hpp"

#include <jni.h>

namespace_ee_begin
#pragma mark JNI Signature Utilities
/**
 * Deduces the signature of a JNI method according to the variadic params and the return type.
 */
template<class T, class... Args>
const char* getJniSignature(Args...) {
    return Concatenate<
            CompileTimeString<'('>, //left parenthesis
            typename CppToJniConverter<Args>::JniType..., //params signature
            CompileTimeString<')'>, //right parenthesis
            typename CppToJniConverter<T>::JniType,
            CompileTimeString<'\0'>> //return type signature
            ::Result::value();
}

#pragma mark Public API

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
    static constexpr uint8_t Arity = sizeof...(Args);
    JNIEnv* jniEnv = JniUtils::getJNIEnvAttach();
    auto&& methodInfo = JniUtils::getStaticMethodInfo(className, methodName, getJniSignature<T, Args...>(args...));
    JniParamDestructor<Arity> paramDestructor(jniEnv);
    return JniCaller<T, decltype(CppToJniConverter<Args>::convert(args))...>::callStatic(jniEnv, methodInfo->classId, methodInfo->methodId, jniParamConverter<Args>(args, paramDestructor)...);
}

/**
 * Generic call to instance method.
 * @param instance The object instance.
 * @param className The class name.
 * @parem methodName The method name
 * @param args Input arguments (if available).
 * @return result whose type is T.
 */
template<class T = void, class... Args>
T call(jobject instance, const std::string& className, const std::string& methodName, const Args&... args) {
    static constexpr uint8_t Arity = sizeof...(Args);
    JNIEnv* jniEnv = JniUtils::getJNIEnvAttach();
    auto&& methodInfo = JniUtils::getMethodInfo(className, methodName, getJniSignature<T, Args...>(args...));
    JniParamDestructor<Arity> paramDestructor(jniEnv);
    return JniCaller<T, decltype(CppToJniConverter<Args>::convert(args))...>::callInstance(jniEnv, instance, methodInfo->methodId, jniParamConverter<Args>(args, paramDestructor)...);
}

/**
 * Retrieves a field in the given object instance.
 * @param instance The object instance.
 * @param propertyName The property name.
 * @return Value of the property.
 */
template<class T>
T getField(jobject instance, const std::string& propertyName) {
    JNIEnv* jniEnv = JniUtils::getJNIEnvAttach();
    jclass clazz = jniEnv->GetObjectClass(instance);
    const char* signature = Concatenate<typename CppToJniConverter<T>::JniType, CompileTimeString<'\0'>>::Result::value();
    jfieldID fid = jniEnv->GetFieldID(clazz, propertyName.c_str(), signature);
    return JniCaller<T>::getField(jniEnv, instance, fid);
}

// jobject global ref wrapper class.
struct JniObject {
    std::string jniClassName;
    jobject instance = nullptr;

    ~JniObject() {
        if (instance != nullptr) {
            auto jniEnv = JniUtils::getJNIEnv();
            jniEnv->DeleteGlobalRef(instance);
        }
    }
    
    template<class... Args>
    static std::shared_ptr<JniObject> create(const std::string& className, Args&&... args) {
        static constexpr uint8_t Arity = sizeof...(Args);
        auto&& result = new JniObject();
        auto&& jniEnv = JniUtils::getJNIEnvAttach();
        auto&& methodInfo = JniUtils::getMethodInfo(className, "<init>", getJniSignature<void, Args...>(args...));
        JniParamDestructor<Arity> paramDestructor(jniEnv);
        result->instance = jniEnv->NewObject(methodInfo->classId, methodInfo->methodId, jniParamConverter<Args>(args, paramDestructor)...);
        result->instance = jniEnv->NewGlobalRef(result->instance);
        result->jniClassName = className;
        return std::make_shared<JniObject>(result);
    }

    template<class T = void, class... Args>
    T call(const std::string& methodName, Args&&... args) const {
        return ee::call<T, Args...>(instance, jniClassName, methodName, std::forward<Args>(args)...);
    }
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_HPP */
