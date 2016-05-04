//
//  EEDialogManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#ifndef EE_LIBRARY_DIALOG_MANAGER_H
#define EE_LIBRARY_DIALOG_MANAGER_H

#include <deque>
#include <functional>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

NS_EE_BEGIN
class Dialog;

/// Dialog manager manages the dialogs in the scene.
class DialogManager {
public:
    using Command = std::function<Dialog*()>;
    
private:
    friend class Dialog;
    
    /// Adds a command the to the command queue.
//    void addCommand(CommandType type, const Command& command);
    
    /// Processes commands in command queue.
    void processCommand();
    
//    std::queue<std::pair<CommandType, Command>> _commandQueue;
    
//    void addToQueue(const std::function<void()>& callback);
//    
//    void pushDialog(cocos2d::Node* container, Dialog* dialog, int localZOrder);
//    void popDialog(Dialog* dialog);
//    
//    void lock(Dialog* dialog);
//    void unlock(Dialog* dialog);
//    
//    void updateCurrentScene();
//    void processActionQueue();
//    
//    cocos2d::Scene* _lastScene;
    
    Dialog* _lockingDialog;
    
//    struct DialogInfo {
//        DialogInfo(cocos2d::Node* _container, Dialog* _dialog)
//        : container(_container)
//        , dialog(_dialog)
//        {}
//        
//        cocos2d::RefPtr<cocos2d::Node> container;
//        Dialog* dialog;
//    };
//    
//    std::deque<std::function<void()>> _actionQueue;
//    std::vector<DialogInfo> _dialogStack;
};
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_MANAGER_H */
