#ifndef EE_LIBRARY_CONTROL_H
#define EE_LIBRARY_CONTROL_H

#include "EEForward.hpp"
#include "EETouchListener.hpp"
#include "cocosbuilder/CCSpriteLoader.h"
#include "CCSprite.h"

namespace_ee_begin
class NodeButton : public cocos2d::Node, public TouchListener {
public:
    CREATE_FUNC(NodeButton);
    
protected:
    virtual cocos2d::Node* getInstance() override { return this; }
};

class Button : public cocos2d::Sprite, public TouchListener {
public:
    CREATE_FUNC(Button);
    
    cocos2d::SpriteFrame* getSpriteFrameForState(ButtonState state) const;
    void setSpriteFrameForState(ButtonState state, const std::string& spriteFrameName);
    void setSpriteFrameForState(ButtonState state, cocos2d::SpriteFrame* spriteFrame);
    
    virtual void setSpriteFrame(cocos2d::SpriteFrame* spriteFrame) override;
    
protected:
    virtual ~Button();
    
    using TouchListener::updateState;
    virtual void updateState(ButtonState state) override;
    
    virtual cocos2d::Node* getInstance() override { return this; }
    
private:
    cocos2d::SpriteFrame* _normalSpriteFrame;
    cocos2d::SpriteFrame* _pressedSpriteFrame;
    cocos2d::SpriteFrame* _disabledSpriteFrame;
};
namespace_ee_end

#endif // EE_LIBRARY_CONTROL_H
