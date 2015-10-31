//
//  EEJniCaller.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_CALLER_HPP
#define EE_LIBRARY_JNI_CALLER_HPP

#include "EEMacro.hpp"
#include "EEJniConverter.hpp"

namespace_ee_begin
// Default implementation (for jobject types).
template<class T, class... Args>
struct JniCaller {
    static T callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        auto obj = env->CallStaticObjectMethod(cls, method, std::forward<Args>(args)...);
        T result = JniToCppConverter<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static T callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        auto obj = env->CallObjectMethod(instance, method, std::forward<Args>(args)...);
        T result = JniToCppConverter<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static T getField(JNIEnv* env, jobject instance, jfieldID fid) {
        auto obj = env->GetObjectField(instance, fid);
        T result = JniToCppConverter<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }
};

// Generic pointer implementation (using jlong types).
template<class T, class... Args>
struct JniCaller<T*, Args...> {
    static T* callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return (T*) env->CallLongMethod(cls, method, std::forward<Args>(args)...);
    }

    static T* callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return (T*) env->CallLongMethod(instance, method, std::forward<Args>(args)...);
    }

    static T* getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return (T*) env->GetLongField(instance, fid);
    }
};

// Void implementation.
template<class... Args>
struct JniCaller<void, Args...> {
    static void callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        env->CallStaticVoidMethod(cls, method, std::forward<Args>(args)...);
    }

    static void callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        env->CallVoidMethod(instance, method, std::forward<Args>(args)...);
    }
};

// Primitive types implementations.
template<class... Args>
struct JniCaller<bool, Args...> {
    static bool callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticBooleanMethod(cls, method, std::forward<Args>(args)...);
    }

    static bool callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallBooleanMethod(instance, method, std::forward<Args>(args)...);
    }

    static bool getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetBooleanField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<int8_t, Args...> {
    static int8_t callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticByteMethod(cls, method, std::forward<Args>(args)...);
    }

    static int8_t callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallByteMethod(instance, method, std::forward<Args>(args)...);
    }

    static int8_t getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetByteField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<uint8_t, Args...> {
    static uint8_t callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticCharMethod(cls, method, std::forward<Args>(args)...);
    }

    static uint8_t callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallCharMethod(instance, method, std::forward<Args>(args)...);
    }

    static uint8_t getField(JNIEnv * env, jobject instance, jfieldID fid) {
        return env->GetCharField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<int16_t, Args...> {
    static int16_t callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticShortMethod(cls, method, std::forward<Args>(args)...);
    }

    static int16_t callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallShortMethod(instance, method, std::forward<Args>(args)...);
    }

    static int16_t getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetShortField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<int32_t, Args...> {
    static int32_t callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticIntMethod(cls, method, std::forward<Args>(args)...);
    }

    static int32_t callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallIntMethod(instance, method, std::forward<Args>(args)...);
    }

    static int32_t getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetIntField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<int64_t, Args...> {
    static int64_t callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticLongMethod(cls, method, std::forward<Args>(args)...);
    }

    static int64_t callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallLongMethod(instance, method, std::forward<Args>(args)...);
    }

    static int64_t getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetLongField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<float, Args...> {
    static float callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticFloatMethod(cls, method, std::forward<Args>(args)...);
    }

    static float callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallFloatMethod(instance, method, std::forward<Args>(args)...);
    }

    static float getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetFloatField(instance, fid);
    }
};

template<class... Args>
struct JniCaller<double, Args...> {
    static double callStatic(JNIEnv* env, jclass cls, jmethodID method, Args&&... args) {
        return env->CallStaticDoubleMethod(cls, method, std::forward<Args>(args)...);
    }

    static double callInstance(JNIEnv* env, jobject instance, jmethodID method, Args&&... args) {
        return env->CallDoubleMethod(instance, method, std::forward<Args>(args)...);
    }

    static double getField(JNIEnv* env, jobject instance, jfieldID fid) {
        return env->GetDoubleField(instance, fid);
    }
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_CALLER_HPP */
