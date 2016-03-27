//
//  EEEvent.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EEEvent_hpp
#define EEEvent_hpp

#include <functional>

#include "EEMacro.hpp"
#include "EEExtension.hpp"

#include "base/CCEventCustom.h"

NS_EE_BEGIN
NS_DETAIL_BEGIN
template<class... Args>
class Event : public cocos2d::EventCustom {
public:
    using ArgTypes = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;
    
    using EventCustom::EventCustom;
    
    void setData(Args... args) {
        _callback = std::bind(&Event::impl, this, std::placeholders::_1, std::ref(std::forward<Args>(args))...);
    }
    
    void invoke(const CallbackType& callback) const {
        _callback(callback);
    }
    
private:
    void impl(const CallbackType& callback, Args... args) const {
        callback(std::forward<Args>(args)...);
    }
    
    std::function<void(const CallbackType&)> _callback;
};
NS_DETAIL_END
NS_EE_END

#endif /* EEEvent_hpp */
