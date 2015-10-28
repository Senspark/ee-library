#include "EEGesture-Impl.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_anonymous_begin
const cocos2d::Size& getWinSize() {
    return cocos2d::Director::getInstance()->getWinSize();
}
namespace_anonymous_end

Gesture::Gesture() : _impl(new Impl(this)) {}
Gesture::~Gesture() {}

Gesture::Impl::Impl(Gesture* ptr)
    : _ptr(ptr)
    , _eyeOffset(std::numeric_limits<float>::lowest())
    , _horizonOffset(std::numeric_limits<float>::max())
    , _eyePosition(getWinSize() * 0.5f)
    , _zoom(1)
    , _viewSize(getWinSize())
    , _isPaused(false)
    , _currentGesture(GestureType::None)
    , _lastTouchPosition(cocos2d::Point::ZERO)
    , _lastDistanceBetweenTouches(0)
    , _isAutoPanning(false)
    , _mainLayer(nullptr)
    , _autoPanVectorLength(0)
    , _autoPanVectorNormalized(cocos2d::Vec2::ZERO) {}

Gesture::Impl::~Impl() {
    for (auto touch : _activeTouches) {
        touch->release();
    }
}

void Gesture::setHorizonOffset(float offset) {
    _impl->_horizonOffset = offset;
}

void Gesture::addLayer(cocos2d::Node* layer, float offset) {
    CC_ASSERT(layer != nullptr);
    _impl->_layerList.emplace_back(layer, offset);

    if (offset == 0.0f) {
        CC_ASSERT(_impl->_mainLayer == nullptr);
        _impl->_mainLayer = layer;
    }
}

void Gesture::update(float delta) {
    if (_impl->_isAutoPanning) {
        if (_impl->_autoPanVectorLength <= 0) {
            _impl->_isAutoPanning = false;
        } else {
            auto&& direction = _impl->_autoPanVectorNormalized * _impl->_autoPanVectorLength;
            _impl->_autoPanVectorLength = _impl->_autoPanVectorLength - getAutoPanDistDecay();
            // Move the eye position.
            _impl->_eyePosition = _impl->_eyePosition - direction / _impl->_zoom * (-_impl->_eyeOffset + _impl->_horizonOffset) / _impl->_horizonOffset;
            // Clamp the eye position.
            // Check whether the eye position is at the border.
            if (_impl->clampEyePosition()) {
                _impl->_isAutoPanning = false;
            }
            // Update layers' position.
            _impl->updateLayers();
        }
    }
}

void Gesture::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    // Add touches to active touches.
    _impl->_activeTouches.reserve(_impl->_activeTouches.size() + touches.size());
    for (auto touch : touches) {
        touch->retain();
        _impl->_activeTouches.push_back(touch);
    }
    // Disable the auto-pan feature.
    _impl->_isAutoPanning = false;
}

