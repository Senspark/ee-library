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

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace_ee_begin
class JniMethodInfo;
class JniFieldInfo;

/**
 * JNI Utils class.
 */
class JniUtils final {
public:
    /// Initializes JNI.
    static void init(JavaVM* vm, JNIEnv* env);

    static JNIEnv* getJNIEnv() {
        return env;
    }
    
    static void setJniEnv(JNIEnv* env);
    
    static JNIEnv* getJNIEnvAttach();    
    
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
