//
//  EEJniMethodInfo.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_METHOD_INFO_HPP
#define EE_LIBRARY_JNI_METHOD_INFO_HPP

#include "EEMacro.hpp"
#include "jni.h"

#include <memory>

namespace_ee_begin
class JniMethodInfo final {
public:
    JniMethodInfo(jclass clazz, jmethodID methodId);
    ~JniMethodInfo();
    
    jclass getClass() const;
    jmethodID getMethodId() const;
    
private:
    jclass _clazz;
    jmethodID _methodId;
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_METHOD_INFO_HPP */
