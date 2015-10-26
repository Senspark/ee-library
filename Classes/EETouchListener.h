#ifndef EE_LIBRARY_TOUCH_LISTENER_H
#define EE_LIBRARY_TOUCH_LISTENER_H

#include "EEForward.hpp"
#include "EEMacro.hpp"

#include <functional>
#include <string>

namespace_ee_begin
enum class ButtonState {
    Normal      = 0,
    Pressed     = 1,
    Disabled    = 2,
};
    
struct TouchState {
    enum Feature {
        None                = 0,
        Enable              = 1 << 0,
        Zoom                = 1 << 1,
        Toggle              = 1 << 2,
        TouchBeganInside    = 1 << 3,
        TouchBeganOutside   = 1 << 4,
    };
    enum Callback {
        TouchBegan          = 1 << 5,
        TouchMoved          = 1 << 6,
        TouchUpInside       = 1 << 7,
        TouchUpOutside      = 1 << 8,
        StateChanged        = 1 << 9,
    };
};
    
class TouchListenerNode {
public:
    cocos2d::EventListenerTouchOneByOne* getEventListener() const;
    void setHitCount(unsigned count);
    
    /// Resets the current state mask.
    void resetState() { setState(TouchState::None); }
    
    /// Sets the state to the specified state mask.
    void setState(int state);
    
    /// Gets the current state mask.
    int getState() const;
    
    /// Toggles the specified state mask.
    void toggleState(int state);
    
    /// Enables all the specified state mask.
    void enableState(int state);
    
    /// Disables all the specified state mask.
    void disableState(int state);
    
    /// Checks whether the specified state mask is enabled.
    bool checkState(int state) const;
    
    ButtonState getButtonState() const;
    
    bool isEnabled() const { return checkState(TouchState::Enable); }
    void setEnabled(bool value) { value ? enableState(TouchState::Enable) : disableState(TouchState::Enable); }
    
    virtual void setCallback(TouchState::Callback callbackType, const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback);
    const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& getCallback(TouchState::Callback callbackType) const;
    
    virtual void setTouchBeganCallback(const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback) { setCallback(TouchState::TouchBegan, callback); }
    virtual void setTouchMovedCallback(const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback) { setCallback(TouchState::TouchMoved, callback); }
    virtual void setTouchUpInsideCallback(const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback) { setCallback(TouchState::TouchUpInside, callback); }
    virtual void setTouchUpOutsideCallback(const std::function<void(cocos2d::Touch*, cocos2d::Event*)>& callback) { setCallback(TouchState::TouchUpOutside, callback); }
    
    virtual void setStateChangedCallback(const std::function<void(ButtonState)>& callback);
    const std::function<void(ButtonState)>& getStateChangedCallback() const;
    
    void setBaseScale(float scale);
    void setBaseScale(float scaleX, float scaleY);
    void setBaseScaleX(float scaleX);
    void setBaseScaleY(float scaleY);

protected:
    virtual float getScaleRatio() const { return 1.15f; }
    virtual float getScaleDuration() const { return 0.04f; }
    
    virtual bool isTouchInside(cocos2d::Touch* touch) const;
    virtual bool isActuallyVisible() const;
    
    TouchListenerNode(cocos2d::Node* instance);
    virtual ~TouchListenerNode();
    
    void updateState();
    virtual void updateState(ButtonState state);
    
    cocos2d::Node* _instance;
    
private:
    friend class Impl;
    
    ButtonState _buttonState;
    
    class Impl;
    std::unique_ptr<Impl> _impl;
};

class TouchListenerSprite : public TouchListenerNode {
public:
    cocos2d::SpriteFrame* getSpriteFrameForState(ButtonState state) const;
    void setSpriteFrameForState(ButtonState state, const std::string& spriteFrameName);
    void setSpriteFrameForState(ButtonState state, cocos2d::SpriteFrame* spriteFrame);
    
protected:
    TouchListenerSprite(cocos2d::Sprite* instance);
    virtual ~TouchListenerSprite();
    
    using TouchListenerNode::updateState;
    virtual void updateState(ButtonState state) override;
    
private:
    friend class ButtonLoader;
    
    class Impl;
    std::unique_ptr<Impl> _impl;
};
namespace_ee_end

#endif // EE_LIBRARY_TOUCH_LISTENER_H
