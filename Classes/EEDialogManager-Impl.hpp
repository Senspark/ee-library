//
//  EEDialogManager-Impl.h
//  roll-eat
//
//  Created by enrevol on 8/27/15.
//
//

#ifndef EE_LIBRARY_DIALOG_MANAGER_IMPL_H
#define EE_LIBRARY_DIALOG_MANAGER_IMPL_H

#include "EEDialogManager.hpp"

#include <queue>
#include <vector>
#include <unordered_set>

namespace_ee_begin
class DialogManager::Impl : public DialogManager {
public:
    void updateCurrentScene();
    void processDialogQueue();
    
    cocos2d::Scene* _lastScene;
    
    std::unordered_set<Dialog*> _lockingDialog;
    
    struct DialogInfo {
        DialogInfo(cocos2d::Node* _container, Dialog* _dialog)
        : container(_container)
        , dialog(_dialog)
        {}
        
        cocos2d::Node* container;
        Dialog* dialog;
    };
    
    struct PushDialogInfo {
        PushDialogInfo(cocos2d::Node* _container, Dialog* _dialog, int _localZOrder)
        : container(_container)
        , dialog(_dialog)
        , localZOrder(_localZOrder)
        {}
                       
        cocos2d::Node* container;
        Dialog* dialog;
        int localZOrder;
    };
    
    void internalPushDialog(const PushDialogInfo& info);
    
    std::queue<PushDialogInfo> _dialogQueue;
    std::vector<DialogInfo> _dialogStack;
};
namespace_ee_end

#endif // EE_LIBRARY_DIALOG_MANAGER_IMPL_H