#include "EETouchListener-Impl.h"
#include "EETouchManager.h"
#include "cocos2d.h"

namespace_ee_begin
TouchListenerNode::TouchListenerNode(cocos2d::Node* instance)
: _instance(instance)
, _buttonState(ButtonState::Normal)
, _impl(new Impl(this)) {}

TouchListenerNode::~TouchListenerNode() {}

cocos2d::EventListenerTouchOneByOne* TouchListenerNode::getEventListener() const {
    return _impl->_listener;
}
    
void TouchListenerNode::setHitCount(unsigned count) {
    _impl->_hitCount = count;
}

void TouchListenerNode::setState(int state) {
    _impl->_state = state;
    updateState();
}

int TouchListenerNode::getState() const {
    return _impl->_state;
}

void TouchListenerNode::toggleState(int state) {
    _impl->_state ^= state;
    updateState();
}

void TouchListenerNode::enableState(int state) {
    _impl->_state |= state;
    updateState();
}

void TouchListenerNode::disableState(int state) {
    _impl->_state &= ~state;
    updateState();
}

bool TouchListenerNode::checkState(int state) const {
    return (_impl->_state & state) != TouchState::None;
}

void TouchListenerNode::setCallback(TouchState::Callback callbackType, const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback) {
    switch (callbackType) {
        case TouchState::TouchBegan: _impl->_callbackList[0] = callback; break;
        case TouchState::TouchMoved: _impl->_callbackList[1] = callback; break;
        case TouchState::TouchUpInside: _impl->_callbackList[2] = callback; break;
        case TouchState::TouchUpOutside: _impl->_callbackList[3] = callback; break;
    }
}

const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& TouchListenerNode::getCallback(TouchState::Callback callbackType) const {
    switch (callbackType) {
        case TouchState::TouchBegan: return _impl->_callbackList[0];
        case TouchState::TouchMoved: return _impl->_callbackList[1];
        case TouchState::TouchUpInside: return _impl->_callbackList[2];
        case TouchState::TouchUpOutside: return _impl->_callbackList[3];
    }
    CC_ASSERT(false);
}
    
void TouchListenerNode::setStateChangedCallback(const std::function<void(ButtonState)>& callback) {
    _impl->_stateChangedCallback = callback;
}
    
const std::function<void(ButtonState)>& TouchListenerNode::getStateChangedCallback() const {
    return _impl->_stateChangedCallback;
}

void TouchListenerNode::updateState() {
    CC_ASSERT((_impl->_state | TouchState::TouchBeganInside | TouchState::TouchBeganOutside) != _impl->_state);
    if (isEnabled() == false) {
        updateState(ButtonState::Disabled);
    } else {
        updateState(_buttonState);
    }
}

void TouchListenerNode::updateState(ButtonState state) {
    if (state != ButtonState::Disabled) {
        _buttonState = state;
    }
    if (checkState(TouchState::StateChanged) && getStateChangedCallback() != nullptr) {
        getStateChangedCallback()(state);
    }
}

ButtonState TouchListenerNode::getButtonState() const {
    if (isEnabled()) return _buttonState;
    return ButtonState::Disabled;
}

void TouchListenerNode::setBaseScale(float scale) {
    setBaseScale(scale, scale);
}
    
void TouchListenerNode::setBaseScale(float scaleX, float scaleY) {
    setBaseScaleX(scaleX);
    setBaseScaleY(scaleY);
}
    
void TouchListenerNode::setBaseScaleX(float scaleX) {
    _impl->_baseScaleX = scaleX;
}
    
void TouchListenerNode::setBaseScaleY(float scaleY) {
    _impl->_baseScaleY = scaleY;
}

TouchListenerNode::Impl::Impl(TouchListenerNode* base)
: _state(TouchState::None)
, _isInside(false)
, _listener(nullptr)
, _hitCount(std::numeric_limits<unsigned>::max())
, _baseScaleX(1.0f)
, _baseScaleY(1.0f)
, _base(base) {
    addListener();
}

