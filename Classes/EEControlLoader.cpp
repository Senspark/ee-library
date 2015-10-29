//
//  EEControlLoader.cpp
//  ee-library
//
//  Created by enrevol on 10/29/15.
//
//

#include "EEControlLoader.hpp"
#include "EEControl.hpp"

namespace_ee_begin
cocos2d::Node* NodeButtonLoader::createNode(cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader) {
    return NodeButton::create();
}

void NodeButtonLoader::onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) {
    NodeLoader::onHandlePropTypeScaleLock(pNode, pParent, pPropertyName, pScaleLock, ccbReader);
    CC_ASSERT(pScaleLock[0] == pScaleLock[1]);
    static_cast<NodeButton*>(pNode)->setBaseScale(pScaleLock[0]);
}

void ButtonLoader::onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) {
    SpriteLoader::onHandlePropTypeScaleLock(pNode, pParent, pPropertyName, pScaleLock, ccbReader);
    CC_ASSERT(pScaleLock[0] == pScaleLock[1]);
    static_cast<Button*>(pNode)->setBaseScale(pScaleLock[0]);
}

cocos2d::Sprite* ButtonLoader::createNode(cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader) {
    return Button::create();
}
namespace_ee_end
