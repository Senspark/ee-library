//
//  EEUiButtonLoader.hpp
//  ee-library
//
//  Created by enrevol on 3/22/16.
//
//

#ifndef EEUiButtonLoader_hpp
#define EEUiButtonLoader_hpp

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include "cocosbuilder/CCNodeLoader.h"
#include "ui/UIWidget.h"

NS_EE_BEGIN
class UiButtonLoader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiButtonLoader, loader);
    
protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent, cocosbuilder::CCBReader* reader) override;
    
    virtual cocos2d::SpriteFrame* parsePropTypeSpriteFrame(cocos2d::Node* node,
                                                           cocos2d::Node* parent,
                                                           cocosbuilder::CCBReader* reader,
                                                           const char* propertyName) override;

    virtual void onHandlePropTypeBlockControl(cocos2d::Node* node,
                                              cocos2d::Node* parent,
                                              const char* propertyName,
                                              cocosbuilder::BlockControlData* blockControlData,
                                              cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeCheck(cocos2d::Node* node,
                                       cocos2d::Node* parent,
                                       const char* propertyName,
                                       bool check,
                                       cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeString(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        const char* string,
                                        cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeFontTTF(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName,
                                         const char* fontTTF,
                                         cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeFloatScale(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            float floatScale,
                                            cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypePoint(cocos2d::Node* node,
                                       cocos2d::Node* parent,
                                       const char* propertyName,
                                       cocos2d::Vec2 point,
                                       cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeSize(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      cocos2d::Size size,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeSpriteFrame(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             cocos2d::SpriteFrame* spriteFrame,
                                             cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeColor3(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        cocos2d::Color3B color3B,
                                        cocosbuilder::CCBReader* reader) override;
    
    std::string _spriteFrameName;
    cocos2d::ui::Widget::TextureResType _textureResType;
};
NS_EE_END

#endif /* EEUiButtonLoader_hpp */
