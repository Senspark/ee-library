//
//  EELanguageDelegate.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageDelegate.hpp"

#include "EELanguage.hpp"
#include "EELanguageFormatter.hpp"
#include "EELanguageSwitcher.hpp"

namespace ee {
using Self = LanguageDelegate;

Self::LanguageDelegate(LanguageSwitcher& switcher)
    : switcher_(switcher) {
    setLanguage(switcher_.getCurrentLanguage());
    switcher_.addDelegate(this);
}

Self::~LanguageDelegate() {
    switcher_.removeDelegate(this);
}

const Language& Self::getLanguage() const {
    return *language_;
}

Self* Self::setKey(const std::string& key) {
    if (key_ && *key_ != key) {
        args_.reset();
    }
    key_ = std::make_unique<std::string>(key);
    auto&& formatter = switcher_.getFormatter(key);
    if (formatter.getPlaceholders() == 0) {
        // Empty format.
        setFormat({});
    } else {
        updateText();
    }
    return this;
}

Self* Self::setFormat(const std::vector<std::string>& args) {
    args_ = std::make_unique<std::vector<std::string>>(args);
    updateText();
    return this;
}

Self* Self::setLanguage(const Language& language) {
    language_ = std::make_unique<Language>(language);
    updateText();
    return this;
}

Self* Self::setTextCallback(const TextCallback& callback) {
    textCallback_ = callback;
    updateText();
    return this;
}

void Self::updateText() {
    if (not textCallback_) {
        return;
    }
    if (not language_) {
        return;
    }
    if (not key_) {
        return;
    }
    if (not args_) {
        return;
    }
    auto text = switcher_.getText(*language_, *key_, *args_);
    textCallback_(text);
}
} // namespace ee
