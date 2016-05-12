//
//  EEDialogUtils.hpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#ifndef EEDialogUtils_hpp
#define EEDialogUtils_hpp

#include "EEMacro.hpp"

NS_EE_BEGIN
namespace dialog {
class Dialog;
} // namespace dialog

using dialog::Dialog;

Dialog* addBlurBackground(Dialog* dialog, float duration = 0.8f,
                          float scale = 0.25f, int blurRadius = 2);
    
Dialog* addHorizontalTransition(Dialog* dialog, float duration = 0.8f,
                                float rate = 0.8f);
NS_EE_END

#endif /* EEDialogUtils_hpp */
