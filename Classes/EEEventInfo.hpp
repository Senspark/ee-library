//
//  EEEvent.h
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#ifndef EE_LIBRARY_EVENT_H
#define EE_LIBRARY_EVENT_H

#include "EEForward.hpp"
#include "EEFunctionTraits.hpp"
#include "EEMacro.hpp"
#include "EEUtils.hpp"
#include "EENonCopyable.hpp"
#include "EENonMovable.hpp"
#include "base/CCEventCustom.h"

#include <functional>
#include <memory>
#include <type_traits>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#   include "EEJniObject.hpp"
#   include "EEJniConverter.hpp"
#endif

namespace_ee_begin
namespace_detail_begin
class EventInfoBase : public NonCopyable, public NonMovable {
public:
    void removeListeners() const;

    const std::string& getKey() const;
    
protected:
    EventInfoBase(std::string key) : _key(std::move(key)) {}
    
    cocos2d::EventListenerCustom* addListenerHelper(const std::function<void(cocos2d::EventCustom*)>& callback) const;
    
    void dispatchHelper(void* data) const;
    
    const std::string _key;
};

class JniEventInfoBase {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
public:
    /// Initializer.
    void initialize(std::string className) const;
    void initialize(std::string className, std::string fieldName) const;
    
protected:
    friend class JniEventInfoManager;
    
    /// Dispatcher.
    virtual void jniDispatch(jobjectArray objects) const = 0;
    
    virtual void internalInitialize(std::string className) const = 0;
    virtual void internalInitialize(std::string className, std::string fieldName) const = 0;
    
    static const char* ClassNames[3];
    static const char* ClassSignatures[3];
#endif
};
namespace_detail_end

// Multiple arguments.
template<class... Args>
class EventInfo : public detail::EventInfoBase {
public:
    using DataType = std::tuple<Args...>;
    
    EventInfo(std::string key) : EventInfoBase(std::move(key)) {}
    
    cocos2d::EventListenerCustom* addListener(const std::function<void(Args...)>& callback) const {
        return addListenerHelper([callback, this](cocos2d::EventCustom* event) {
            auto&& data = *static_cast<const DataType*>(event->getUserData());
            unpackTuple(typename SequenceGenerator<sizeof...(Args)>::Type(), data, callback);
        });
    }
    
    void dispatch(Args... args) const {
        auto&& data = DataType(std::forward<Args>(args)...);
        dispatchHelper(&data);
    }
    
private:
    template<std::size_t... Indices>
    void unpackTuple(Sequence<Indices...>, const DataType& data, const std::function<void(Args...)>& callback) const {
        callback(std::get<Indices>(data)...);
    }
};

// Zero argument.
template<>
class EventInfo<> : public detail::EventInfoBase {
public:
    using DataType = std::tuple<>;
    
    EventInfo(std::string key) : EventInfoBase(std::move(key)) {}
    
    cocos2d::EventListenerCustom* addListener(const std::function<void()>& callback) const {
        return addListenerHelper([callback](cocos2d::EventCustom*) {
            callback();
        });
    }
    
    void dispatch() const {
        dispatchHelper(nullptr);
    }
};

template<class... Args>
class JniEventInfo : public EventInfo<Args...>, public detail::JniEventInfoBase {
public:
    using typename EventInfo<Args...>::DataType;
    
    using EventInfo<Args...>::EventInfo;
    using EventInfo<Args...>::dispatch;
    
protected:
    using EventInfo<Args...>::_key;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
protected:
    virtual void jniDispatch(jobjectArray objects) const override {
        internalDispatch(objects, typename SequenceGenerator<sizeof...(Args)>::Type());
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
    void internalDispatch(jobjectArray objects, Sequence<Indices...>) const {
        auto&& convertedObjects = JniUtils::toVectorJObject(objects);
        CC_ASSERT(convertedObjects.size() == sizeof...(Args));
        dispatch(JniToCppConverter<Args>::convert(convertedObjects.at(Indices))...);
    }
#endif
};

template<>
class JniEventInfo<> : public EventInfo<>, public detail::JniEventInfoBase {
public:
    using typename EventInfo<>::DataType;
    
    using EventInfo<>::EventInfo;
    
protected:
    using EventInfo<>::_key;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
protected:
    virtual void jniDispatch(jobjectArray objects) const override {
        EE_LOGD("JniEventInfo::dispatch %s", _key.c_str());
        auto&& convertedObjects = JniUtils::toVectorJObject(objects);
        CC_ASSERT(convertedObjects.size() == 0);
        dispatch();
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

template<class... Args>
using Event [[deprecated("Use EventInfo instead.")]] = EventInfo<Args...>;

#define CREATE_EVENT_INFO(variableName) variableName(# variableName)
namespace_ee_end

#endif // EE_LIBRARY_EVENT_H