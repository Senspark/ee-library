//
//  EEJni.cpp
//  ee-library
//
//  Created by enrevol on 11/1/15.
//
//

#include "EEJni.hpp"

namespace_ee_begin
void setStaticField(const std::string& className, const std::string& fieldName, const char* signature, jobject value) {
    EE_LOGD("setStaticField: className = %s fieldName = %s", className.c_str(), fieldName.c_str());
    JNIEnv* env = JniUtils::getJNIEnvAttach();
    auto&& fieldInfo = JniUtils::getStaticFieldInfo(className, fieldName, signature);
    JniCaller<jobject>::setStaticField(env, fieldInfo->getClass(), fieldInfo->getFieldId(), value);
}
namespace_ee_end
