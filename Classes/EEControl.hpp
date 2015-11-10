#ifndef EE_LIBRARY_CONTROL_HPP
#define EE_LIBRARY_CONTROL_HPP

#include "EEForward.hpp"
#include "EETouchListener.hpp"
#include "cocosbuilder/CCSpriteLoader.h"
#include "CCSprite.h"

namespace_ee_begin
/**
 * NodeButton = cocos2d::Node + handle touches.
 */
class NodeButton : public virtual cocos2d::Node, public TouchListener {
public:
    static NodeButton* create();
    
protected:
    virtual bool init() override;
};

/**
 * Button = cocos2d::Sprite + handle touches.
 */
class Button : public cocos2d::Sprite, public TouchListener {
public:
    static Button* create();
    
    cocos2d::SpriteFrame* getSpriteFrameForState(ButtonState state) const;
    void setSpriteFrameForState(ButtonState state, const std::string& spriteFrameName);
    void setSpriteFrameForState(ButtonState state, cocos2d::SpriteFrame* spriteFrame);
        
protected:
    virtual ~Button();
    
    virtual bool init() override;
    
    using TouchListener::updateState;
    virtual void updateState(ButtonState state) override;
    
private:
    cocos2d::SpriteFrame* _normalSpriteFrame;
    cocos2d::SpriteFrame* _pressedSpriteFrame;
    cocos2d::SpriteFrame* _disabledSpriteFrame;
};
namespace_ee_end

#endif // EE_LIBRARY_CONTROL_HPP
