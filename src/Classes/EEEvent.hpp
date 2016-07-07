//
//  EEEvent.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EE_LIBRARY_EVENT_HPP_
#define EE_LIBRARY_EVENT_HPP_

#include <functional>

#include "EEMacro.hpp"

#include <base/CCEventCustom.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
/// Variadic arguments (custom) event.
template <class... Args> class Event final : public cocos2d::EventCustom {
public:
    using ArgTypes = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;

    using EventCustom::EventCustom;

    /// Assigns the event data to invoke later.
    void setData(Args... args) {
        callback_ = std::bind(&Event::invokeImpl, this, std::placeholders::_1,
                              std::forward<Args>(args)...);
    }

    /// Invokes the given callback with stored arguments.
    void invoke(const CallbackType& callback) const { callback_(callback); }

private:
    void invokeImpl(const CallbackType& callback, Args... args) const {
        callback(std::forward<Args>(args)...);
    }

    std::function<void(const CallbackType&)> callback_;
};
NS_DETAIL_END
NS_EE_END

#endif /* EE_LIBRARY_EVENT_HPP_ */
