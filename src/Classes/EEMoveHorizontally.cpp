//
//  EEMoveHorizontally.cpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#include "EEMoveHorizontally.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
MoveHoritzontally::MoveHoritzontally() {
    auto&& winSize = cocos2d::Director::getInstance()->getWinSize();
    
    auto place = cocos2d::Place::create(cocos2d::Point(-winSize.width / 2,
                                                       winSize.height / 2));
    
    auto move = cocos2d::MoveBy::create(0.8f, cocos2d::Vec2(winSize.width, 0));
    auto moveIn = cocos2d::EaseElasticOut::create(move->clone(), 0.8f);
    auto moveOut = cocos2d::EaseElasticIn::create(move->clone(), 0.8f);
    
    addShowingTransitions(place, moveIn);
    addHidingTransitions(moveOut);
}
NS_EE_END