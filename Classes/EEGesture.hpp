#ifndef EE_LIBRARY_GESTURE_H
#define EE_LIBRARY_GESTURE_H

#include "EEForward.hpp"
#include "EEMacro.hpp"

#include <vector>
#include <memory>
#include <utility>

namespace_ee_begin
class Gesture {
protected:
    Gesture();
    virtual ~Gesture();

    /// Sets the horizon offset from the main layer.
    void setHorizonOffset(float offset);

    /// Adds a parallax layer to the list.
    void addLayer(cocos2d::Node* layer, float offset);

    /// Call this function in derived class for auto pan feature.
    virtual void update(float delta) = 0;

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) = 0;
    
    void setViewSize(const cocos2d::Size& size);

    void pauseGesture();
    void resumeGesture();
    
    void setZoomValue(float value);
    void lookAt(const cocos2d::Point& position);
    
    enum class Border {
        Top,
        Left,
        Right,
        Bottom,
    };
    
    virtual void onCollideWithBorders(Border collidedBorder) {};

    virtual float getMinZoomValue() const { return 1.0f; }
    virtual float getMaxZoomValue() const { return 3.0f; }
    virtual float getViewDistanceRatio() const { return 5.0f; }

    virtual float getPanThreshold() const { return 0.5f; }
    virtual float getPinchThreshold() const { return 0.5f; }

    virtual float getAutoPanThreshold() const { return 10.0f; }
    virtual float getAutoPanMaxDist() const { return 20.0f; }
    virtual float getAutoPanDistDecay() const { return 1.0f; }
    
    virtual bool doesStopAutoPanWhenCollideHorizontalBorders() const { return true; }
    virtual bool doesStopAutoPanWhenCollideVerticalBorders() const { return true; }

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
namespace_ee_end

#endif // EE_LIBRARY_GESTURE_H
