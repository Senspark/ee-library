//
//  EEUiTextLoader.hpp
//  ee-library
//
//  Created by enrevol on 3/25/16.
//
//

#ifndef EEUiTextLoader_hpp
#define EEUiTextLoader_hpp

#include "EEMacro.hpp"

#include <cocosbuilder/CCNodeLoader.h>

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::Text.
class UiTextLoader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiTextLoader, loader);
    
protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeColor3(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        cocos2d::Color3B color3B,
                                        cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeByte(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      unsigned char byte,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeBlendFunc(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocos2d::BlendFunc blendFunc,
                                           cocosbuilder::CCBReader* ccbReader) override;
    
    virtual void onHandlePropTypeFontTTF(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName,
                                         const char * fontTTF,
                                         cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeText(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      const char* text,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeFloatScale(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            float floatScale,
                                            cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeIntegerLabeled(cocos2d::Node* node,
                                                cocos2d::Node* parent,
                                                const char* propertyName,
                                                int integerLabeled,
                                                cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeSize(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      cocos2d::Size size,
                                      cocosbuilder::CCBReader* reader) override;
};

NS_EE_END

#endif /* EEUiTextLoader_hpp */