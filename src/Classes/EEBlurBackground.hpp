//
//  EEBlurBackground.hpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#ifndef EEBlurBackground_hpp
#define EEBlurBackground_hpp

#include "EEDialogTransitionProtocol.hpp"

NS_EE_BEGIN
class BlurBackground : public virtual DialogTransitionProtocol {
public:
    
protected:
    virtual void onShowBegan() override;
};
NS_EE_END

#endif /* EEBlurBackground_hpp */
