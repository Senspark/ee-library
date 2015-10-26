#include "EEControl.h"
#include "EETouchListener-Impl.h"

namespace_ee_begin
void NodeButtonLoader::onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) {
    NodeLoader::onHandlePropTypeScaleLock(pNode, pParent, pPropertyName, pScaleLock, ccbReader);
    static_cast<NodeButton*>(pNode)->setBaseScale(pScaleLock[0], pScaleLock[1]);
}

void ButtonLoader::onHandlePropTypeScaleLock(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, float* pScaleLock, cocosbuilder::CCBReader* ccbReader) {
    SpriteLoader::onHandlePropTypeScaleLock(pNode, pParent, pPropertyName, pScaleLock, ccbReader);
    static_cast<Button*>(pNode)->setBaseScale(pScaleLock[0], pScaleLock[1]);
}
    
void ButtonLoader::onHandlePropTypeSpriteFrame(cocos2d::Node* pNode, cocos2d::Node* pParent, const char* pPropertyName, cocos2d::SpriteFrame* pSpriteFrame, cocosbuilder::CCBReader* ccbReader) {
    SpriteLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pSpriteFrame, ccbReader);
    pSpriteFrame->retain();
    static_cast<Button*>(pNode)->_impl->_normalSpriteFrame = pSpriteFrame;    
}
namespace_ee_end