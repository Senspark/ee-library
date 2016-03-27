//
//  EEGesture__.cpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#include "Gesture.hpp"
#include "cocos2d.h"

namespace_ee_begin

inline NAMESPACE_BEGIN(v2)
Gesture::Gesture()
{}

Gesture::~Gesture() {
}

void Gesture::setHorizonOffset(float offset) {
    _impl->_horizonOffset = offset;
}

void Gesture::addLayer(cocos2d::Node* layer, float offset, const cocos2d::Point& position) {
    _impl->_layers.emplace_back(layer, offset, position);
}

void Gesture::update(float delta) {
    if (_impl->_isAutoPanning) {

    }
}

void Gesture::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    // Add touches to active touches.
    for (auto touch : touches) {
        touch->retain();
    }
    _impl->_activeTouches.insert(_impl->_activeTouches.cend(), touches.cbegin(), touches.cend());
    // Disable auto-pan.
    _impl->_isAutoPanning = false;
}

void Gesture::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    do {
        CC_BREAK_IF(_impl->_activeTouches.empty());
        // Remove inactive touches.
        auto iter = std::remove_if(_impl->_activeTouches.begin(), _impl->_activeTouches.end(), [](cocos2d::Touch* touch) {
            bool ret = false;
            if (touch->getReferenceCount() == 1) {
                touch->release();
                ret = true;
            }
            return ret;
        });
        _impl->_activeTouches.erase(iter, _impl->_activeTouches.cend());
        if (_impl->_activeTouches.size() == 1) {
            if (_impl->_currentGesture == Impl::GestureType::None ||
                _impl->_currentGesture == Impl::GestureType::Pan) {
                if (_impl->_eyeOffset != std::numeric_limits<float>::lowest()) {
                    auto touch = _impl->_activeTouches.front();
                    auto&& position = touch->getLocation();
                    if (_impl->_currentGesture == Impl::GestureType::Pan) {
                        
                    }
                }
            }
        }
    } while (false);
}
NAMESPACE_END(v2)
namespace_ee_end