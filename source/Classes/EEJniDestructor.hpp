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

/// JNI param conversor helper: Converts the parameter to JNI and adds it to the destructor if needed.
class JniParam final {
public:
    template<class T, class D>
    static auto convert(const T& arg, D& destructor) {
        auto result = detail::CppToJni<T>::convert(arg);
        decide(result, destructor);
        return result;
    }
    
private:
    /// Helper template to decide when a jni type must be destroyed (by adding the ref to the JNIParamDestructor struct).
    template<class T, class D>
    inline static void decide(T jniParam, D& destructor) {
        constexpr bool NeedToDestroy = (std::is_same<T, jstring>::value ||
                                        std::is_same<T, jobject>::value ||
                                        std::is_same<T, jbyteArray>::value ||
                                        std::is_same<T, jbooleanArray>::value ||
                                        std::is_same<T, jshortArray>::value ||
                                        std::is_same<T, jcharArray>::value ||
                                        std::is_same<T, jintArray>::value ||
                                        std::is_same<T, jlongArray>::value ||
                                        std::is_same<T, jfloatArray>::value ||
                                        std::is_same<T, jdoubleArray>::value);
        internalDecide<T, D>(std::integral_constant<bool, NeedToDestroy>(), jniParam, destructor);
    }

    template<class T, class D>
    inline static void internalDecide(std::true_type, T jniParam, D& destructor) {
        destructor.add(jniParam);
    }

    template<class T, class D>
    inline static void internalDecide(std::false_type, T, D&) {
        static_assert(std::is_same<T, int8_t>::value ||
                      std::is_same<T, uint8_t>::value ||
                      std::is_same<T, int16_t>::value ||
                      std::is_same<T, uint16_t>::value ||
                      std::is_same<T, int32_t>::value ||
                      std::is_same<T, uint32_t>::value ||
                      std::is_same<T, int64_t>::value ||
                      std::is_same<T, uint64_t>::value,
                      "Wrong typename");
    }
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_DESTRUCTOR_HPP */
