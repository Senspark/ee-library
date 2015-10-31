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
JniMethodInfo::JniMethodInfo(jclass classId, jmethodID methodId)
: classId(classId)
, methodId(methodId)
{}

JniMethodInfo::~JniMethodInfo() {
    if (classId != nullptr) {
        JniUtils::getJNIEnv()->DeleteLocalRef(classId);
    }
}
namespace_ee_end
