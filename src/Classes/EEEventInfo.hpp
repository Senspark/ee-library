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
#include "EEEvent.hpp"
#include "EEEventListener.hpp"

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
    
private:
    std::string _key;
};
NS_DETAIL_END

/// Variadic arguments event info.
///
/// Improve the way of dispatching custom events.
///
/// Old way:
/// @code
/// // Register event listener.
/// auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
/// auto callback = CC_CALLBACK_1(MyClass::myCallback, this);
/// dispatcher->addCustomEventListener("event_name", callback);
///
/// // Callback function.
/// void MyClass::myCallback(cocos2d::EventCustom* event) {
///     auto userData = event->getUserData();
///     int myData = *static_cast<int*>(userData);
///     doSomething(myData);
/// }
///
/// // Dispatch event.
/// int myData = getMyData();
/// auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
/// dispacher->dispatchCustomEvent("event_name", &myData);
///
/// // Unregister all event listeners.
/// auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
/// dispatcher->removeCustomEventListeners("event_name");
/// @endcode
///
/// New way using ee::EventInfo.
/// @code
/// // Register event info.
/// ee::EventInfo<int> myEvent("event_name");
///
/// // Register event listener.
/// myEvent.addListener(CC_CALLBACK_1(MyClass::myCallback, this));
///
/// // Callback function.
/// void MyClass::myCallback(int myData) {
///     doSomething(myData);
/// }
///
/// // Dispatch event.
/// int myData = getMyData();
/// myEvent.dispatch(myData);
///
/// // Unregister all event listeners.
/// myEvent.removeListeners();
/// @endcode
/// @see CREATE_EVENT_INFO
/// @see CREATE_EVENT_INFO_EX
template<class... Args>
class EventInfo final : public detail::EventInfoBase {
public:
    using DataType = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;
    
    using EventType = detail::Event<Args...>;
    using EventListenerType = detail::EventListener<Args...>;
    
    using detail::EventInfoBase::EventInfoBase;
    
    /// Register an event listener.
    cocos2d::EventListener* addListener(const CallbackType& callback) const {
        auto eventCallback = [callback](EventType* event) {
            event->invoke(callback);
        };
        auto listener = EventListenerType::create(getKey(), eventCallback);
        addListenerHelper(listener);
        return listener;
    }
    
    void dispatch(Args... args) const {
        EventType ev(getKey());
        ev.setData(std::forward<Args>(args)...);
        dispatchHelper(&ev);
    }
};

#define CREATE_EVENT_INFO(variableName) \
    variableName(# variableName)

#define CREATE_EVENT_INFO_EX(...) \
    decltype(GET_FIRST_ARG(__VA_ARGS__)) CREATE_EVENT_INFO(__VA_ARGS__)
NS_EE_END

#endif // EE_LIBRARY_EVENT_INFO_HPP_