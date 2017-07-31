//
//  EELanguageDelegate.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageDelegate.hpp"
#include "EELanguage.hpp"
#include "EELanguageSwitcher.hpp"

namespace ee {
LanguageDelegate::LanguageDelegate() {
    setKey(LanguageSwitcher::NullKey);
    auto&& switcher = LanguageSwitcher::getInstance();
    setLanguage(switcher.getCurrentLanguage());
    switcher.addDelegate(this);
}

LanguageDelegate::~LanguageDelegate() {
    auto&& switcher = LanguageSwitcher::getInstance();
    switcher.removeDelegate(this);
}

LanguageDelegate* LanguageDelegate::setKey(const std::string& key) {
    key_ = std::make_unique<std::string>(key);
    updateText();
    return this;
}

LanguageDelegate*
LanguageDelegate::setFormat(const std::vector<std::string>& args) {
    args_ = args;
    updateText();
    return this;
}

LanguageDelegate* LanguageDelegate::setLanguage(const Language& language) {
    language_ = std::make_unique<Language>(language);
    updateText();
    return this;
}

LanguageDelegate*
LanguageDelegate::setTextCallback(const TextCallback& callback) {
    textCallback_ = callback;
    updateText();
    return this;
}

void LanguageDelegate::updateText() {
    if (not textCallback_) {
        return;
    }
    if (not language_) {
        return;
    }
    if (not key_) {
        return;
    }
    auto&& switcher = LanguageSwitcher::getInstance();
    auto text = switcher.getText(*language_, *key_, args_);
    textCallback_(text);
}
} // namespace ee
