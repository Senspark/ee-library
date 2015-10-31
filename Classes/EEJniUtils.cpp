//
//  EEJniUtils.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "EEJniUtils.hpp"
#include "EEJniException.hpp"
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
        int status = javaVM->AttachCurrentThread(&env, NULL);
        if (status < 0) {
            throw JniException("Could not attach the JNI environment to the current thread.");
        }
    }
    return env;
}

jstring JniUtils::toJString(const char* str) {
    return env->NewStringUTF(str);
}

jobjectArray JniUtils::toJObjectArray(const std::vector<std::string>& data) {
    jclass classId = env->FindClass("java/lang/String");
    jint size = data.size();
    jobjectArray joa = env->NewObjectArray(size, classId, 0);

    for (int i = 0; i < size; i++) {
        jstring jstr = toJString(data[i]);
        env->SetObjectArrayElement(joa, i, jstr);
    }
    env->DeleteLocalRef(classId);

    checkException();
    return joa;
}

jbyteArray JniUtils::toJObjectArray(const std::vector<uint8_t>& data) {
    jbyteArray jba = env->NewByteArray(data.size());
    env->SetByteArrayRegion(jba, 0, data.size(), (const jbyte*) &data[0]);
    checkException();
    return jba;
}

std::string JniUtils::toString(jstring str) {
    if (str == nullptr) {
        return std::string();
    }
    jboolean isCopy;
    const char* chars = env->GetStringUTFChars(str, &isCopy);
    std::string s = chars;
    env->ReleaseStringUTFChars(str, chars);
    checkException();
    return s;
}

std::vector<std::string> JniUtils::toVectorString(jobjectArray array) {
    std::vector<std::string> result;
    if (array != nullptr) {
        jsize size = env->GetArrayLength(array);
        result.reserve(size);
        for (int i = 0; i < size; i++) {
            jobject valueJObject = env->GetObjectArrayElement(array, i);
            result.push_back(toString((jstring) valueJObject));
            env->DeleteLocalRef(valueJObject);
        }
    }
    return result;
}

std::vector<uint8_t> JniUtils::toVectorByte(jbyteArray array) {
    if (array == nullptr) {
        return std::vector<uint8_t>();
    }
    jsize size = env->GetArrayLength(array);
    std::vector<uint8_t> result(size);
    env->GetByteArrayRegion(array, 0, size, (jbyte*) &result[0]);
    checkException();
    return result;
}

std::vector<jobject> JniUtils::toVectorJObject(jobjectArray array) {
    std::vector<jobject> result;
    if (array != nullptr) {
        jsize size = env->GetArrayLength(array);
        result.reserve(size);
        for (int i = 0; i < size; i++) {
            jobject valueJObject = env->GetObjectArrayElement(array, i);
            result.push_back(valueJObject);
        }
    }
    return result;
}

std::shared_ptr<JniMethodInfo> JniUtils::getStaticMethodInfo(const std::string& className, const std::string& methodName, const char* signature) {
    jclass classId = nullptr;
    jmethodID methodId = nullptr;

    classId = env->FindClass(className.c_str());
    checkException();

    if (classId == nullptr) {
        throw JniException(std::string("Could not find the given class: ") + className);
    }

    methodId = env->GetStaticMethodID(classId, methodName.c_str(), signature);
    checkException();

    if (methodId == nullptr) {
        throw JniException(std::string("Could not find the given '") + methodName + std::string("' static method in the given '") + className + std::string("' class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniMethodInfo>(JniMethodInfo(classId, methodId));
}

std::shared_ptr<JniMethodInfo> JniUtils::getMethodInfo(const std::string& className, const std::string& methodName, const char* signature) {
    jclass classId = nullptr;
    jmethodID methodId = nullptr;
    classId = env->FindClass(className.c_str());

    checkException();
    if (classId == nullptr) {
        throw JniException(std::string("Could not find the given class: ") + className);
    }

    methodId = env->GetMethodID(classId, methodName.c_str(), signature);
    checkException();

    if (methodId == nullptr) {
        throw JniException(std::string("Could not find the given '") + methodName + std::string("' static method in the given '") + className + std::string("' class using the '") + signature + std::string("' signature."));
    }

    return std::make_shared<JniMethodInfo>(JniMethodInfo(classId, methodId));
}

void JniUtils::checkException() {
    if (env->ExceptionCheck()) {
        jthrowable jthrowable = env->ExceptionOccurred();
        env->ExceptionDescribe();
        env->ExceptionClear();
        auto&& methodInfo = getMethodInfo("java/lang/Throwable", "getMessage", "()Ljava/lang/String;");
        std::string exceptionMessage = toString(reinterpret_cast<jstring>(env->CallObjectMethod(jthrowable, methodInfo->methodId)));
        throw new JniException(exceptionMessage);
    }
}
namespace_ee_end
