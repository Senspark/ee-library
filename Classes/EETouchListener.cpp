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

void TouchListener::addListener() {
    _impl->_listener = cocos2d::EventListenerTouchOneByOne::create();
    _impl->_listener->onTouchBegan = CC_CALLBACK_2(Impl::onTouchBegan, _impl.get());
    _impl->_listener->onTouchMoved = CC_CALLBACK_2(Impl::onTouchMoved, _impl.get());
    _impl->_listener->onTouchEnded = CC_CALLBACK_2(Impl::onTouchEnded, _impl.get());
    _impl->_listener->onTouchCancelled = CC_CALLBACK_2(Impl::onTouchCanceled, _impl.get());
    _impl->_listener->setEnabled(false);
    _impl->_listeningNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_impl->_listener, _impl->_listeningNode);
}

cocos2d::EventListenerTouchOneByOne* TouchListener::getEventListener() const {
    return _impl->_listener;
}

void TouchListener::enableState(int state) {
    if (state & TouchState::Zoom) {
        setZoom(true);
    }
}

void TouchListener::setTouchType(TouchType type) {
    _impl->_touchType = type;
}

TouchType TouchListener::getTouchType() const {
    return _impl->_touchType;
}

ButtonState TouchListener::getButtonState() const {
    if (isEnabled()) {
        return _impl->_buttonState;
    }
    return ButtonState::Disabled;
}

bool TouchListener::isEnabled() const {
    return _impl->_listener->isEnabled();
}
void TouchListener::setEnabled(bool enabled) {
    _impl->_listener->setEnabled(enabled);
    updateState();
}

void TouchListener::setSwallowTouches(bool needSwallow) {
    _impl->_listener->setSwallowTouches(needSwallow);
}

bool TouchListener::isSwallowTouches() {
    return _impl->_listener->isSwallowTouches();
}

void TouchListener::setBaseScale(float scale) {
    setBaseScaleX(scale);
    setBaseScaleY(scale);
}

void TouchListener::setBaseScale(float scaleX, float scaleY) {
    setBaseScaleX(scaleX);
    setBaseScaleY(scaleY);
}

void TouchListener::setBaseScaleX(float scale) {
    _impl->_baseScaleX = scale;
}

void TouchListener::setBaseScaleY(float scale) {
    _impl->_baseScaleY = scale;
}

void TouchListener::setListeningNode(cocos2d::Node* node) {
    _impl->_listeningNode = node;
}

cocos2d::Node* TouchListener::getListeningNode() const {
    return _impl->_listeningNode;
}

bool TouchListener::isZoom() const {
    return _impl->_isZoom;
}

