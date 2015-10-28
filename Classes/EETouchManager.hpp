//
//  EETouchManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/4/15.
//
//

#ifndef EE_LIBRARY_TOUCH_MANAGER_H
#define EE_LIBRARY_TOUCH_MANAGER_H

#include "EEForward.hpp"
#include "EEMacro.hpp"

namespace_ee_begin
class TouchManager {
public:
    static TouchManager* getInstance();
    
    void lock(cocos2d::Touch* touch);
    void unlock();
    
    bool isLocked();
    
private:
    class Impl;
};
namespace_ee_end

#endif /* EE_LIBRARY_TOUCH_MANAGER_H */
