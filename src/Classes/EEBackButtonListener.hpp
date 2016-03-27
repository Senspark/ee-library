//
//  EEBackButtonListener.hpp
//  ee-library
//
//  Created by enrevol on 10/26/15.
//
//

#ifndef EE_LIBRARY_BACK_BUTTON_LISTENER_HPP
#define EE_LIBRARY_BACK_BUTTON_LISTENER_HPP

#include "EEMacro.hpp"
#include "EEForward.hpp"
#include "cocos2d.h"

namespace_ee_begin
class BackButtonListener {
protected:
    virtual ~BackButtonListener() {}
    
    void addBackButtonListener();
    void removeBackButtonListener();
    
    virtual void pressedBack() = 0;
    
private:
    cocos2d::Node* getInstance();
    
    cocos2d::EventListenerKeyboard* _listener;
};
namespace_ee_end

#endif /* EE_LIBRARY_BACK_BUTTON_LISTENER_HPP */