void Gesture::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    if (_impl->_activeTouches.empty()) {
        return;
    }

    // Check whether any touch is inactive.
    std::size_t count = 0;
    for (auto touch : _impl->_activeTouches) {
        if (touch->getReferenceCount() > 1) {
            _impl->_activeTouches.at(count++) = touch;
        } else {
            touch->release();
        }
    }
    _impl->_activeTouches.resize(count);

    // Handle gestures.
    if (_impl->_activeTouches.size() == 1) {
        // There is only one active touch on the screen.
        // So we have to handle pan gesture.
        // Check whether there isn't any active gesture or the active gesture is pan.
        if (_impl->_currentGesture == Impl::GestureType::None || _impl->_currentGesture == Impl::GestureType::Pan) {
            // Check whether the current zoom is larger than 1, i.e. eyeOffset is float min.
            if (_impl->_eyeOffset != std::numeric_limits<float>::lowest()) {
                // Get the touch pointer.
                auto touch = _impl->_activeTouches.front();
                // Get the current touch position.
                auto&& position = touch->getLocation();
                // Check whether there isn't any previous pan gesture.
                if (_impl->_currentGesture == Impl::GestureType::Pan) {
                    // This is the delta position on view layer.
                    auto&& delta = position - _impl->_lastTouchPosition;
                    float distSq = delta.getLengthSq();
                    if (distSq > getPanThreshold()) {
                        // Move the eye position.
                        _impl->_eyePosition = _impl->_eyePosition - delta / _impl->_zoom * (-_impl->_eyeOffset + _impl->_horizonOffset) / _impl->_horizonOffset;
                        // Clamp the eye position.
                        _impl->clampEyePosition();
                        // Update layers' position.
                        _impl->updateLayers();
                    }
                    _impl->_lastPanDelta = delta;
                }
                // Change the active gesture to pan.
                _impl->_currentGesture = Impl::GestureType::Pan;
                // Assign last touch position to the current position.
                _impl->_lastTouchPosition = position;
            }
        }
    } else if (_impl->_activeTouches.size() == 2) {
        // There are two touches on the screen.
        // So we have to handle pinch gesture.
        // Pinch gesture has higher priority then pan gesture.
        // Gets the current two touches.
        auto touchA = _impl->_activeTouches.at(0);
        auto touchB = _impl->_activeTouches.at(1);

        auto&& positionA = touchA->getLocation();
        auto&& positionB = touchB->getLocation();
        float distSq = positionA.getDistanceSq(positionB);

        // Check whether the previous gesture is pinch.
        if (_impl->_currentGesture != Impl::GestureType::Pinch) {
            _impl->_currentGesture = Impl::GestureType::Pinch;
            _impl->_lastDistanceBetweenTouches = distSq;
            return;
        }

        if (std::abs(_impl->_lastDistanceBetweenTouches - distSq) <= getPinchThreshold()) {
            _impl->_lastDistanceBetweenTouches = distSq;
            return;
        }

        // We store distance as square.
        // So we have to use square root here.
        float ratio = std::sqrt(distSq / _impl->_lastDistanceBetweenTouches);
        _impl->_lastDistanceBetweenTouches = distSq;

        _impl->zoomTo(_impl->_zoom * ratio, (positionA + positionB) * 0.5f);
    } else {
        CCLOG("There are more than 2 touches on the screen, ignored.");
    }

    // CCLOG("zoom = %f eyeOffset = %f eyePosition = %f %f.", _zoom, _eyeOffset, _eyePosition.x, _eyePosition.y);
}

void Gesture::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    if (_impl->_activeTouches.empty()) {
        return;
    }

    // Remove touches from active touches.
    for (auto touch : touches) {
        auto iter = std::find(_impl->_activeTouches.cbegin(), _impl->_activeTouches.cend(), touch);
        if (iter != _impl->_activeTouches.cend()) {
            _impl->_activeTouches.erase(iter);
            touch->release();
        }
    }

    // Check whether auto-pan feature should be enabled.
    // Check whether the last gesture is the pan gesture.
    if (_impl->_currentGesture == Impl::GestureType::Pan) {
        float distSq = _impl->_lastPanDelta.getLengthSq();
        if (distSq > _impl->getAutoPanThresholdSq()) {
            CCLOG("Anto-pan enabled.");
            _impl->_isAutoPanning = true;
            _impl->_autoPanVectorNormalized = _impl->_lastPanDelta.getNormalized();
            if (distSq < _impl->getAutoPanMaxDistSq()) {
                _impl->_autoPanVectorLength = std::sqrt(distSq);
            } else {
                _impl->_autoPanVectorLength = getAutoPanMaxDist();
            }
        }
    }

    // Reset.
    _impl->_currentGesture = Impl::GestureType::None;
}

void Gesture::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    // Remove touches from active touches.
     for (auto touch : touches) {
         _impl->_activeTouches.erase(std::find(_impl->_activeTouches.cbegin(), _impl->_activeTouches.cend(), touch));
     }

    // Reset.
    _impl->_currentGesture = Impl::GestureType::None;
}

