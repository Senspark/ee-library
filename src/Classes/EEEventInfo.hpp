//
//  EEEvent.h
//  roll-eat
//
//  Created by enrevol on 9/19/15.
//
//

#ifndef EE_LIBRARY_EVENT_INFO_HPP_
#define EE_LIBRARY_EVENT_INFO_HPP_

#include <functional>

#include "EEMacro.hpp"
#include "EEForward.hpp"
#include "EEEvent.hpp"
#include "EEEventListener.hpp"
#include "EEUtils.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
class EventInfoBase {
public:
    explicit EventInfoBase(std::string key);
    
    /// Remove all associated event listeners.
    void removeListeners() const;

    /// Retrieves the event unikey key.
    const std::string& getKey() const;
    
protected:
    void addListenerHelper(cocos2d::EventListener* listener) const;
    
    void dispatchHelper(cocos2d::Event* event) const;
    
    int _id;
    std::string _key;
    
    static int counter;
};
NS_DETAIL_END

template<class... Args>
class EventInfo : public detail::EventInfoBase {
public:
    using DataType = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;
    
    using detail::EventInfoBase::EventInfoBase;
    
    cocos2d::EventListener* addListener(const CallbackType& callback) const {
        auto listener = detail::EventListener<Args...>::create(toString(_id, _key), [callback](detail::Event<Args...>* event) {
            event->invoke(callback);
        });
        addListenerHelper(listener);
        return listener;
    }
    
    void dispatch(Args... args) const {
        detail::Event<Args...> ev(toString(_id, _key));
        ev.setData(std::forward<Args>(args)...);
        dispatchHelper(&ev);
    }
};

#define CREATE_EVENT_INFO(variableName) variableName(# variableName)
#define CREATE_EVENT_INFO_EX(...)       decltype(GET_FIRST_ARG(__VA_ARGS__)) CREATE_EVENT_INFO(__VA_ARGS__)
NS_EE_END

#endif // EE_LIBRARY_EVENT_INFO_HPP_