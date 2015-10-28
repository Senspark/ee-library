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
#include "CCNode.h"

namespace_ee_begin
class BackButtonListener : public virtual cocos2d::Node {
protected:
    void addBackButtonListener();
    void removeBackButtonListener();
    
    virtual void pressedBack() = 0;
    
    cocos2d::EventListenerKeyboard* _backButtonListener;
};
namespace_ee_end

#endif /* EE_LIBRARY_BACK_BUTTON_LISTENER_HPP */
