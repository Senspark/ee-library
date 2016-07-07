//
//  EEEventDispatcher.hpp
//  ee-library
//
//  Created by Zinge on 7/7/16.
//
//

#ifndef EEEventDispatcher_hpp
#define EEEventDispatcher_hpp

#include <memory>
#include <vector>

#include "EEEvent.hpp"
#include "EEEventInfoV2.hpp"
#include "EEEventListener.hpp"
#include "EEUtils.hpp"

namespace ee {
class EventDispatcher {
public:
    template <class EventType, class Callable>
    void addListener(Callable&& callable) {
        addListener0(EventType{}, std::forward<Callable>(callable));
    }

    template <class EventType, class... Args> void dispatch(Args... args) {
        dispatch0(EventType{}, std::forward<Args>(args)...);
    }

    void clear() noexcept { listeners_.clear(); }

private:
    template <int Id, class... Args, class Callable>
    void addListener0(EventInfo<Id, Args...>, Callable&& callable) {
        using CallbackType = typename EventInfo<Id, Args...>::CallbackType;
        using EventType = detail::Event<Args...>;
        using EventInfoType = EventInfo<Id, Args...>;
        using EventListenerType = detail::EventListener<Args...>;

        auto listener = EventListenerType::create(
            EventInfoType::getKey(),
            std::bind(&EventType::invoke, std::placeholders::_1,
                      CallbackType{callable}));

        addListener1(listener);
        listeners_.push_back(ee::make_unique_listener(listener));
    }

    template <int Id, class... Args>
    void dispatch0(EventInfo<Id, Args...>, Args... args) {
        using EventType = detail::Event<Args...>;
        using EventInfoType = EventInfo<Id, Args...>;

        EventType ev{EventInfoType::getKey()};
        ev.setData(std::forward<Args>(args)...);
        dispatch1(&ev);
    }

    void addListener1(cocos2d::EventListener* listener) const;
    void dispatch1(cocos2d::Event* event) const;

    std::vector<UniqueListener> listeners_;
};
} // namespace ee

#endif /* EEEventDispatcher_hpp */
