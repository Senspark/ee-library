//
//  EEDialogManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#ifndef EE_LIBRARY_DIALOG_MANAGER_H
#define EE_LIBRARY_DIALOG_MANAGER_H

#include "EEForward.hpp"
#include "EEMacro.hpp"

namespace_ee_begin
class DialogManager {
public:
    static DialogManager* getInstance();
    
    Dialog* getCurrentDialog() const;
    void hideDialog();
    
    cocos2d::Node* getRunningNode();
    cocos2d::Node* getParentNode();
    
    cocos2d::Scene* getCurrentScene();
    
private:
    friend class Dialog;
    
    void pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder);
    void popDialog(Dialog* dialog);
    
    void lock(Dialog* dialog);
    void unlock(Dialog* dialog);
    
    class Impl;   
};
namespace_ee_end

#endif /* EE_LIBRARY_DIALOG_MANAGER_H */