void TouchListenerNode::Impl::addListener() {
    const int ScaleUpTag = 999;
    const int OtherActionTag = 1000;
    
    _listener = cocos2d::EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = [this, ScaleUpTag, OtherActionTag](cocos2d::Touch* touch, cocos2d::Event* event) {
        bool ret = false;
        _base->_instance->retain();
        if (TouchManager::getInstance()->isLocked() == false && _hitCount > 0) {
            // Check whether the button is enabled.
            if (_base->checkState(TouchState::Enable)) {
                // Check whether the button is actually visible.
                if (_base->isActuallyVisible()) {
                    // Check whether the touch position is inside the button area.
                    _isInside = _base->isTouchInside(touch);
                    // Touch began have to be inside to trigger the event.
                    if (_base->checkState(TouchState::TouchBeganInside)) {
                        if ((ret = _isInside)) {
                            // Touch began callback.
                            if (_base->checkState(TouchState::TouchBegan) && _base->getCallback(TouchState::TouchBegan) != nullptr) {
                                _base->getCallback(TouchState::TouchBegan)(touch, event);
                            }
                            // Check whether the button supports zoom-in feature.
                            if (_base->checkState(TouchState::Zoom)) {
                                // Stop all zoom-in related actions.
                                _base->_instance->stopActionByTag(OtherActionTag);
                                _base->_instance->stopActionByTag(ScaleUpTag);
                                // Create zoom-in action.
                                auto press = cocos2d::ScaleTo::create(_base->getScaleDuration(), _baseScaleX * _base->getScaleRatio(), _baseScaleY * _base->getScaleRatio());
                                press->setTag(ScaleUpTag);
                                _base->_instance->runAction(press);
                            }
                            // Check whether the button supports toggle feature.
                            if (_base->checkState(TouchState::Toggle)) {
                                _base->updateState(ButtonState::Pressed);
                            }
                        }
                    } else if (_base->checkState(TouchState::TouchBeganOutside)) {
                        // Touch began have to be outside to trigger the event.
                        if ((ret = (_isInside == false))) {
                            // Touch began callback.
                            if (_base->checkState(TouchState::TouchBegan) && _base->getCallback(TouchState::TouchBegan) != nullptr) {
                                _base->getCallback(TouchState::TouchBegan)(touch, event);
                            }
                        }
                    }
                }
            }
        }
        if (ret) {
            TouchManager::getInstance()->lock(touch);
        }
        _base->_instance->release();
        return ret;
    };
    
    _listener->onTouchMoved = [this, ScaleUpTag, OtherActionTag](cocos2d::Touch* touch, cocos2d::Event* event) {
        // Detect same touch location.
        if (touch->getDelta() != cocos2d::Vec2::ZERO) {
            // Check whether the button is actually visible.
            if (_base->isActuallyVisible()) {
                _base->_instance->retain();
                CC_ASSERT(_base->checkState(TouchState::Enable));
                // Touch moved callback.
                if (_base->checkState(TouchState::TouchMoved) && _base->getCallback(TouchState::TouchMoved) != nullptr) {
                    _base->getCallback(TouchState::TouchMoved)(touch, event);
                }
                bool wasInside = _isInside;
                _isInside = _base->isTouchInside(touch);
                // Check whether the button supports zoom-in or toggle feature.
                if (_base->checkState(TouchState::Zoom) || _base->checkState(TouchState::Toggle)) {
                    if (_isInside != wasInside) {
                        // Check whether the button supports zoom-in feature.
                        if (_base->checkState(TouchState::Zoom)) {
                            // Stop all zoom-in related actions.
                            _base->_instance->stopActionByTag(OtherActionTag);
                            _base->_instance->stopActionByTag(ScaleUpTag);
                            if (_isInside) {
                                float ratio = (_baseScaleX * _base->getScaleRatio() - _base->_instance->getScaleX()) / (_baseScaleX * _base->getScaleRatio() - _baseScaleX);
                                auto press = cocos2d::ScaleTo::create(_base->getScaleDuration() * ratio, _baseScaleX * _base->getScaleRatio(), _baseScaleY * _base->getScaleRatio());
                                press->setTag(ScaleUpTag);
                                _base->_instance->runAction(press);
                            } else {
                                float ratio = (_base->_instance->getScaleX() - _baseScaleX) / (_baseScaleX * _base->getScaleRatio() - _baseScaleX);
                                auto normal = cocos2d::ScaleTo::create(_base->getScaleDuration() * ratio, _baseScaleX, _baseScaleY);
                                normal->setTag(OtherActionTag);
                                _base->_instance->runAction(normal);
                            }
                        }
                        // Check whether the button supports toggle feature.
                        if (_base->checkState(TouchState::Toggle)) {
                            if (_isInside) {
                                _base->updateState(ButtonState::Pressed);
                            } else {
                                _base->updateState(ButtonState::Normal);
                            }
                        }
                    }
                }
                _base->_instance->release();
            }
        }
    };
    
    _listener->onTouchCancelled = _listener->onTouchEnded = [this, ScaleUpTag, OtherActionTag](cocos2d::Touch* touch, cocos2d::Event* event) {
        CC_ASSERT(_base->checkState(TouchState::Enable));
        // Check whether the button is actually visible.
        if (_base->isActuallyVisible()) {
            // Check whether the button supports zoom-in feature.
            _base->_instance->retain();
            if (_base->checkState(TouchState::Zoom)) {
                if (_isInside) {
                    // Get the current zoom-in action.
                    auto currentAction = static_cast<cocos2d::ScaleTo*>(_base->_instance->getActionByTag(ScaleUpTag));
                    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
                    actions.reserve(3);
                    if (currentAction != nullptr) {
                        if (currentAction->isDone() == false) {
                            float remaining = _base->getScaleDuration() - currentAction->getElapsed();
                            auto delay = cocos2d::DelayTime::create(remaining);
                            actions.pushBack(delay);
                        }
                    }
                    auto normal = cocos2d::ScaleTo::create(_base->getScaleDuration(), _baseScaleX, _baseScaleY);
                    actions.pushBack(normal);
                    auto callFunc = cocos2d::CallFunc::create([touch, event, this]() {
                        if (_base->checkState(TouchState::TouchUpInside) && _base->getCallback(TouchState::TouchUpInside) != nullptr) {
                            _base->getCallback(TouchState::TouchUpInside)(touch, event);
                        }
                        --_hitCount;
                        TouchManager::getInstance()->unlock();
                    });
                    actions.pushBack(callFunc);
                    auto sequence = cocos2d::Sequence::create(actions);
                    sequence->setTag(OtherActionTag);
                    _base->_instance->runAction(sequence);
                }
            }
            // Check whether the button supports toggle feature.
            if (_base->checkState(TouchState::Toggle)) {
                if (_isInside) {
                    _base->updateState(ButtonState::Normal);
                }
            }
            if (_base->checkState(TouchState::Zoom) == false) {
                if (_isInside) {
                    if (_base->checkState(TouchState::TouchUpInside) && _base->getCallback(TouchState::TouchUpInside) != nullptr) {
                        --_hitCount;
                        _base->getCallback(TouchState::TouchUpInside)(touch, event);
                    }
                }
            }
            if (_isInside == false) {
                if (_base->checkState(TouchState::TouchUpOutside) && _base->getCallback(TouchState::TouchUpOutside) != nullptr) {
                    --_hitCount;
                    _base->getCallback(TouchState::TouchUpOutside)(touch, event);
                }
            }
            if ((_base->checkState(TouchState::Zoom) && _isInside) == false) {
                TouchManager::getInstance()->unlock();
            }
            _isInside = false;
            _base->_instance->release();
        }
    };
    
    _base->_instance->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, _base->_instance);
}

