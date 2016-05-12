//
//  DialogGuard.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef DialogGuard_hpp
#define DialogGuard_hpp

#include "EEMacro.hpp"

#include <base/CCRefPtr.h>

NS_EE_BEGIN
namespace dialog {
class Dialog;
    
struct Guard final {
    explicit Guard(Dialog* dialog_);
    
    Dialog* dialog;
    
private:
    cocos2d::RefPtr<cocos2d::Node> guard_;
};
} // namespace dialog
NS_EE_END

#endif /* DialogGuard_hpp */
