//
//  EEMultilingualDelegate.cpp
//  ee-library
//
//  Created by Zinge on 7/31/17.
//
//

#include "EEMultilingualDelegate.hpp"
#include "EELanguageSwitcher.hpp"
#include "EELanguageFormatter.hpp"

#include <2d/CCNode.h>
#include <base/CCValue.h>

namespace ee {
void MultilingualDelegate::onNodeLoaded(cocos2d::Node* node,
                                        cocosbuilder::NodeLoader* nodeLoader) {
    CC_ASSERT(key_);
}

bool MultilingualDelegate::onAssignCCBMemberVariable(
    cocos2d::Ref* target, const char* memberVariableName, cocos2d::Node* node) {
    return false;
}

bool MultilingualDelegate::onAssignCCBCustomProperty(
    cocos2d::Ref* target, const char* memberVariableName,
    const cocos2d::Value& value) {
    if (dynamic_cast<MultilingualDelegate*>(target) == this) {
        std::string name = memberVariableName;
        if (name == "multilingual_key") {
            CC_ASSERT(value.getType() == cocos2d::Value::Type::STRING);
            auto key = value.asString();

            auto&& switcher = LanguageSwitcher::getInstance();
            auto formatter =
                switcher.getFormatter(switcher.getCurrentLanguage(), key);

            std::vector<std::string> emptyTokens(formatter.getPlaceholders(),
                                                 "");
            setFormat(emptyTokens);
            setKey(key);
        }
    }
    return false;
}
} // namespace ee
