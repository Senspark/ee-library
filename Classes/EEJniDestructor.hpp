//
//  EEJniParamDestructor.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_DESTRUCTOR_HPP
#define EE_LIBRARY_JNI_DESTRUCTOR_HPP

#include "EEMacro.hpp"
#include "EEJniUtils.hpp"

namespace_ee_begin
// Helper object to destroy parameters converter to JNI.
template<uint8_t ParamCount>
class JniParamDestructor final {
public:
    JniParamDestructor(JNIEnv* env)
    : _env(env)
    , _currentIndex(0)
    {}
    
    void add(jobject jniObject) {
        _params[_currentIndex++] = jniObject;
    }
    
    ~JniParamDestructor() {
        for (int i = 0; i < ParamCount; ++i) {
            if (_params[i] != nullptr) {
                _env->DeleteLocalRef(_params[i]);
            }
        }
        JniUtils::checkException();
    }
    
private:
    JNIEnv* _env;
    jobject _params[ParamCount] = { 0 };
    int _currentIndex;
};

// Optimized base case for the destructor.
template<>
struct JniParamDestructor<0> final {
    JniParamDestructor(JNIEnv* env) {}
    
    ~JniParamDestructor() {
        JniUtils::checkException();
    }
};

// Helper template to decide when a jni type must be destroyed (by adding the ref to the JNIParamDestructor struct).
template<class T, class D>
struct JniDestructorDecider {
    // By default the template ignores destruction (for primitive types)
    static void decide(T jniParam, D& destructor) {
        static_assert(std::is_same<T, int8_t>::value ||
                      std::is_same<T, uint8_t>::value ||
                      std::is_same<T, int32_t>::value ||
                      std::is_same<T, uint32_t>::value ||
                      std::is_same<T, int64_t>::value ||
                      std::is_same<T, uint64_t>::value,
                      "Wrong typename");
    }
};

template<class D>
struct JniDestructorDecider<jobject, D> {
    static void decide(jobject obj, D& destructor) {
        destructor.add(obj);
    }
};

template<class D>
struct JniDestructorDecider<jbyteArray, D> {
    static void decide(jbyteArray obj, D& destructor) {
        destructor.add((jobject) obj);
    }
};

template<class D>
struct JniDestructorDecider<jobjectArray, D> {
    static void decide(jobjectArray obj, D& destructor) {
        destructor.add((jobject) obj);
    }
};

template<class D>
struct JniDestructorDecider<jstring, D> {
    static void decide(jstring obj, D& destructor) {
        destructor.add((jobject) obj);
    }
};

// JNI param conversor helper: Converts the parameter to JNI and adds it to the destructor if needed.
template<class T, class D>
auto jniParamConverter(const T& arg, D& destructor) -> decltype(CppToJniConverter<T>::convert(arg)) {
    auto result = CppToJniConverter<T>::convert(arg);
    JniDestructorDecider<decltype(result), D>::decide(result, destructor);
    return result;
}
namespace_ee_end

#endif /* EE_LIBRARY_JNI_DESTRUCTOR_HPP */
