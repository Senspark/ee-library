//
//  EEBlurBackground.hpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#ifndef EEBlurBackground_hpp
#define EEBlurBackground_hpp

#include "EECocos2dxFwd.hpp"
#include "EEDialogTransitionProtocol.hpp"

NS_EE_BEGIN
class BlurBackground : public virtual DialogTransitionProtocol {
public:
    
protected:
    virtual void onShowBegan() override;
    virtual void onHideBegan() override;
    
    cocos2d::Sprite* blurredBackgroundSprite_;
};
NS_EE_END

#endif /* EEBlurBackground_hpp */
