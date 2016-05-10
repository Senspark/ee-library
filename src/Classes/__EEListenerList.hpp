//
//  EEEventVector.hpp
//  ee-library
//
//  Created by Zinge on 5/8/16.
//
//

#ifndef EEEventVector_hpp
#define EEEventVector_hpp

#include <vector>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEUtils.hpp"

NS_EE_BEGIN
class ListenerList {
public:
    void push_back(cocos2d::EventListener* listener);
    
    void clear() noexcept;
    
private:
    std::vector<UniqueListener> listeners_;
};
NS_EE_END

#endif /* EEEventVector_hpp */
