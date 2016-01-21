//
//  EEGesture__.hpp
//  ee-library
//
//  Created by enrevol on 11/25/15.
//
//

#ifndef EEGesture___hpp
#define EEGesture___hpp

#include "EEForward.hpp"
#include "EEMacro.hpp"
#include "cocos2d.h"

#include <vector>

namespace_ee_begin
inline NAMESPACE_BEGIN(v2)
class Gesture {
    Gesture();
    virtual ~Gesture();
    
    /**
     * Sets the horizon offset from the main layer.
     * @param offset The distance from the main layer to horizon.
     */
    void setHorizonOffset(float offset);
    
    /**
     * Adds a parallax layer to the list.
     * @param layer The layer to be added.
     * @param offset The distance from the given layer to the main layer.
     * @param position The position in horizon.
     */
    void addLayer(cocos2d::Node* layer, float offset, const cocos2d::Point& position = cocos2d::Point::ZERO);
    
    virtual void update(float delta) = 0;
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    
    /**
     * Sets the horizon size.
     * @param size The size to be set.
     */
    void setHorizonSize(cocos2d::Size& size);
    
    /**
     *
     *
     */
    void lookAt(const cocos2d::Point& position);
    void setZoomValue(float zoom);
    
    void pauseGesture();
    void resumeGesture();
    
    enum class Border { Top, Left, Right, Bottom };
    
    virtual bool onCollideWithBorder(Border collidedBorder) { return true; }
    
    void setMinZoomValue(float zoom);
    void setMaxZoomValue(float zoom);
    void setViewDistanceRatio(float ratio);
    
    void setPanThreshold(float threshold);
    void setPinchThreshold(float threshold);
    
    void setAutoPanThreshold(float threshold);
    void setAutoPanMaxDist(float dist);
    void setAutoPanDistDecay(float dist);
    
private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
NAMESPACE_END(v2)
namespace_ee_end

#endif /* EEGesture___hpp */