void Gesture::pauseGesture() {
    _impl->_isPaused = true;

    // Reset.
    _impl->_currentGesture = Impl::GestureType::None;

    // Remove all touches.
    for (auto touch : _impl->_activeTouches) {
        touch->release();
    }
    _impl->_activeTouches.clear();
}

void Gesture::resumeGesture() {
    _impl->_isPaused = false;
}
    
void Gesture::setZoomValue(float value) {
    _impl->zoomTo(value);
}
    
void Gesture::lookAt(const cocos2d::Point& position) {
    cocos2d::Point center(getWinSize() * 0.5f);
    // (eyePosition - position) / (position - center) = -eyeOffset / horizonOffset.
    // eyePosition - position = (-eyeOffset / horizonOffset) * (position - center).
    // eyePosition = (-eyeOffset / horizonOffset) * (position - center) + position.
    _impl->_eyePosition = (-_impl->_eyeOffset / _impl->_horizonOffset) * (position - center) + position;
    _impl->clampEyePosition();
    _impl->updateLayers();
}
    
void Gesture::setViewSize(const cocos2d::Size& size) {
    _impl->_viewSize = size;
}

void Gesture::Impl::zoomTo(float zoom, const cocos2d::Point& centerPosition /* = cocos2d::Point::ZERO */) {
    float oldZoom = _zoom;
    
    // Clamp zoom.
    _zoom = cocos2d::clampf(zoom, _ptr->getMinZoomValue(), _ptr->getMaxZoomValue());
    if (_zoom <= 1.0f + FLT_EPSILON) {
        _zoom = 1.0f;
        _eyeOffset = std::numeric_limits<float>::lowest();
        // Mimimal zoom case, i.e. currentZoom is equal to 1.0.
        for (auto&& pair : _layerList) {
            auto layer = pair.first;
            layer->setVisible(true);
            if (layer == _mainLayer) {
                // Main layer has anchor point of (0, 0).
                layer->setPosition(cocos2d::Point::ZERO);
            } else {
                // Sub layer has anchor point of (0.5, 0.5).
                layer->setPosition(getWinSize() * 0.5f);
            }
            layer->setScale(1.0f, 1.0f);
        }
    } else {
        _eyeOffset = _horizonOffset / (1 - _zoom);

        if (centerPosition != cocos2d::Point::ZERO) {
            auto&& localScaleCenter = _mainLayer->convertToNodeSpace(centerPosition);
            auto&& oldCenterPoint = localScaleCenter * _mainLayer->getScale();
            auto&& newCenterPoint = localScaleCenter * _zoom;
            auto&& centerPointDelta = oldCenterPoint - newCenterPoint;
            auto&& newPosition = _mainLayer->getPosition() + centerPointDelta;

            // Update eye position.
            _eyePosition = newPosition / (1 - _zoom);
        } else if (oldZoom > 1.0f) {
            auto&& oldNewPosition = _eyePosition * (1 - oldZoom);
            _eyePosition = oldNewPosition / (1 - _zoom);
        }

        // Clamp eye position.
        clampEyePosition();

        // Update layers' position and scale.
        updateLayers();
    }
}

