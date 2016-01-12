//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by enrevol on 11/7/15.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP
#define EE_LIBRARY_DATA_INFO_HPP

#include "EEMacro.hpp"
#include "EENonCopyable.hpp"
#include "EEUtils.hpp"

#include <string>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#   include "EEFunctionTraits.hpp"
#   include "EEJniBoxedType.hpp"
#   include "EEJniObject.hpp"
#   include "EEJniUtils.hpp"
#   include "EEJniConverter.hpp"
#endif

namespace_ee_begin
template<class T>
void setInfoHelper(const char* key, const T& value);

template<class T>
T getInfoHelper(const char* key, const T& defaultValue);

namespace_detail_begin
/// Single argument.
template<class T>
std::string createKey(T&& value) {
    return toString("___", std::forward<T>(value));
}

/// Multiple arguments.
template<class T, class... Args>
std::string createKey(T&& value, Args&&... args) {
    return toString("___", std::forward<T>(value), createKey(std::forward<Args>(args)...));
}

class DataInfoBase : public NonCopyable {
public:
    const std::string& getKey() const;

protected:
    DataInfoBase(std::string key) : _key(std::move(key)) {
        
    }
    
    DataInfoBase(DataInfoBase&&) = default;
    DataInfoBase& operator=(DataInfoBase&&) = delete;
    
    const std::string _key;
};

class JniDataInfoBase {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
public:
    /// Initializer.
    void initialize(std::string className) const;
    void initialize(std::string className, std::string fieldName) const;
    
protected:
    friend class JniDataInfoManager;
    
    /// Getter.
    virtual jobject jniGet(jobjectArray objects) const = 0;
    
    /// Setter.
    virtual void jniSet(jobjectArray objects) const = 0;
    
    virtual void internalInitialize(std::string className) const = 0;
    virtual void internalInitialize(std::string className, std::string fieldName) const = 0;
    
    static const char* ClassNames[3];
    static const char* ClassSignatures[3];
#endif
};

namespace_detail_end

template<class... Args>
class DataInfo;

// Zero format argument.
template<class T>
class DataInfo<T> : public detail::DataInfoBase {
public:
    using ValueType = typename std::decay<T>::type;
    using FormatType = std::tuple<>;
    
    using FormatArgs [[deprecated("Use FormatType instead.")]] = FormatType;
    
    /**
     * Constructor.
     * @param key Unique key to store in user defaults.
     * @param defaultValue The default value in user defaults.
     */
    explicit DataInfo(std::string key, T defaultValue = T())
    : DataInfoBase(std::move(key))
    , _defaultValue(std::move(defaultValue)) {}
    
    /**
     * Setter.
     * @param value The desired value to be set.
     */
    void set(T value) const {
        setInfoHelper<ValueType>(_key.c_str(), std::forward<T>(value));
    }
    
    /**
     * Getter.
     * @return Retrieved value.
     */
    ValueType get() const {
        return getInfoHelper<ValueType>(_key.c_str(), _defaultValue);
    }
    
    /**
     * Combination of getter and setter.
     */
    const DataInfo<T>& add(T value) const {
        set(get() + std::forward<T>(value));
        return *this;
    }
    
    /**
     * Implicit conversion.
     */
    operator ValueType() const {
        return get();
    }
    
    const DataInfo<T>& operator=(T value) const {
        set(std::forward<T>(value));
        return *this;
    }
    
    const DataInfo<T>& operator+=(T value) const {
        add(std::forward<T>(value));
        return *this;
    }
    
    const DataInfo<T>& operator-=(T value) const {
        add(-value);
        return *this;
    }
    
    ValueType operator++(int) const {
        auto ret = get();
        set(ret + 1);
        return ret;
    }
    
    ValueType operator--(int) const {
        auto ret = get();
        set(ret - 1);
        return ret;
    }
    
    const DataInfo<T>& operator++() const {
        add(+1);
        return *this;
    }
    
    const DataInfo<T>& operator--() const {
        add(-1);
        return *this;
    }
    
private:
    const T _defaultValue;
};

// Multiple format arguments.
template<class T, class... Args>
class DataInfo<T, Args...> : public detail::DataInfoBase {
public:
    using ValueType = typename std::decay<T>::type;
    using FormatType = std::tuple<Args...>;
    
    using FormatArgs [[deprecated("Use FormatType instead.")]] = FormatType;
    
    explicit DataInfo(std::string key, T defaultValue = T())
    : DataInfoBase(std::move(key))
    , _defaultValue(std::move(defaultValue)) {}
    
    void set(T value, Args... args) const {
        setInfoHelper<T>(toString(_key, detail::createKey(std::forward<Args>(args)...)).c_str(), std::forward<T>(value));
    }
    
    ValueType get(Args... args) const {
        return getInfoHelper<ValueType>(toString(_key, detail::createKey(std::forward<Args>(args)...)).c_str(), _defaultValue);
    }
    
    void add(T value, Args... args) const {
        auto&& current = get(args...);
        set(current + std::forward<T>(value), std::forward<Args>(args)...);
    }
    
    DataInfo<T> at(Args... args) const {
        return DataInfo<T>(toString(_key, detail::createKey(std::forward<Args>(args)...)).c_str(), _defaultValue);
    }
    
private:
    const T _defaultValue;
};

