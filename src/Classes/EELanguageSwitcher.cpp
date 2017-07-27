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

#include <base/CCRefPtr.h>
#include <base/CCValue.h>
#include <platform/CCFileUtils.h>

namespace ee {
bool LanguageSwitcher::LanguageComparator::
operator()(const Language& lhs, const Language& rhs) const {
    return lhs.getCode() < rhs.getCode();
}

const std::string LanguageSwitcher::NullKey = "__this_is_a_null_key";

LanguageSwitcher& LanguageSwitcher::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

LanguageSwitcher::LanguageSwitcher() {
    locked_ = false;
    currentLanguage_ = std::make_unique<Language>(Language::English);
}

LanguageSwitcher::~LanguageSwitcher() {
}

Language LanguageSwitcher::getCurrentLanguage() const {
    return *currentLanguage_;
}

void LanguageSwitcher::changeLanguage(const Language& language) {
    CC_ASSERT(not locked_);
    locked_ = true;
    currentLanguage_ = std::make_unique<Language>(language);
    for (auto iter = delegates_.cbegin(); iter != delegates_.cend();) {
        auto&& delegate = *iter;
        CC_ASSERT(not delegate.expired());
        if (delegate.expired()) {
            iter = delegates_.erase(iter);
        } else {
            auto ptr = delegate.lock();
            ptr->setLanguage(language);
            ++iter;
        }
    }
    for (auto&& delegate : delegates_) {
        CC_ASSERT(not delegate.expired());
    }
    locked_ = false;
}

std::string LanguageSwitcher::getFormat(const Language& language,
                                        const std::string& key) const {
    std::string result = "{null}";
    try {
        result = dictionaries_.at(language).at(key);
    } catch (const std::out_of_range& ex) {
        CC_ASSERT(false);
    }
    return result;
}

std::string LanguageSwitcher::getText(const Language& language,
                                      const std::string& key) const {
    return getText(language, {});
}

std::string
LanguageSwitcher::getText(const Language& language, const std::string& key,
                          const std::vector<std::string>& args) const {
    auto format = getText(language, key);

    std::size_t token_index = 0;
    std::size_t char_index = 0;
    std::string result;

    while (char_index < format.size()) {
        if (format[char_index] == '%') {
            if (char_index + 1 < format.size() &&
                format[char_index + 1] == '%') {
                // Two consecutive %.
                result.push_back('%');
                char_index += 2;
            } else {
                if (token_index < args.size()) {
                    // Replace % with a token.
                    result += args[token_index];
                    ++char_index;
                    ++token_index;
                } else {
                    CC_ASSERT(false);
                }
            }
        } else {
            result.push_back(format[char_index]);
            ++char_index;
        }
    }

    return result;
}

void LanguageSwitcher::loadLanguage(const Language& language,
                                    const cocos2d::ValueMap& map) {
    for (auto&& elt : map) {
        auto&& key = elt.first;
        auto&& text = elt.second.asString();
        dictionaries_[language][key] = text;
    }
}

void LanguageSwitcher::loadLanguage(const Language& language,
                                    const std::string& filename) {
    auto map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(filename);
    loadLanguage(language, map);
}

void LanguageSwitcher::addDelegate(
    const std::shared_ptr<LanguageDelegate>& delegate) {
    CC_ASSERT(not locked_);
    delegates_.insert(delegate);
}

void LanguageSwitcher::removeDelegate(
    const std::shared_ptr<LanguageDelegate>& delegate) {
    CC_ASSERT(not locked_);
    delegates_.erase(delegate);
}
} // namespace ee
