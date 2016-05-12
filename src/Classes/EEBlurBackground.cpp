//
//  EEBlurBackground.cpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#include "EEBlurBackground.hpp"
#include "EEDialog.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
void BlurBackground::onShowBegan() {
    auto thiz = dynamic_cast<Dialog*>(this);
    CC_ASSERT(thiz != nullptr);
    
    CC_ASSERT(blurredBackgroundSprite_ == nullptr);
    
    blurredBackgroundSprite_ = captureBlurredScreenInPoints(0.25f);
    blurredBackgroundSprite_->setScale(4.0f);
    blurredBackgroundSprite_->setNormalizedPosition(cocos2d::Vec2::ANCHOR_MIDDLE);
    
    blurredBackgroundSprite_->setOpacity(0);
    blurredBackgroundSprite_->runAction(cocos2d::FadeIn::create(0.8f));
    
    thiz->getContainer()->addChild(blurredBackgroundSprite_,
                                   std::numeric_limits<int>::lowest());
}

void BlurBackground::onHideBegan() {
    CC_ASSERT(blurredBackgroundSprite_ != nullptr);
    
    auto sequence = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.8f),
                                              cocos2d::RemoveSelf::create(),
                                              nullptr);
    
    blurredBackgroundSprite_->runAction(sequence);
}
NS_EE_END