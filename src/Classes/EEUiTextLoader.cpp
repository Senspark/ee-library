//
//  EEUiTextLoader.cpp
//  ee-library
//
//  Created by enrevol on 3/25/16.
//
//

#include "EEUiTextLoader.hpp"

#include "ui/UIText.h"

NS_EE_BEGIN
#define PROPERTY_COLOR "color"
#define PROPERTY_OPACITY "opacity"
#define PROPERTY_BLENDFUNC "blendFunc"
#define PROPERTY_FONTNAME "fontName"
#define PROPERTY_FONTSIZE "fontSize"
#define PROPERTY_HORIZONTALALIGNMENT "horizontalAlignment"
#define PROPERTY_VERTICALALIGNMENT "verticalAlignment"
#define PROPERTY_STRING "string"
#define PROPERTY_DIMENSIONS "dimensions"

cocos2d::Node* UiTextLoader::createNode(cocos2d::Node* parent, cocosbuilder::CCBReader* reader) {
    return cocos2d::ui::Text::create();
}

void UiTextLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                          cocos2d::Node* parent,
                                          const char* propertyName,
                                          cocos2d::Color3B color3B,
                                          cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_COLOR) == 0) {
        text->setColor(color3B);
    } else {
        NodeLoader::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
    }
}

void UiTextLoader::onHandlePropTypeByte(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        unsigned char byte,
                                        cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_OPACITY) == 0) {
        text->setOpacity(byte);
    } else {
        NodeLoader::onHandlePropTypeByte(node, parent, propertyName, byte, reader);
    }
}

void UiTextLoader::onHandlePropTypeBlendFunc(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             cocos2d::BlendFunc blendFunc,
                                             cocosbuilder::CCBReader* reader) {
    // auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_BLENDFUNC) == 0) {
        CCASSERT(false, "Unexpected value.");
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(node, parent, propertyName, blendFunc, reader);
    }
}

void UiTextLoader::onHandlePropTypeFontTTF(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           const char * fontTTF,
                                           cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_FONTNAME) == 0) {
        text->setFontName(fontTTF);
    } else {
        NodeLoader::onHandlePropTypeFontTTF(node, parent, propertyName, fontTTF, reader);
    }
}

void UiTextLoader::onHandlePropTypeText(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        const char* text,
                                        cocosbuilder::CCBReader* reader) {
    auto uiText = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_STRING) == 0) {
        uiText->setString(text);
    } else {
        NodeLoader::onHandlePropTypeText(node, parent, propertyName, text, reader);
    }
}

void UiTextLoader::onHandlePropTypeFloatScale(cocos2d::Node* node,
                                              cocos2d::Node* parent,
                                              const char* propertyName,
                                              float floatScale,
                                              cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_FONTSIZE) == 0) {
        text->setFontSize(floatScale);
    } else {
        NodeLoader::onHandlePropTypeFloatScale(node, parent, propertyName, floatScale, reader);
    }
}

void UiTextLoader::onHandlePropTypeIntegerLabeled(cocos2d::Node* node,
                                                  cocos2d::Node* parent,
                                                  const char* propertyName,
                                                  int integerLabeled,
                                                  cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
        text->setTextHorizontalAlignment(static_cast<cocos2d::TextHAlignment>(integerLabeled));
    } else if(strcmp(propertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
        text->setTextVerticalAlignment(static_cast<cocos2d::TextVAlignment>(integerLabeled));
    } else {
        NodeLoader::onHandlePropTypeFloatScale(node, parent, propertyName, integerLabeled, reader);
    }
}

void UiTextLoader::onHandlePropTypeSize(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        cocos2d::Size size,
                                        cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (std::strcmp(propertyName, PROPERTY_DIMENSIONS) == 0) {
        text->setTextAreaSize(size);
    } else {
        NodeLoader::onHandlePropTypeSize(node, parent, propertyName, size, reader);
    }
}
NS_EE_END