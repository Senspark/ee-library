//
//  EETouchListener__.cpp
//  ee-library
//
//  Created by enrevol on 10/28/15.
//
//

#include "EETouchListener-Impl.hpp"
#include "EETouchManager.hpp"
#include "cocos2d.h"

namespace_ee_begin
TouchListener::TouchListener()
: _impl(new Impl(this))
{}

TouchListener::~TouchListener() {}

void TouchListener::setTouchType(TouchType type) {
    _impl->_touchType = type;
}

TouchType TouchListener::getTouchType() const {
    return _impl->_touchType;
}

bool TouchListener::isEnabled() const {
    return _impl->_listener->isEnabled();
}
void TouchListener::setEnabled(bool enabled) {
    _impl->_listener->setEnabled(enabled);
}

void TouchListener::setSwallowTouches(bool needSwallow) {
    _impl->_listener->setSwallowTouches(needSwallow);
}

bool TouchListener::isSwallowTouches() {
    return _impl->_listener->isSwallowTouches();
}

void TouchListener::setBaseScale(float scale) {
    _impl->_baseScale = scale;
}

void TouchListener::setZoomScaleRatio(float ratio) {
    _impl->_zoomScaleRatio = ratio;
}

float TouchListener::getZoomScaleRatio() const {
    return _impl->_zoomScaleRatio;
}

void TouchListener::setZoomDuration(float duration) {
    _impl->_zoomDuration = duration;
}

float TouchListener::getZoomDuration() const {
    return _impl->_zoomDuration;
}

void TouchListener::setTouchBeganCallback(const TouchEventCallback& callback) {
    _impl->_touchBeganCallback = callback;
}

void TouchListener::setTouchMovedCallback(const TouchEventCallback& callback) {
    _impl->_touchMovedCallback = callback;
}

void TouchListener::setTouchUpCallback(const TouchEventCallback& callback) {
    _impl->_touchUpCallback = callback;
}

void TouchListener::setTouchMovedThreshold(float delta) {
    _impl->_moveThreshold = delta;
}

void TouchListener::updateState() {
    if (isEnabled()) {
        updateState(_impl->_buttonState);
    } else {
        updateState(ButtonState::Disabled);
    }
}

void TouchListener::updateState(ButtonState state) {
    if (state != ButtonState::Disabled) {
        _impl->_buttonState = state;
    }
}

TouchListener::Impl::Impl(TouchListener* base)
: _base(base)
, _shouldCancel(false)
, _moveThreshold(0)
, _isInside(false)
, _state(0)
, _buttonState(ButtonState::Normal)
, _baseScale(1.0f)
, _zoomScaleRatio(1.15f)
, _zoomDuration(0.04f)
{}

void TouchListener::Impl::addListener() {
    _listener = cocos2d::EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = CC_CALLBACK_2(Impl::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(Impl::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(Impl::onTouchEnded, this);
    _listener->onTouchCancelled = CC_CALLBACK_2(Impl::onTouchCanceled, this);
}

bool TouchListener::Impl::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    bool ret = false;
    auto instance = _base->getInstance();
    instance->retain();
    // Check whether touch is locked.
    if (TouchManager::getInstance()->isLocked() == false) {
        // Reset cancel flag.
        _shouldCancel = false;
        // Check whether touch is inside.
        _isInside = isTouchInside(touch);
        // Touch inside type.
        if (_touchType == TouchType::Inside) {
            if (_isInside) {
                ret = true;
                if (_zoomScaleRatio != 1.0f) {
                    // Run zoom action.
                    instance->stopActionByTag(ZoomActionTag);
                    instance->stopActionByTag(OtherActionTag);
                    auto press = cocos2d::ScaleTo::create(_zoomDuration, _baseScale * _zoomScaleRatio);
                    press->setTag(ZoomActionTag);
                    instance->runAction(press);
                }
                // Update state.
                _base->updateState(ButtonState::Pressed);
                // Lock touch.
                TouchManager::getInstance()->lock(touch);
            }
        } else {
            // Touch outside type.
            if (_isInside == false) {
                // We should not lock touch outside.
                ret = true;
            }
        }
        // Invoke callback.
        if (_touchBeganCallback) {
            _touchBeganCallback(touch, event);
        }
        ret = true;
    }
    instance->release();
    return ret;
}

void TouchListener::Impl::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    // Check whether its the same position.
    if (touch->getDelta() == cocos2d::Vec2::ZERO) {
        return;
    }
    // Check whether touch should be cancelled.
    float delta = std::max(std::abs(touch->getDelta().x), std::abs(touch->getDelta().y));
    if (_moveThreshold < delta) {
        _shouldCancel = true;
    }
    auto instance = _base->getInstance();
    instance->retain();
    bool wasInside = _isInside;
    _isInside = isTouchInside(touch);
    if (wasInside != _isInside) {
        // Touch inside type.
        if (_touchType == TouchType::Inside) {
            instance->stopActionByTag(ZoomActionTag);
            float finalScale = _baseScale * _zoomScaleRatio;
            float currentScale = instance->getScaleX();
            if (_isInside) {
                float ratio = (finalScale - currentScale) / (finalScale - _baseScale);
                auto press = cocos2d::ScaleTo::create(_zoomDuration * ratio, finalScale);
                press->setTag(ZoomActionTag);
                instance->runAction(press);
            } else {
                float ratio = (currentScale - _baseScale) / (finalScale - _baseScale);
                auto normal = cocos2d::ScaleTo::create(_zoomDuration * ratio, _baseScale);
                normal->setTag(OtherActionTag);
                instance->runAction(normal);
            }
        } else {
            // Touch outside type does nothing.
        }
        // Update state.
        if (_isInside) {
            _base->updateState(ButtonState::Pressed);
        } else {
            _base->updateState(ButtonState::Normal);
        }
    }
    // Invoke callback.
    if (_touchMovedCallback) {
        _touchMovedCallback(touch, event);
    }
    instance->release();
}

void TouchListener::Impl::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    auto instance = _base->getInstance();
    instance->retain();
    // Touch inside type.
    if (_touchType == TouchType::Inside) {
        // Run zoom action.
        if (_zoomScaleRatio != 1.0f) {
            if (_isInside) {
                auto currentAction = static_cast<cocos2d::ScaleTo*>(instance->getActionByTag(ZoomActionTag));
                cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
                actions.reserve(3);
                if (currentAction != nullptr && currentAction->isDone() == false) {
                    float remaining = _zoomDuration - currentAction->getElapsed();
                    auto delay = cocos2d::DelayTime::create(remaining);
                    actions.pushBack(delay);
                }
                auto normal = cocos2d::ScaleTo::create(_zoomDuration, _baseScale);
                actions.pushBack(normal);
                auto callFunc = cocos2d::CallFunc::create([touch, event, this] {
                    if (_shouldCancel == false) {
                        if (_touchUpCallback) {
                            _touchUpCallback(touch, event);
                        }
                    }
                    TouchManager::getInstance()->unlock(touch);
                });
                actions.pushBack(callFunc);
                auto sequence = cocos2d::Sequence::create(actions);
                sequence->setTag(OtherActionTag);
                instance->runAction(sequence);
            }
        } else {
            
        }
    } else {
        if (_shouldCancel == false) {
            if (_touchUpCallback) {
                _touchUpCallback(touch, event);
            }
        }
    }
    // Update state.
    if (_isInside) {
        _base->updateState(ButtonState::Normal);
    }
    instance->release();
}

void TouchListener::Impl::onTouchCanceled(cocos2d::Touch* touch, cocos2d::Event* event) {
    TouchManager::getInstance()->unlock(touch);
}
namespace_ee_end