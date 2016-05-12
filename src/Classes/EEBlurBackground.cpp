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
    
    auto blurredBackgroundSprite = captureBlurredScreenInPoints(0.25f);
    blurredBackgroundSprite->setScale(4.0f);
    blurredBackgroundSprite->setNormalizedPosition(cocos2d::Vec2::ANCHOR_MIDDLE);
    
    thiz->getContainer()->addChild(blurredBackgroundSprite,
                                   std::numeric_limits<int>::lowest());
}
NS_EE_END