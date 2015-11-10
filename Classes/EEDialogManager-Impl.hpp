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

#include <deque>
#include <memory>
#include <utility>
#include <vector>

namespace_ee_begin
class DialogManager::Impl : public DialogManager {
public:
    void updateCurrentScene();
    void processActionQueue();
    
    cocos2d::Scene* _lastScene;
    Dialog* _lockingDialog;
    
    struct DialogInfo {
        DialogInfo(cocos2d::Node* _container, Dialog* _dialog)
        : container(_container)
        , dialog(_dialog)
        {}
        
        cocos2d::Node* container;
        Dialog* dialog;
    };
    
    std::deque<std::function<void()>> _actionQueue;
    std::vector<DialogInfo> _dialogStack;
};
namespace_ee_end

#endif // EE_LIBRARY_DIALOG_MANAGER_IMPL_H