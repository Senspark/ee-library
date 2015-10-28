//
//  EETouchListenerNode-Impl.hpp
//  ee-library
//
//  Created by enrevol on 10/28/15.
//
//

#ifndef EE_LIBRARY_TOUCH_LISTENER_IMPL_HPP
#define EE_LIBRARY_TOUCH_LISTENER_IMPL_HPP

#include "EETouchListener.hpp"

namespace_ee_begin
class TouchListener::Impl {
public:
    Impl(TouchListener* base);
    
    void addListener();
    bool isTouchInside(cocos2d::Touch* touch) const;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCanceled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    TouchListener* _base;
    
    bool _shouldCancel;
    float _moveThreshold;
    
    bool _isInside;
    int _state;
    
    ButtonState _buttonState;
    TouchType _touchType;
    
    float _baseScale;
    float _zoomScaleRatio;
    float _zoomDuration;
    
    TouchEventCallback _touchBeganCallback;
    TouchEventCallback _touchMovedCallback;
    TouchEventCallback _touchUpCallback;
    
    cocos2d::EventListenerTouchOneByOne* _listener;
    
    static constexpr int ZoomActionTag = 999;
    static constexpr int OtherActionTag = 1000;
};
namespace_ee_end

#endif /* EE_LIBRARY_TOUCH_LISTENER_IMPL_HPP */
