//
//  DialogGuard.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef EE_LIBRARY_DIALOG_GUARD_HPP_
#define EE_LIBRARY_DIALOG_GUARD_HPP_

#include "EEMacro.hpp"

#include <base/CCRefPtr.h>

NS_EE_BEGIN
namespace dialog {
class Dialog;
    
struct Guard final {
    explicit Guard(Dialog* dialog);
    
    Dialog* dialog;
    
private:
    cocos2d::RefPtr<cocos2d::Node> guard_;
};
} // namespace dialog
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_GUARD_HPP_ */
