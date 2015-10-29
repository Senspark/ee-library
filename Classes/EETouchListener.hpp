//
//  EETouchListener__.hpp
//  ee-library
//
//  Created by enrevol on 10/28/15.
//
//

#ifndef EE_LIBRARY_TOUCH_LISTENER_HPP
#define EE_LIBRARY_TOUCH_LISTENER_HPP

#include "EEMacro.hpp"
#include "EEForward.hpp"

#include <functional>
#include <utility>
#include <memory>

namespace_ee_begin
enum class ButtonState {
    Normal,
    Pressed,
    Disabled,
};

enum class TouchType {
    Inside,
    Outside
};

struct TouchState {
    enum {
        Toggle = 1 << 0,
        Zoom   = 1 << 1,
    };
};

class TouchListener {
public:
    using TouchEventCallback = std::function<void(cocos2d::Touch*, cocos2d::Event*)>;
    
    CC_DEPRECATED_ATTRIBUTE cocos2d::EventListenerTouchOneByOne* getEventListener() const;
    
    CC_DEPRECATED_ATTRIBUTE void enableState(int state);
    
    /// Touch type.
    /// Default is inside.
    void setTouchType(TouchType type);
    TouchType getTouchType() const;
    
    ButtonState getButtonState() const;
    
    /// Checks whether the touch listener is enabled.
    /// Default is false.
    bool isEnabled() const;
    void setEnabled(bool enabled);
    
    /// Default is false.
    void setSwallowTouches(bool needSwallow);
    bool isSwallowTouches();
    
    /// Checks whether the touch listener can zoom.
    bool isZoom() const;
    void setZoom(bool enabled);
    
    /// Set base scale used for zoom.
    /// Default is 1.0f.
    void setBaseScale(float scale);
    void setBaseScale(float scaleX, float scaleY);
    void setBaseScaleX(float scale);
    void setBaseScaleY(float scale);
    
    /// Zoom scale.
    /// Default is 1.15f.
    void setZoomScaleRatio(float ratio);
    float getZoomScaleRatio() const;
    
    /// Zoom duration.
    /// Default is 0.04f.
    void setZoomDuration(float duration);
    float getZoomDuration() const;
    
    /// Listening node.
    /// Default is `this`.
    cocos2d::Node* getListeningNode() const;
    void setListeningNode(cocos2d::Node* node);
    
    /// Inside checker.
    void setInsideChecker(const std::function<bool(cocos2d::Touch*)>& checker);
    
    /// Common touch listeners.
    void setTouchBeganCallback(const TouchEventCallback& callback);
    void setTouchMovedCallback(const TouchEventCallback& callback);
    void setTouchUpCallback(const TouchEventCallback& callback);
    
    const TouchEventCallback& getTouchUpCallback() const;
    
    CC_DEPRECATED_ATTRIBUTE void setTouchUpInsideCallback(const TouchEventCallback& callback) {
        setTouchUpCallback(callback);
    }
    
    CC_DEPRECATED_ATTRIBUTE void setTouchUpOutsideCallback(const TouchEventCallback& callback) {
        setTouchUpCallback(callback);
    }
    
    /// Ignores touch end when touch move delta exceeds the given delta.
    /// Defaults is 0.
    void setTouchMovedThreshold(float delta);
    
protected:
    TouchListener();
    virtual ~TouchListener();
    
    void addListener();
    
    void updateState();
    virtual void updateState(ButtonState state);
    
    class Impl;
    friend class Impl;
    std::unique_ptr<Impl> _impl;
};
namespace_ee_end

#endif /* EE_LIBRARY_TOUCH_LISTENER_HPP */
