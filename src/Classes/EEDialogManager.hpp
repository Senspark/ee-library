//
//  EEDialogManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#ifndef EE_LIBRARY_DIALOG_MANAGER_HPP_
#define EE_LIBRARY_DIALOG_MANAGER_HPP_

#include <vector>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <base/CCRefPtr.h>

NS_EE_BEGIN
namespace dialog {
class Dialog;
class Command;
class Guard;
class DialogManager;
} // namespace dialog

using dialog::DialogManager;

namespace dialog {
/// Dialog manager manages the dialogs in the scene.
class DialogManager {
public:
    static DialogManager* getInstance();
    
    /// Attempts to push the given dialog to the top level.
    void pushDialog(Dialog* dialog);
    
    /// Attempts to push the given dialog to the specified level.
    void pushDialog(Dialog* dialog, std::size_t level);
    
    /// Attempts to pop the given dialog.
    void popDialog(Dialog* dialog);
    
    /// Attempts to pop the top dialog.
    void popDialog();
    
    /// Retrieves the dialog whose the specified level in the current scece.
    Dialog* getDialog(std::size_t level);
    
    /// Retrieves the top dialog.
    /// @return @c nullptr if there is not any dialog in the current scene.
    Dialog* getTopDialog();
    
    /// Updates the whole dialog graph in the current scene.
    void updateCurrentScene();
    
    /// Processes available dialog command queue.
    void processCommandQueue();
    
    /// Checks whether there is a locking dialog.
    bool isLocked();
    
    /// Unlock the command queue with current dialog.
    void unlock(Dialog* dialog);
    
    /// Lock the command queue with given dialog.
    void lock(Dialog* dialog);
    
private:
    cocos2d::Node* getRunningNode();
    
    void pushDialogImmediately(Dialog* dialog, std::size_t level);
    
    void popDialogImmediately(Dialog* dialog);
    
    std::size_t currentLevel_;
    
    cocos2d::Scene* currentScene_;
    
    Dialog* lockingDialog_;
    
    std::vector<Command> commandQueue_;
    
    std::vector<Guard> dialogStack_;
};
} // namespace dialog.
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_MANAGER_HPP_ */
