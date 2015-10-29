//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "EEDialog.hpp"
#include "EEDialogManager.hpp"
#include "cocos2d.h"

namespace_ee_begin
bool Dialog::init() {
    // Close the dialog when pressing its outer.
    setTouchType(TouchType::Outside);
    setTouchUpCallback(CC_CALLBACK_0(Dialog::hide, this));
    return true;
}
    
void Dialog::onEnter() {
    NodeButton::onEnter();
    setEnabled(true);
}
    
void Dialog::onExit() {
    NodeButton::onExit();
    setEnabled(false);
}
    
void Dialog::pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder) {
    DialogManager::getInstance()->pushDialog(container, dialog, localZOrder);
}

void Dialog::popDialog(Dialog* dialog) {
    DialogManager::getInstance()->popDialog(dialog);
}
namespace_ee_end