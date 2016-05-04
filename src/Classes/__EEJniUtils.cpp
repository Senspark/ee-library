//
//  EEJniUtils.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "EEJniUtils.hpp"
#include "EEJni.hpp"
#include "EEJniException.hpp"
#include "EEJniFieldInfo.hpp"
#include "EEJniMethodInfo.hpp"

#include <utility>

namespace_ee_begin
JNIEnv* JniUtils::env = nullptr;
JavaVM* JniUtils::javaVM = nullptr;

void JniUtils::init(JavaVM* vm, JNIEnv* jniEnv) {
    JniUtils::javaVM = vm;
    JniUtils::env = jniEnv;
}

JNIEnv* JniUtils::getJNIEnvAttach() {
    if (javaVM != nullptr) {
        int status = javaVM->AttachCurrentThread(&env, nullptr);
        if (status < 0) {
            throw JniException("Could not attach the JNI environment to the current thread.");
        }
    }
    return env;
}

void JniUtils::setJniEnv(JNIEnv* env) {
    JniUtils::env = env;
}

std::shared_ptr<JniFieldInfo> JniUtils::getStaticFieldInfo(const std::string& className, const std::string& fieldName, const char* signature) {
    jclass clazz = env->FindClass(className.c_str());
    checkException();
    if (clazz == nullptr) {
        throw JniException(std::string("Could not find the given class: ") + className);
    }

    jfieldID fieldId = env->GetStaticFieldID(clazz, fieldName.c_str(), signature);
    checkException();
    if (fieldId == nullptr) {
        throw JniException(std::string("Could not find the given '") + fieldName + std::string("' static field in the given '") + className + std::string("' class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniFieldInfo>(clazz, fieldId);
}

std::shared_ptr<JniFieldInfo> JniUtils::getFieldInfo(jobject instance, const std::string& fieldName, const char* signature) {
    jclass clazz = env->GetObjectClass(instance);
    checkException();
    if (clazz == nullptr) {
        throw JniException("Could not find instance class.");
    }

    jfieldID fieldId = env->GetFieldID(clazz, fieldName.c_str(), signature);
    checkException();
    if (fieldId == nullptr) {
        throw JniException(std::string("Could not find the given '") + fieldName + std::string("' field in instance class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniFieldInfo>(clazz, fieldId);
}

std::shared_ptr<JniMethodInfo> JniUtils::getStaticMethodInfo(const std::string& className, const std::string& methodName, const char* signature) {
    jclass clazz = env->FindClass(className.c_str());
    checkException();
    if (clazz == nullptr) {
        throw JniException(std::string("Could not find the given class: ") + className);
    }

    jmethodID methodId = env->GetStaticMethodID(clazz, methodName.c_str(), signature);
    checkException();
    if (methodId == nullptr) {
        throw JniException(std::string("Could not find the given '") + methodName + std::string("' static method in the given '") + className + std::string("' class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniMethodInfo>(clazz, methodId);
}

std::shared_ptr<JniMethodInfo> JniUtils::getMethodInfo(jobject instance, const std::string& methodName, const char* signature) {
    jclass clazz = env->GetObjectClass(instance);
    checkException();
    if (clazz == nullptr) {
        throw JniException("Could not find instance class.");
    }

    jmethodID methodId = env->GetMethodID(clazz, methodName.c_str(), signature);
    checkException();
    if (methodId == nullptr) {
        throw JniException(std::string("Could not find the given '") + methodName + std::string("' method in instance class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniMethodInfo>(clazz, methodId);
}

std::shared_ptr<JniMethodInfo> JniUtils::getConstructorInfo(const std::string& className, const char* signature) {
    jclass clazz = env->FindClass(className.c_str());
    checkException();
    if (clazz == nullptr) {
        throw JniException(std::string("Could not find the given class: ") + className);
    }

    jmethodID methodId = env->GetMethodID(clazz, "<init>", signature);
    checkException();
    if (methodId == nullptr) {
        throw JniException(std::string("Could not find the constructor method in the given '") + className + std::string("' class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniMethodInfo>(clazz, methodId);
}

void JniUtils::checkException() {
    if (env->ExceptionCheck()) {
        jthrowable jthrowable = env->ExceptionOccurred();
        env->ExceptionDescribe();
        env->ExceptionClear();
        jclass clazz = env->FindClass("java/lang/Throwable");
        jmethodID methodId = env->GetMethodID(clazz, "getMessage", "()Ljava/lang/String;");
        // auto&& methodInfo = getMethodInfo("java/lang/Throwable", "getMessage", "()Ljava/lang/String;");
        std::string exceptionMessage = JniToCppConverter::toString(reinterpret_cast<jstring>(env->CallObjectMethod(jthrowable, methodId)));
        env->DeleteLocalRef(clazz);
        throw new JniException(exceptionMessage);
    }
}
namespace_ee_end