template<class... Args>
class JniDataInfo;

template<class T>
class JniDataInfo<T> : public DataInfo<T>, public detail::JniDataInfoBase {
public:
    using typename DataInfo<T>::ValueType;
    using typename DataInfo<T>::FormatType;
    
    using DataInfo<T>::DataInfo;

    using DataInfo<T>::get;
    using DataInfo<T>::set;
    using DataInfo<T>::operator=;
    
protected:
    using DataInfo<T>::_key;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
protected:
    virtual jobject jniGet(jobjectArray objects) const override {
        auto&& convertedObjects = JniToCppConverter::toVectorObject(objects);
        CC_ASSERT(convertedObjects.size() == 0);
        auto&& value = get();
        EE_LOGD("JniDataInfo::get: key = %s value = %s", _key.c_str(), toString(value).c_str());
        jobject ret = detail::CppToJni<MakeBoxedTypeT<ValueType>>::convert(value);
        return ret;
    }
    
    virtual void jniSet(jobjectArray objects) const override {
        auto&& convertedObjects = JniToCppConverter::toVectorObject(objects);
        CC_ASSERT(convertedObjects.size() == 1);
        auto&& value = detail::JniToCpp<ValueType>::convert(convertedObjects.at(0));
        EE_LOGD("JniDataInfo::set: key = %s value = %s", _key.c_str(), toString(value).c_str());
        JniUtils::getJNIEnv()->DeleteLocalRef(convertedObjects.front());
        set(value);
    }
    
    virtual void internalInitialize(std::string className) const override {
        internalInitialize(std::move(className), _key);
    }
    
    virtual void internalInitialize(std::string className, std::string fieldName) const override {
        auto obj = JniObject::create(ClassNames[0], this);
        setStaticField(std::move(className), std::move(fieldName), ClassSignatures[0], obj->getInstance());
    }
#endif
};

template<class T, class... Args>
class JniDataInfo<T, Args...> : public DataInfo<T, Args...>, public detail::JniDataInfoBase {
public:
    using typename DataInfo<T, Args...>::ValueType;
    using typename DataInfo<T, Args...>::FormatType;
    
    using DataInfo<T, Args...>::DataInfo;

    using DataInfo<T, Args...>::get;
    using DataInfo<T, Args...>::set;
    
protected:
    using DataInfo<T, Args...>::_key;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
protected:
    virtual jobject jniGet(jobjectArray objects) const override {
        return internalGetter(objects, typename SequenceGenerator<sizeof...(Args)>::Type());
    }

    virtual void jniSet(jobjectArray objects) const override {
        internalSetter(objects, typename SequenceGenerator<sizeof...(Args)>::Type());
    }
    
    virtual void internalInitialize(std::string className) const override {
        internalInitialize(std::move(className), _key);
    }
    
    virtual void internalInitialize(std::string className, std::string fieldName) const override {
        constexpr int Arity = sizeof...(Args);
        auto obj = JniObject::create(ClassNames[Arity], this);
        setStaticField(std::move(className), std::move(fieldName), ClassSignatures[Arity], obj->getInstance());
    }
    
private:
    template<std::size_t... Indices>
    jobject internalGetter(jobjectArray objects, Sequence<Indices...>) const {
        auto&& convertedObjects = JniToCppConverter::toVectorObject(objects);
        CC_ASSERT(convertedObjects.size() == sizeof...(Indices));
        auto&& value = get(detail::JniToCpp<Args>::convert(convertedObjects.at(Indices))...);
        EE_LOGD("JniDataInfo::get: key = %s value = %s", _key.c_str(), toString(value).c_str());
        for (jobject obj : convertedObjects) {
            JniUtils::getJNIEnv()->DeleteLocalRef(obj);
        }
        jobject ret = detail::CppToJni<MakeBoxedTypeT<ValueType>>::convert(value);
        return ret;
    }
    
    template<std::size_t... Indices>
    void internalSetter(jobjectArray objects, Sequence<Indices...>) const {
        EE_LOGD("%s multi-args begin.", __PRETTY_FUNCTION__);
        auto&& convertedObjects = JniToCppConverter::toVectorObject(objects);
        CC_ASSERT(convertedObjects.size() == sizeof...(Indices) + 1);
        auto&& value = detail::JniToCpp<ValueType>::convert(convertedObjects.at(0));
        EE_LOGD("JniDataInfo::set: key = %s value = %s", _key.c_str(), toString(value).c_str());
        set(value, detail::JniToCpp<Args>::convert(convertedObjects.at(Indices + 1))...);
        for (jobject obj : convertedObjects) {
            JniUtils::getJNIEnv()->DeleteLocalRef(obj);
        }
        EE_LOGD("%s multi-args end.", __PRETTY_FUNCTION__);
    }
#endif
};

#define CREATE_DATA_INFO(...) GET_FUNCTION(CREATE_DATA_INFO_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define CREATE_DATA_INFO_1(variableName)                variableName(# variableName)
#define CREATE_DATA_INFO_2(variableName, defaultValue)  variableName(# variableName, defaultValue)
namespace_ee_end

#endif /* EE_LIBRARY_DATA_INFO_HPP */
