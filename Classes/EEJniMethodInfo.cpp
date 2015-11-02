//
//  EEJniMethodInfo.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "EEJniMethodInfo.hpp"
#include "EEJniUtils.hpp"

namespace_ee_begin
JniMethodInfo::JniMethodInfo(jclass clazz, jmethodID methodId)
: _clazz(clazz)
, _methodId(methodId)
{}

JniMethodInfo::~JniMethodInfo() {
    if (_clazz != nullptr) {
        JniUtils::getJNIEnv()->DeleteLocalRef(_clazz);
    }
}

jclass JniMethodInfo::getClass() const {
    return _clazz;
}

jmethodID JniMethodInfo::getMethodId() const {
    return _methodId;
}
namespace_ee_end
