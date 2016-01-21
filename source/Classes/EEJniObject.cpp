//
//  EEJniObject.cpp
//  ee-library
//
//  Created by enrevol on 11/1/15.
//
//

#include "EEJniObject.hpp"

namespace_ee_begin
JniObject::JniObject(jobject instance, std::string className)
: _instance(instance)
, _className(std::move(className))
{}

JniObject::~JniObject() {
    if (_instance != nullptr) {
        JniUtils::getJNIEnv()->DeleteGlobalRef(_instance);
    }
}

jobject JniObject::getInstance() const {
    return _instance;
}

const std::string& JniObject::getClassName() const {
    return _className;
}
namespace_ee_end