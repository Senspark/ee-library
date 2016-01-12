//
//  EEGesture-Impl__.hpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#ifndef EEGesture_Impl___h
#define EEGesture_Impl___h

#include "EEGesture__.hpp"
#include "EEProjection.hpp"

namespace_ee_begin
inline NAMESPACE_BEGIN(v2)
class Gesture::Impl final {
public:
    Impl(Gesture* base);
    ~Impl();
    
    Gesture* _base;
    
    constexpr float getAutoPanThresholdSq() const {
        return _autoPanThreshold * _autoPanThreshold;
    }
    
    constexpr float getAutoPanMaxDistSq() const {
        return _autoPanMaxDist * _autoPanMaxDist;
    }
    
    void zoomTo(float zoom, const cocos2d::Point& centerPosition);
    
    void updateLayers();
    
    float _eyeOffset;
    float _horizonOffset;
    
    float _autoPanThreshold;
    float _autoPanMaxDist;
    
    bool _isAutoPanning;
    
    detail::Projection _projection;
    
    enum class GestureType {
        None,
        Pan,
        Pinch
    } _currentGesture;
    
    struct LayerInfo {
        cocos2d::Node* layer;
        float offset;
        cocos2d::Point position;
        
        explicit LayerInfo(cocos2d::Node* _layer, float _offset, cocos2d::Point _position)
        : layer(_layer)
        , offset(_offset)
        , position(std::move(_position))
        {}
    };
    
    std::vector<LayerInfo> _layers;
    std::vector<cocos2d::Touch*> _activeTouches;
};
NAMESPACE_END(v2)
namespace_ee_end

#endif /* EEGesture_Impl___h */
