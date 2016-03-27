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
    static T call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        auto obj = env->CallObjectMethod(instance, methodId, args...);
        T result = detail::JniToCpp<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static T callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        auto obj = env->CallStaticObjectMethod(clazz, methodId, args...);
        T result = detail::JniToCpp<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static T getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        auto obj = env->GetObjectField(instance, fieldId);
        T result = detail::JniToCpp<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static T getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        auto obj = env->GetStaticObjectField(clazz, fieldId);
        T result = detail::JniToCpp<T>::convert(obj);
        if (obj != nullptr) {
            env->DeleteLocalRef(obj);
        }
        return result;
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jobject value) {
        env->SetObjectField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jobject value) {
        env->SetStaticObjectField(clazz, fieldId, value);
    }
};

// Generic pointer implementation (using jlong types).
template<class T, class... Args>
struct JniCaller<T*, Args...> {
    static T* call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return (T*) env->CallLongMethod(instance, methodId, args...);
    }

    static T* callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return (T*) env->CallLongMethod(clazz, methodId, args...);
    }

    static T* getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return (T*) env->GetLongField(instance, fieldId);
    }

    static T* getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return (T*) env->GetStaticLongField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jlong value) {
        env->SetLongField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jlong value) {
        env->SetStaticLongField(clazz, fieldId, value);
    }
};

template<>
struct JniCaller<jobject> {
    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jobject value) {
        env->SetObjectField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jobject value) {
        env->SetStaticObjectField(clazz, fieldId, value);
    }
};

// Void implementation.
template<class... Args>
struct JniCaller<void, Args...> {
    static void call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
            env->CallVoidMethod(instance, methodId, args...);
        }

    static void callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        env->CallStaticVoidMethod(clazz, methodId, args...);
    }
};

// Primitive types implementations.
template<class... Args>
struct JniCaller<bool, Args...> {
    static bool call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallBooleanMethod(instance, methodId, args...);
    }

    static bool callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticBooleanMethod(clazz, methodId, args...);
    }

    static bool getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetBooleanField(instance, fieldId);
    }

    static bool getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticBooleanField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jboolean value) {
        env->SetBooleanField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jboolean value) {
        env->SetStaticBooleanField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<int8_t, Args...> {
    static int8_t call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallByteMethod(instance, methodId, args...);
    }

    static int8_t callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticByteMethod(clazz, methodId, args...);
    }

    static int8_t getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetByteField(instance, fieldId);
    }

    static int8_t getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticByteField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jbyte value) {
        env->SetByteField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jbyte value) {
        env->SetStaticByteField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<uint8_t, Args...> {
    static uint8_t call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallCharMethod(instance, methodId, args...);
    }

    static uint8_t callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticCharMethod(clazz, methodId, args...);
    }

    static uint8_t getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetCharField(instance, fieldId);
    }

    static uint8_t getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticCharField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jchar value) {
        env->SetCharField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jchar value) {
        env->SetStaticCharField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<int16_t, Args...> {
    static int16_t call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallShortMethod(instance, methodId, args...);
    }

    static int16_t callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticShortMethod(clazz, methodId, args...);
    }

    static int16_t getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetShortField(instance, fieldId);
    }

    static int16_t getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticShortField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jshort value) {
        env->SetShortField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jshort value) {
        env->SetStaticShortField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<int32_t, Args...> {
    static int32_t call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallIntMethod(instance, methodId, args...);
    }

    static int32_t callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticIntMethod(clazz, methodId, args...);
    }

    static int32_t getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetIntField(instance, fieldId);
    }

    static int32_t getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticIntField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jint value) {
        env->SetIntField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jint value) {
        env->SetStaticIntField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<int64_t, Args...> {
    static int64_t call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallLongMethod(instance, methodId, args...);
    }

    static int64_t callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticLongMethod(clazz, methodId, args...);
    }

    static int64_t getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetLongField(instance, fieldId);
    }

    static int64_t getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticLongField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jlong value) {
        env->SetLongField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jlong value) {
        env->SetStaticLongField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<float, Args...> {
    static float call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallFloatMethod(instance, methodId, args...);
    }

    static float callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticFloatMethod(clazz, methodId, args...);
    }

    static float getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetFloatField(instance, fieldId);
    }

    static float getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticFloatField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jfloat value) {
        env->SetFloatField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jfloat value) {
        env->SetStaticFloatField(clazz, fieldId, value);
    }
};

template<class... Args>
struct JniCaller<double, Args...> {
    static double call(JNIEnv* env, jobject instance, jmethodID methodId, Args... args) {
        return env->CallDoubleMethod(instance, methodId, args...);
    }

    static double callStatic(JNIEnv* env, jclass clazz, jmethodID methodId, Args... args) {
        return env->CallStaticDoubleMethod(clazz, methodId, args...);
    }

    static double getField(JNIEnv* env, jobject instance, jfieldID fieldId) {
        return env->GetDoubleField(instance, fieldId);
    }

    static double getStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId) {
        return env->GetStaticDoubleField(clazz, fieldId);
    }

    static void setField(JNIEnv* env, jobject instance, jfieldID fieldId, jdouble value) {
        env->SetDoubleField(instance, fieldId, value);
    }

    static void setStaticField(JNIEnv* env, jclass clazz, jfieldID fieldId, jdouble value) {
        env->SetStaticDoubleField(clazz, fieldId, value);
    }
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_CALLER_HPP */
