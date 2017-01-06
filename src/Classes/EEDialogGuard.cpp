//
//  DialogGuard.cpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#include "EEDialogGuard.hpp"
#include "EEDialog.hpp"

NS_EE_BEGIN
namespace dialog {
Guard::Guard(Dialog* dialog_)
    : dialog(dialog_), guard_(dialog->getContainer()) {}
} // namespace dialog
NS_EE_END
