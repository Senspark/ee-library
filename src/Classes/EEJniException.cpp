//
//  JniException.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "EEJniException.hpp"

namespace_ee_begin
JniException::JniException(const char* message)
: _message(message) {
    EE_LOGE("JNI Exception: %s", message);
}

JniException::JniException(const std::string& message)
: _message(message) {
    EE_LOGE("JNI Exception: %s", message.c_str());
}

const char* JniException::what() const throw () {
    return _message.c_str();
}
namespace_ee_end