void TouchListener::setZoom(bool enabled) {
    _impl->_isZoom = enabled;
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

void TouchListener::setInsideChecker(const std::function<bool(cocos2d::Touch*)>& checker) {
    _impl->_insideChecker = checker;
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

auto TouchListener::getTouchUpCallback() const -> const TouchEventCallback& {
    return _impl->_touchUpCallback;
}

void TouchListener::setTouchMovedThreshold(float delta) {
    _impl->_moveThreshold = delta;
}

void TouchListener::setCancelTouchEnd(bool cancel) {
    _impl->_isCancelTouchEnd = cancel;
}

bool TouchListener::isCancelTouchEnd() const {
    return _impl->_isCancelTouchEnd;
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
, _moveThreshold(2.0f)
, _isCancelTouchEnd(false)
, _isInside(false)
, _state(0)
, _buttonState(ButtonState::Normal)
, _touchType(TouchType::Inside)
, _baseScaleX(1.0f)
, _baseScaleY(1.0f)
, _isZoom(false)
, _zoomScaleRatio(1.15f)
, _zoomDuration(0.04f)
, _insideChecker(nullptr)
, _touchBeganCallback(nullptr)
, _touchMovedCallback(nullptr)
, _touchUpCallback(nullptr)
, _listener(nullptr) {
    _insideChecker = [this](cocos2d::Touch* touch) {
        bool ret = false;
        auto&& localPosition = _listeningNode->convertToNodeSpace(touch->getLocation());
        auto&& contentSize = _listeningNode->getContentSize();
        if (0 <= localPosition.x && localPosition.x <= contentSize.width &&
            0 <= localPosition.y && localPosition.y <= contentSize.height) {
            ret = true;
        }
        return ret;
    };
}

bool TouchListener::Impl::isActuallyVisible() const {
    bool ret = true;
    for (auto node = _listeningNode; node != nullptr && ret; node = node->getParent()) {
        ret = node->isVisible();
    }
    return ret;
}

bool TouchListener::Impl::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    bool ret = false;
    _listeningNode->retain();
    // Check whether touch is locked.
    if (TouchManager::getInstance()->isLocked() == false && isActuallyVisible()) {
        // Reset cancel flag.
        _shouldCancel = false;
        // Check whether touch is inside.
        _isInside = _insideChecker(touch);
        // Touch inside type.
        if (_touchType == TouchType::Inside) {
            if (_isInside) {
                ret = true;
                if (_isZoom && _zoomScaleRatio != 1.0f) {
                    // Run zoom action.
                    _listeningNode->stopActionByTag(ZoomActionTag);
                    _listeningNode->stopActionByTag(OtherActionTag);
                    auto press = cocos2d::ScaleTo::create(_zoomDuration, _baseScaleX * _zoomScaleRatio, _baseScaleY * _zoomScaleRatio);
                    press->setTag(ZoomActionTag);
                    _listeningNode->runAction(press);
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
                if (_isZoom) {
                    CCLOG("Touch outside does not work with zoom-in.");
                }
            }
        }
        if (ret) {
            // Invoke callback.
            if (_touchBeganCallback) {
                _touchBeganCallback(touch, event);
            }
        }
    }
    _listeningNode->release();
    return ret;
}

void TouchListener::Impl::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    // Check whether its the same position.
    if (touch->getDelta() == cocos2d::Vec2::ZERO) {
        return;
    }
    // Check whether touch should be cancelled.
    if (_isCancelTouchEnd) {
        float delta = std::max(std::abs(touch->getDelta().x), std::abs(touch->getDelta().y));
        if (_moveThreshold < delta) {
            _shouldCancel = true;
        }
    }
    _listeningNode->retain();
    bool wasInside = _isInside;
    _isInside = _insideChecker(touch);
    if (wasInside != _isInside) {
        // Touch inside type.
        if (_touchType == TouchType::Inside) {
            if (_isZoom && _zoomScaleRatio != 1.0f) {
                _listeningNode->stopActionByTag(ZoomActionTag);
                _listeningNode->stopActionByTag(OtherActionTag);
                float finalScaleX = _baseScaleX * _zoomScaleRatio;
                float finalScaleY = _baseScaleY * _zoomScaleRatio;
                float currentScaleX = _listeningNode->getScaleX();
                if (_isInside) {
                    float ratio = (finalScaleX - currentScaleX) / (finalScaleX - _baseScaleX);
                    auto press = cocos2d::ScaleTo::create(_zoomDuration * ratio, finalScaleX, finalScaleY);
                    press->setTag(ZoomActionTag);
                    _listeningNode->runAction(press);
                } else {
                    float ratio = (currentScaleX - _baseScaleX) / (finalScaleX - _baseScaleX);
                    auto normal = cocos2d::ScaleTo::create(_zoomDuration * ratio, _baseScaleX, _baseScaleY);
                    normal->setTag(OtherActionTag);
                    _listeningNode->runAction(normal);
                }
            }
            // Update state.
            if (_isInside) {
                _base->updateState(ButtonState::Pressed);
            } else {
                _base->updateState(ButtonState::Normal);
            }
        } else {
            // Touch outside type does nothing.
            if (_isZoom) {
                CCLOG("Touch outside does not work with zoom-in.");
            }
        }
    }
    // Invoke callback.
    if (_touchMovedCallback) {
        _touchMovedCallback(touch, event);
    }
    _listeningNode->release();
}

void TouchListener::Impl::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    _listeningNode->retain();
    // Touch inside type.
    if (_touchType == TouchType::Inside) {
        // Run zoom action.
        if (_isZoom && _zoomScaleRatio != 1.0f) {
            if (_isInside) {
                auto currentAction = static_cast<cocos2d::ScaleTo*>(_listeningNode->getActionByTag(ZoomActionTag));
                cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
                actions.reserve(3);
                if (currentAction != nullptr && currentAction->isDone() == false) {
                    float remaining = _zoomDuration - currentAction->getElapsed();
                    auto delay = cocos2d::DelayTime::create(remaining);
                    actions.pushBack(delay);
                }
                auto normal = cocos2d::ScaleTo::create(_zoomDuration, _baseScaleX, _baseScaleY);
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
                _listeningNode->runAction(sequence);
            }
        } else {
            if (_isInside) {
                if (_shouldCancel == false) {
                    if (_touchUpCallback) {
                        _touchUpCallback(touch, event);
                    }
                }
                TouchManager::getInstance()->unlock(touch);
            }
        }
    } else {
        if (_isZoom) {
            CCLOG("Touch outside does not work with zoom-in.");
        }
        if (_isInside == false) {
            if (_shouldCancel == false) {
                if (_touchUpCallback) {
                    _touchUpCallback(touch, event);
                }
            }
        }
    }
    // Update state.
    if (_isInside) {
        _base->updateState(ButtonState::Normal);
    }
    _listeningNode->release();
}

void TouchListener::Impl::onTouchCanceled(cocos2d::Touch* touch, cocos2d::Event* event) {
    TouchManager::getInstance()->unlock(touch);
}
namespace_ee_end