//
//  EEDialogMessage.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef EE_LIBRARY_DIALOG_COMMAND_HPP_
#define EE_LIBRARY_DIALOG_COMMAND_HPP_

#include <cstddef>

#include "EEMacro.hpp"
#include "EEDialogGuard.hpp"

NS_EE_BEGIN
namespace dialog {
class Dialog;

enum class CommandType {
    Push,
    Pop,
};

/// Stores a dialog command.
///
/// The dialog and its container will be retained.
struct Command final {
    explicit Command(CommandType type,
                     Dialog* dialog,
                     std::size_t level);
    
    CommandType type;
    Dialog* dialog;
    std::size_t level;
    
private:
    Guard guard_;
};
} // namespace dialog.
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_COMMAND_HPP_ */
