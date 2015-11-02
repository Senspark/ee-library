//
//  EEJniObject.hpp
//  ee-library
//
//  Created by enrevol on 11/1/15.
//
//

#ifndef EE_LIBRARY_JNI_OBJECT_HPP
#define EE_LIBRARY_JNI_OBJECT_HPP

#include "EEMacro.hpp"
#include "EEJni.hpp"
#include "EEJniConverter.hpp"
#include "EEJniDestructor.hpp"

#include <memory>
#include <string>

namespace_ee_begin
/// jobject global ref wrapper class.
class JniObject final {
public:
    template<class... Args>
    static std::shared_ptr<JniObject> create(const std::string& className, Args&&... args) {
        static constexpr uint8_t Arity = sizeof...(Args);
        JNIEnv* env = JniUtils::getJNIEnv();
        auto&& methodInfo = JniUtils::getConstructorInfo(className, getJniSignature<void, Args...>());
        JniParamDestructor<Arity> paramDestructor(env);
        jobject instance = env->NewObject(methodInfo->getClass(), methodInfo->getMethodId(), jniParamConverter<Args>(args, paramDestructor)...);
        instance = env->NewGlobalRef(instance);
        return std::make_shared<JniObject>(instance, className);
    }
    
    JniObject(jobject instance, std::string className);
    
    ~JniObject();
    
    template<class T = void, class... Args>
    T call(const std::string& methodName, Args&&... args) const {
        return ee::call<T, Args...>(_instance, _className, methodName, std::forward<Args>(args)...);
    }
    
    jobject getInstance() const;
    
    const std::string& getClassName() const;
    
private:
    std::string _className;
    jobject _instance;
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_OBJECT_HPP */
