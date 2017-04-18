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

#include "EECocos2dxFwd.hpp"

#include <base/CCRefPtr.h>

namespace ee {
namespace dialog {
enum class CommandType;

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
    /// @note This is also update the current scene.
    Dialog* getDialog(std::size_t level);

    /// Retrieves the top dialog.
    /// @note This is also update the current scene.
    /// @return @c nullptr if there is not any dialog in the current scene.
    Dialog* getTopDialog();

    std::size_t getTopDialogLevel();

private:
    /// Updates the whole dialog graph in the current scene.
    /// @return Whether the current scene has changed.
    bool updateCurrentScene();

    Dialog* getDialogWithoutUpdate(std::size_t level) const;

    Dialog* getTopDialogWithoutUpdate() const;

    /// Gets the (top) running node in the current scene.
    /// @note updateCurrentScene() must be called first to ensure that the
    /// result is correct.
    cocos2d::Node* getRunningNode() const;

    /// Checks whether there is a locking dialog.
    /// @note updateCurrentScene() must be called first to ensure that the
    /// result is correct.
    bool isLocked() const;

    /// Unlocks the command queue with the specified dialog.
    /// @param dialog The dialog to be unlocked.
    void unlock(Dialog* dialog);

    /// Locks the command queue with the specified dialog.
    /// @param dialog The dialog to be locked.
    void lock(Dialog* dialog);

    /// Processes available dialog command queue.
    /// @return Whether any command was executed.
    bool processCommandQueue();

    /// Processes the specified command.
    /// @param command The command to be processed.
    /// @return Whether the command was executed.
    bool processCommand(const Command& command);

    /// Processes a push command.
    /// @param dialog The dialog to be pushed.
    /// @param level The level to be considered.
    /// @return Whether the command was executed.
    bool processPushCommand(Dialog* dialog, std::size_t level);

    /// Processes a pop command.
    /// @param dialog The dialog to be popped.
    /// @param level The level to be considered.
    /// @return Whether the command was executed.
    bool processPopCommand(Dialog* dialog, std::size_t level);

    /// Pushes a command to the command queue.
    /// @param command The command to be pushed.
    /// @return Whether the command was pushed.
    bool pushCommand(const Command& command);

    void pushDialogImmediately(Dialog* dialog, std::size_t level);

    void popDialogImmediately(Dialog* dialog);

    std::size_t currentLevel_;

    cocos2d::Scene* currentScene_;

    Dialog* lockingDialog_;

    std::vector<Command> commandQueue_;

    std::vector<Guard> dialogStack_;
};
} // namespace dialog.
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_MANAGER_HPP_ */
