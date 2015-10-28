#ifndef EE_LIBRARY_CONTROL_H
#define EE_LIBRARY_CONTROL_H

#include "EEForward.hpp"
#include "EETouchListener.hpp"
#include "cocosbuilder/CCSpriteLoader.h"
#include "2d/CCSprite.h"

namespace_ee_begin
class NodeButton : public cocos2d::Node, public TouchListenerNode {
public:
    CREATE_FUNC(NodeButton);
    NodeButton() : TouchListenerNode(this) {}
};

class NodeButtonLoader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NodeButtonLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(NodeButton);
    virtual void onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) override;
};

class Button : public cocos2d::Sprite, public TouchListenerSprite {
public:
    CREATE_FUNC(Button);
    Button() : TouchListenerSprite(this) {}
};

class ButtonLoader : public cocosbuilder::SpriteLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ButtonLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(Button);
    virtual void onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) override;
    virtual void onHandlePropTypeSpriteFrame(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, cocos2d::SpriteFrame* pSpriteFrame, cocosbuilder::CCBReader* ccbReader) override;
};
namespace_ee_end

#endif // EE_LIBRARY_CONTROL_H
