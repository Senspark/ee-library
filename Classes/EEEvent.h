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
#include "base/CCEventCustom.h"

#include <functional>
#include <memory>
#include <type_traits>

namespace_ee_begin
class EventBase {
public:
    void removeListeners() const;
    
    EventBase(const EventBase&) = delete;
    EventBase(EventBase&&) = delete;
    EventBase& operator=(const EventBase&) = delete;
    EventBase& operator=(EventBase&&) = delete;
    
protected:
    EventBase() : _key(ee::toString("event___", counter++)) {}
    
    cocos2d::EventListenerCustom* addListenerHelper(const std::function<void(cocos2d::EventCustom*)>& callback) const;
    
    void dispatchHelper(void* data) const;
    
private:
    static int counter;
    const std::string _key;
};

// Multiple arguments.
template<class... Args>
class Event : public EventBase {
public:
    using DataType = std::tuple<Args...>;
    
    Event() {}

    cocos2d::EventListenerCustom* addListener(const std::function<void(Args...)>& callback) const {
        return addListenerHelper([callback, this](cocos2d::EventCustom* event) {
            auto&& data = *static_cast<DataType*>(event->getUserData());
            unpackTuple(typename SequenceGenerator<sizeof...(Args)>::Type(), data, callback);
        });
    }
    
    void dispatch(Args... args) const {
        auto&& data = DataType(std::forward<Args>(args)...);
        dispatchHelper(&data);
    }
    
private:
    template<std::size_t... S>
    void unpackTuple(Sequence<S...>, const DataType& data, const std::function<void(Args...)>& callback) const {
        callback(std::get<S>(data)...);
    }
};
    
// Zero argument.
template<>
class Event<> : public EventBase {
public:
    Event() {}
    
    cocos2d::EventListenerCustom* addListener(const std::function<void()>& callback) const {
        return addListenerHelper([callback](cocos2d::EventCustom*) {
            callback();
        });
    }
    
    void dispatch() const {
        dispatchHelper(nullptr);
    }
};    
namespace_ee_end

#endif // EE_LIBRARY_EVENT_H