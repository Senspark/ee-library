//
//  EEControlLoader.hpp
//  ee-library
//
//  Created by enrevol on 10/29/15.
//
//

#ifndef EE_LIBRARY_CONTROL_LOADER_HPP
#define EE_LIBRARY_CONTROL_LOADER_HPP

#include "EEMacro.hpp"
#include "cocosbuilder/CCSpriteLoader.h"

namespace_ee_begin
class NodeButton;
class Button;

class NodeButtonLoader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NodeButtonLoader, loader);
protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader) override;
    virtual void onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) override;
};

class ButtonLoader : public cocosbuilder::SpriteLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ButtonLoader, loader);
protected:
    virtual cocos2d::Sprite* createNode(cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader) override;
    virtual void onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) override;
};
namespace_ee_end

#endif /* EE_LIBRARY_CONTROL_LOADER_HPP */
