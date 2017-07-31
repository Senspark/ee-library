//
//  EELanguageSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageSwitcher.hpp"
#include "EELanguage.hpp"
#include "EELanguageDelegate.hpp"
#include "EELanguageFormatter.hpp"

#include <base/CCRefPtr.h>
#include <base/CCValue.h>
#include <platform/CCFileUtils.h>

namespace ee {
namespace {
std::stack<LanguageSwitcher*> instances;
static LanguageSwitcher nilInstance;
} // namespace

bool LanguageSwitcher::LanguageComparator::
operator()(const Language& lhs, const Language& rhs) const {
    return lhs.getCode() < rhs.getCode();
}

LanguageSwitcher& LanguageSwitcher::getInstance() {
    return *instances.top();
}

LanguageSwitcher::LanguageSwitcher() {
    instances.push(this);
    locked_ = false;
    currentLanguage_ = std::make_unique<Language>(Language::English);
}

LanguageSwitcher::~LanguageSwitcher() {
    instances.pop();
}

const Language& LanguageSwitcher::getCurrentLanguage() const {
    return *currentLanguage_;
}

void LanguageSwitcher::changeLanguage(const Language& language) {
    CC_ASSERT(not locked_);
    locked_ = true;
    currentLanguage_ = std::make_unique<Language>(language);
    for (auto&& delegate : delegates_) {
        delegate->setLanguage(language);
    }
    locked_ = false;
}

const LanguageFormatter&
LanguageSwitcher::getFormatter(const Language& language,
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

const std::string& LanguageSwitcher::getFormat(const Language& language,
                                               const std::string& key) const {
    auto&& formatter = getFormatter(language, key);
    return formatter.getFormat();
}

std::string LanguageSwitcher::getText(const Language& language,
                                      const std::string& key) const {
    return getText(language, {});
}

std::string
LanguageSwitcher::getText(const Language& language, const std::string& key,
                          const std::vector<std::string>& args) const {
    auto&& formatter = getFormatter(language, key);
    auto result = formatter.format(args);
    return result;
}

void LanguageSwitcher::loadLanguage(const Language& language,
                                    const cocos2d::ValueMap& map) {
    for (auto&& elt : map) {
        auto&& key = elt.first;
        auto&& text = elt.second.asString();
        auto formatter = LanguageFormatter(text);
        dictionaries_[language].emplace(key, std::move(formatter));
    }
}

void LanguageSwitcher::loadLanguage(const Language& language,
                                    const std::string& filename) {
    auto map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(filename);
    loadLanguage(language, map);
}

void LanguageSwitcher::addDelegate(LanguageDelegate* delegate) {
    CC_ASSERT(not locked_);
    delegates_.insert(delegate);
}

void LanguageSwitcher::removeDelegate(LanguageDelegate* delegate) {
    CC_ASSERT(not locked_);
    delegates_.erase(delegate);
}
} // namespace ee