bool TouchListenerNode::isTouchInside(cocos2d::Touch* touch) const {
    auto&& position = _instance->getParent()->convertToNodeSpace(touch->getLocation());
    auto&& box = _instance->getBoundingBox();
    return box.containsPoint(position);
}
    
bool TouchListenerNode::isActuallyVisible() const {
    bool ret = true;
    for (auto node = _instance; node != nullptr && ret; node = node->getParent()) {
        ret = node->isVisible();
    }
    return ret;
}

TouchListenerSprite::TouchListenerSprite(cocos2d::Sprite* instance)
: TouchListenerNode(instance)
, _impl(new Impl()) {}

TouchListenerSprite::~TouchListenerSprite() {}

TouchListenerSprite::Impl::Impl()
: _normalSpriteFrame(nullptr)
, _pressedSpriteFrame(nullptr)
, _disabledSpriteFrame(nullptr) {}

TouchListenerSprite::Impl::~Impl() {
    if (_normalSpriteFrame != nullptr) {
        _normalSpriteFrame->release();
    }
    if (_pressedSpriteFrame != nullptr) {
        _pressedSpriteFrame->release();
    }
    if (_disabledSpriteFrame != nullptr) {
        _disabledSpriteFrame->release();
    }
}

cocos2d::SpriteFrame* TouchListenerSprite::getSpriteFrameForState(ButtonState state) const {
    cocos2d::SpriteFrame* ret = nullptr;
    switch (state) {
        case ButtonState::Normal: ret = _impl->_normalSpriteFrame; break;
        case ButtonState::Pressed: ret = _impl->_pressedSpriteFrame; break;
        case ButtonState::Disabled: ret = _impl->_disabledSpriteFrame; break;
    }
    return ret;
}

void TouchListenerSprite::setSpriteFrameForState(ButtonState state, const std::string& spriteFrameName) {
    setSpriteFrameForState(state, cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName));
}

void TouchListenerSprite::setSpriteFrameForState(ButtonState state, cocos2d::SpriteFrame* spriteFrame) {
    cocos2d::SpriteFrame** stateSpriteFrame = nullptr;
    switch (state) {
        case ButtonState::Normal: stateSpriteFrame = &_impl->_normalSpriteFrame; break;
        case ButtonState::Pressed: stateSpriteFrame = &_impl->_pressedSpriteFrame; break;
        case ButtonState::Disabled: stateSpriteFrame = &_impl->_disabledSpriteFrame; break;
    }
    if (*stateSpriteFrame != nullptr) {
        (*stateSpriteFrame)->release();
    }
    (*stateSpriteFrame) = spriteFrame;
    (*stateSpriteFrame)->retain();
    updateState();
}

void TouchListenerSprite::updateState(ButtonState state) {
    TouchListenerNode::updateState(state);
    auto sprite = getSpriteFrameForState(state);
    if (sprite != nullptr) {
        static_cast<cocos2d::Sprite*>(_instance)->setSpriteFrame(sprite);
    }
}
namespace_ee_end