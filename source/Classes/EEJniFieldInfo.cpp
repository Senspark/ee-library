//
//  EEJniFieldInfo.cpp
//  ee-library
//
//  Created by enrevol on 11/1/15.
//
//

#include "EEJniFieldInfo.hpp"
#include "EEJniUtils.hpp"

namespace_ee_begin
JniFieldInfo::JniFieldInfo(jclass clazz, jfieldID fieldId)
: _clazz(clazz)
, _fieldId(fieldId)
{}

JniFieldInfo::~JniFieldInfo() {
    if (_clazz != nullptr) {
        JniUtils::getJNIEnv()->DeleteLocalRef(_clazz);
    }
}

jclass JniFieldInfo::getClass() const {
    return _clazz;
}

jfieldID JniFieldInfo::getFieldId() const {
    return _fieldId;
}
namespace_ee_end
