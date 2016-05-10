//
//  CCNodeV3Loader.hpp
//  ee-library
//
//  Created by Zinge on 5/10/16.
//
//

#ifndef CCNodeV3Loader_hpp
#define CCNodeV3Loader_hpp

#include "EEMacro.hpp"

#include <cocosbuilder/CCNodeLoader.h>

NS_EE_BEGIN
class NodeV3Loader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NodeV3Loader, loader);
    
protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeColor3(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        cocos2d::Color3B color3B,
                                        cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeCheck(cocos2d::Node* node,
                                       cocos2d::Node* parent,
                                       const char* propertyName,
                                       bool check,
                                       cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeByte(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      unsigned char byte,
                                      cocosbuilder::CCBReader* reader) override;
};
NS_EE_END

#endif /* CCNodeV3Loader_hpp */
