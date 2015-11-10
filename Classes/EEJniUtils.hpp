//
//  EEJniUtils.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_UTILS_HPP
#define EE_LIBRARY_JNI_UTILS_HPP

#include "EEMacro.hpp"
#include "jni.h"

#include <memory>
#include <string>
#include <vector>

namespace_ee_begin
class JniMethodInfo;
class JniFieldInfo;

class JniUtils final {
public:
    /// Initializes JNI.
    static void init(JavaVM* vm, JNIEnv* env);

    static JNIEnv* getJNIEnv() {
        return env;
    }
    
    static void setJniEnv(JNIEnv* env);
    
    static JNIEnv* getJNIEnvAttach();

    /// Converts const char* to String
    static jstring toJString(const char* str);

    /// Converts std::string to String.
    static jstring toJString(const std::string& str) {
        return toJString(str.c_str());
    }
    
    /// Converts std::vector<std::string> to String[].
    static jobjectArray toJObjectArray(const std::vector<std::string>& data);

    /// Converts std::vector<uint8_t> to byte[].
    static jbyteArray toJObjectArray(const std::vector<uint8_t>& data);

    /// Converts String to std::string.
    static std::string toString(jstring str);

    /// Converts Integer to int.
    static int toInt(jobject obj);

    /// Converts String[] to std::vector<std::string>.
    static std::vector<std::string> toVectorString(jobjectArray array);

    /// Converts byte[] to std::vector<uint8_t>.
    static std::vector<uint8_t> toVectorByte(jbyteArray array);
    static std::vector<jobject> toVectorJObject(jobjectArray array);
    
    static std::shared_ptr<JniFieldInfo> getStaticFieldInfo(const std::string& className, const std::string& fieldName, const char* signature);
    static std::shared_ptr<JniFieldInfo> getFieldInfo(jobject instance, const std::string& fieldName, const char* signature);

    static std::shared_ptr<JniMethodInfo> getStaticMethodInfo(const std::string& className, const std::string& methodName, const char* signature);
    static std::shared_ptr<JniMethodInfo> getMethodInfo(jobject instance, const std::string& methodName, const char* signature);

    static std::shared_ptr<JniMethodInfo> getConstructorInfo(const std::string& className, const char* signature);

    static void checkException();

private:
    static JNIEnv* env;
    static JavaVM* javaVM;
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_UTILS_HPP */