bool Gesture::Impl::clampEyePosition() {
    bool ret = false;
    float widthRatio = 1.0f;
    float heightRatio = 1.0f;
    if (_viewSize.width + FLT_EPSILON < getWinSize().width) {
        // -minEyeOffset / (-minEyeOffset + horizonOffset) = 1 / getMinZoomValue.
        // getMinZoomValue * minEyeOffset = minEyeOffset - horizonOffset.
        // minEyeOffset * (getMinZoomValue - 1) = -horizonOffset.
        // minEyeOffset = -horizonOffset / (getMinZoomValue - 1).
        float minEyeOffset = -_horizonOffset / ((getWinSize().width / _viewSize.width) - 1);
        // ratio = (eyeOffset - minEyeOffset) / (-minEyeOffset + horizonOffset).
        widthRatio = (_eyeOffset - minEyeOffset) / (-minEyeOffset + _horizonOffset);
    }
    if (_viewSize.height + FLT_EPSILON < getWinSize().height) {
        float minEyeOffset = -_horizonOffset / ((getWinSize().height / _viewSize.height) - 1);
        heightRatio = (_eyeOffset - minEyeOffset) / (-minEyeOffset + _horizonOffset);
    }
    float maxX = getWinSize().width * widthRatio;
    float maxY = getWinSize().height * heightRatio;
    _eyePosition.x = cocos2d::clampf(_eyePosition.x, 0, maxX);
    _eyePosition.y = cocos2d::clampf(_eyePosition.y, 0, maxY);
    if (_ptr->doesStopAutoPanWhenCollideVerticalBorders() && (_eyePosition.x <= FLT_EPSILON || maxX - FLT_EPSILON <= _eyePosition.x)) {
        // Collide with vertical borders.
        if(_eyePosition.x <= FLT_EPSILON) {
            _ptr->onCollideWithBorders(Border::Left);
        }
        else {
            _ptr->onCollideWithBorders(Border::Right);
        }
        ret = true;
    }
    if (_ptr->doesStopAutoPanWhenCollideHorizontalBorders() && (_eyePosition.y <= FLT_EPSILON || maxY - FLT_EPSILON <= _eyePosition.y)) {
        // Collide with horizontal borders.
        if(_eyePosition.y <= FLT_EPSILON) {
            _ptr->onCollideWithBorders(Border::Bottom);
        }
        else {
            _ptr->onCollideWithBorders(Border::Top);
        }
        ret = true;
    }
    
    return ret;
}
    
void Gesture::Impl::updateLayers() {
    // Main layer should be the first element in the layer list.
    if (_mainLayer != _layerList.front().first) {
        auto iter = std::find_if(_layerList.begin(), _layerList.end(), [this](const std::pair<cocos2d::Node*, float>& pair) {
            return pair.first == _mainLayer;
        });
        std::iter_swap(iter, _layerList.begin());
    }
    
    for (auto&& pair : _layerList) {
        auto layer = pair.first;
        float offset = pair.second;
        if (offset * _ptr->getViewDistanceRatio() <= _eyeOffset) {
            // The layer is behind the eyes.
            layer->setVisible(false);
        } else {
            // The layer is in front of the eyes.
            layer->setVisible(true);
            float layerZoom = (-_eyeOffset + _horizonOffset) / (-_eyeOffset + offset);
            
            float minX = 0;
            float minY = 0;
            if (_eyePosition.x > FLT_EPSILON) {
                // Calculate minX.
                minX = _eyePosition.x - _eyePosition.x * layerZoom;
            }
            if (_eyePosition.y > FLT_EPSILON) {
                // Calculate minY.
                minY = _eyePosition.y - _eyePosition.y * layerZoom;
            }
            
            auto&& layerSize = getWinSize() * layerZoom;
            cocos2d::Point position(minX + layerSize.width * layer->getAnchorPoint().x, minY + layerSize.height * layer->getAnchorPoint().y);
            if (layer == _mainLayer) {
                layer->setScale(_zoom, _zoom);
                layer->setPosition(position);
            } else {
                // Sub layer is in the main layer.
                // So we have to divide its scale by the main layer's scale.
                if (layer->getParent() == _mainLayer) {
                    position = _mainLayer->convertToNodeSpace(position);
                    layerZoom /= _zoom;
                }
                layer->setScale(layerZoom, layerZoom);
                // Convert the center position to the main layer local position because sub-layer is in main layer.
                layer->setPosition(position);
            }
        }
    }
}
namespace_anonymous_end
