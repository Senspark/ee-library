//
//  EELanguageSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageSwitcher.hpp"

#include <stack>

#include <base/CCRefPtr.h>
#include <base/CCValue.h>
#include <platform/CCFileUtils.h>

#include "EELanguage.hpp"
#include "EELanguageDelegate.hpp"
#include "EELanguageFormatter.hpp"

namespace ee {
using Self = LanguageSwitcher;

bool Self::LanguageComparator::operator()(const Language& lhs,
                                          const Language& rhs) const {
    return lhs.getCode() < rhs.getCode();
}

Self::LanguageSwitcher() {
    locked_ = false;
    currentLanguage_ = std::make_unique<Language>(Language::English);
}

Self::~LanguageSwitcher() {}

const Language& Self::getCurrentLanguage() const {
    return *currentLanguage_;
}

void Self::changeLanguage(const Language& language) {
    CC_ASSERT(not locked_);
    locked_ = true;
    currentLanguage_ = std::make_unique<Language>(language);
    for (auto&& delegate : delegates_) {
        delegate->setLanguage(language);
    }
    locked_ = false;
}

const LanguageFormatter& Self::getFormatter(const std::string& key) const {
    return getFormatter(getCurrentLanguage(), key);
}

const LanguageFormatter& Self::getFormatter(const Language& language,
                                            const std::string& key) const {
    try {
        auto&& result = dictionaries_.at(language).at(key);
        return result;
    } catch (const std::out_of_range& ex) {
        CC_ASSERT(false);
        static const LanguageFormatter nil("{null}");
        return nil;
    }
}

const std::string& Self::getFormat(const std::string& key) const {
    return getFormat(getCurrentLanguage(), key);
}

const std::string& Self::getFormat(const Language& language,
                                   const std::string& key) const {
    auto&& formatter = getFormatter(language, key);
    return formatter.getFormat();
}

std::string Self::getText(const std::string& key) const {
    return getText(getCurrentLanguage(), key);
}

std::string Self::getText(const std::string& key,
                          const std::vector<std::string>& args) const {
    return getText(getCurrentLanguage(), key, args);
}

std::string Self::getText(const Language& language,
                          const std::string& key) const {
    return getText(language, key, {});
}

std::string Self::getText(const Language& language, const std::string& key,
                          const std::vector<std::string>& args) const {
    auto&& formatter = getFormatter(language, key);
    auto result = formatter.format(args);
    return result;
}

void Self::loadLanguage(const Language& language,
                        const cocos2d::ValueMap& map) {
    for (auto&& elt : map) {
        auto&& key = elt.first;
        auto&& text = elt.second.asString();
        auto formatter = LanguageFormatter(text);
        dictionaries_[language].emplace(key, std::move(formatter));
    }
}

void Self::loadLanguage(const Language& language, const std::string& filename) {
    auto map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(filename);
    loadLanguage(language, map);
}

void Self::addDelegate(LanguageDelegate* delegate) {
    CC_ASSERT(not locked_);
    delegates_.insert(delegate);
}

void Self::removeDelegate(LanguageDelegate* delegate) {
    CC_ASSERT(not locked_);
    delegates_.erase(delegate);
}
} // namespace ee
