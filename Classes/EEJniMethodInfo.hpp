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
    jclass classId;
    jmethodID methodId;
    
    JniMethodInfo(jclass classId, jmethodID methodId);
    ~JniMethodInfo();
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_METHOD_INFO_HPP */
