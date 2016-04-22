//
//  EEEventListener.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EEEventListener_hpp
#define EEEventListener_hpp

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEForward.hpp"

#include <base/CCEventListener.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
/// Variadic arguments (custom) event listener.
template<class... Args>
class EventListener final : public cocos2d::EventListener {
public:
    using EventType = Event<Args...>;
    using CallbackType = std::function<void(EventType*)>;
    
    /// Creates an event listener with name and callback.
    static EventListener* create(const std::string& eventName,
                                 const CallbackType& callback) {
        auto result = new (std::nothrow) EventListener();
        if (result != nullptr && result->init(eventName, callback)) {
            result->autorelease();
        } else {
            delete result;
            result = nullptr;
        }
        return result;
    }
     
    virtual bool checkAvailable() override {
        return cocos2d::EventListener::checkAvailable() && _onCustomEvent;
    }
    
    virtual EventListener* clone() override {
        return create(_listenerID, _onCustomEvent);
    }
    
private:    
    bool init(const ListenerID& listenerId, const CallbackType& callback) {
        _onCustomEvent = callback;
        auto listener = [this](cocos2d::Event* event) {
            if (_onCustomEvent) {
                _onCustomEvent(dynamic_cast<EventType*>(event));
            }
        };
        return cocos2d::EventListener::init(Type::CUSTOM, listenerId, listener);
    }
    
    CallbackType _onCustomEvent;
};
NS_DETAIL_END
NS_EE_END

#endif /* EEEventListener_hpp */
