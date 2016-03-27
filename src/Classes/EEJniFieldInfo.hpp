//
//  EEJniFieldInfo.hpp
//  ee-library
//
//  Created by enrevol on 11/1/15.
//
//

#ifndef EE_LIBRARY_JNI_FIELD_INFO_HPP
#define EE_LIBRARY_JNI_FIELD_INFO_HPP

#include "EEMacro.hpp"
#include "jni.h"

#include <memory>

namespace_ee_begin
class JniFieldInfo final {
public:
    JniFieldInfo(jclass clazz, jfieldID fieldId);
    ~JniFieldInfo();
    
    jclass getClass() const;
    jfieldID getFieldId() const;
    
private:
    jclass _clazz;
    jfieldID _fieldId;
};
namespace_ee_end

#endif /* EEJniFieldInfo_hpp */
