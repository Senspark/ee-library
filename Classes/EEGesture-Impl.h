//
//  EEGesture-Impl.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/24/15.
//
//

#ifndef EE_LIBRARY_GESTURE_IMPL_H
#define EE_LIBRARY_GESTURE_IMPL_H

#include "EEGesture.h"
#include "math/Vec2.h"
#include "math/CCGeometry.h"

namespace_ee_begin
class Gesture::Impl {
public:
    Impl(Gesture* ptr);
    ~Impl();
    
    Gesture* _ptr;
    
    float getAutoPanThresholdSq() const { return _ptr->getAutoPanThreshold() * _ptr->getAutoPanThreshold(); }
    float getAutoPanMaxDistSq() const { return _ptr->getAutoPanMaxDist() * _ptr->getAutoPanMaxDist(); }
    
    void zoomTo(float zoom, const cocos2d::Point& centerPosition = cocos2d::Point::ZERO);
    void updateLayers();
    bool clampEyePosition();
    
    float _eyeOffset;
    float _horizonOffset;
    cocos2d::Size _viewSize;
    
    bool _isPaused;
    
    enum class GestureType {
        None,
        Pan,
        Pinch,
    } _currentGesture;
    
    bool _isAutoPanning;
    cocos2d::Vec2 _lastPanDelta;
    float _autoPanVectorLength;
    cocos2d::Vec2 _autoPanVectorNormalized;
    
    cocos2d::Point _eyePosition;
    cocos2d::Point _lastTouchPosition;
    
    float _lastDistanceBetweenTouches;
    float _zoom;
    
    std::vector<cocos2d::Touch*> _activeTouches;
    
    // List of layers.
    // Main layer should be LHGameWorldNode and have zero offset.
    cocos2d::Node* _mainLayer;
    std::vector<std::pair<cocos2d::Node*, float>> _layerList;
};
namespace_ee_end

#endif // EE_LIBRARY_GESTURE_IMPL_H