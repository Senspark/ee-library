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

#include <cocosbuilder/CCNodeLoader.h>
#include <ui/UIWidget.h>

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::Button.
///
/// Simple example.
/// @code
/// // In onResolveCCBCCControlSelector method.
///
/// // Old way used for cocos2d::extension::ControlButton.
/// // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMyButtonPressed",
/// //                                     MyClass::onMyButtonPressed);
///
/// // New way used for cocos2d::ui::Button.
/// // This macro assumes that your_method_name_in_cocosbuilder
/// // and the function name have the same name.
/// // Do note that MyClass::onMyButtonPressed should have zero argument.
/// CCB_SELECTORRESOLVER_CCCONTROL_CLICK_EX_0(MyClass::onMyButtonPressed);
///
/// // If your_method_name_in_cocosbuilder
/// // have different name than the function name,
/// // use the following macro:
/// CCB_SELECTORRESOLVER_CCCONTROL_CLICK(this, "your_method_name_in_cocosbuilder",
///                                      CC_CALLBACK_0(MyClass::onMyButtonPressed,
///                                                    this));
/// @endcode
/// @see @c CCB_SELECTORRESOLVER_CCCONTROL_TOUCH
/// @see @c CCB_SELECTORRESOLVER_CCCONTROL_TOUCH_EX
/// @see @c CCB_SELECTORRESOLVER_CCCONTROL_CLICK_EX_1
/// @see @c cocos2d::ui::Widget::ccWidgetTouchCallback
/// @see @c cocos2d::ui::Widget::ccWidgetClickCallback
class UiButtonLoader : public cocosbuilder::NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiButtonLoader, loader);
    
protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;
    
    virtual cocos2d::SpriteFrame* parsePropTypeSpriteFrame(cocos2d::Node* node,
                                                           cocos2d::Node* parent,
                                                           cocosbuilder::CCBReader* reader,
                                                           const char* propertyName) override;
    
    virtual void onHandlePropTypeBlock(cocos2d::Node* node,
                                       cocos2d::Node* parent,
                                       const char* propertyName,
                                       cocosbuilder::BlockData* blockData,
                                       cocosbuilder::CCBReader* ccbReader) override;
    
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
    virtual void onHandlePropTypeFloat(cocos2d::Node* node,
                                       cocos2d::Node* parent,
                                       const char* propertyName,
                                       float floatValue,
                                       cocosbuilder::CCBReader* reader) override;
    
    virtual void onHandlePropTypeFloatScale(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            float floatScale,
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
    
    bool spriteFrameEnabled_;
    std::string spriteFrameName_;
    cocos2d::ui::Widget::TextureResType textureResType_;
};
NS_EE_END

#endif /* EEUiButtonLoader_hpp */
