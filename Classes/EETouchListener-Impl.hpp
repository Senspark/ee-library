//
//  EETouchListener-Impl.h
//  roll-eat
//
//  Created by enrevol on 9/2/15.
//
//

#ifndef EE_LIBRARY_TOUCH_LISTENER_IMPL_H
#define EE_LIBRARY_TOUCH_LISTENER_IMPL_H

#include "EETouchListener.hpp"

namespace_ee_begin
class TouchListenerNode::Impl {
public:
    Impl(TouchListenerNode* base);
    
    void addListener();

    bool _isInside;
    int _state;
    
    float _baseScaleX;
    float _baseScaleY;
    
    unsigned _hitCount;
    
    TouchListenerNode* _base;
    
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> _callbackList[4];
    std::function<void(ButtonState)> _stateChangedCallback;
    
    cocos2d::EventListenerTouchOneByOne* _listener;
};
    
class TouchListenerSprite::Impl {
public:
    Impl();
    virtual ~Impl();
    
    cocos2d::SpriteFrame* _normalSpriteFrame;
    cocos2d::SpriteFrame* _pressedSpriteFrame;
    cocos2d::SpriteFrame* _disabledSpriteFrame;
};
namespace_ee_end

#endif // EE_LIBRARY_TOUCH_LISTENER_IMPL_H
