//
//  LanguageFormatter.cpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#include "EELanguageFormatter.hpp"

#include <platform/CCPlatformMacros.h>

namespace ee {
LanguageFormatter::LanguageFormatter(const std::string& format) {
    setFormat(format);
}

void LanguageFormatter::setFormat(const std::string& format) {
    raw_ = format;
    tokens_.clear();

    placeholders_ = 0;
    std::size_t charIndex = 0;

    std::string word;
    while (charIndex < format.size()) {
        auto c = format[charIndex];
        if (c == '%') {
            if (charIndex + 1 < format.size() && format[charIndex + 1] == '%') {
                // Two consecutive %.
                word.push_back('%');
                charIndex += 2;
            } else {
                // Flush the current word.
                if (not word.empty()) {
                    tokens_.push_back(word);
                    word.clear();
                }
                // Placeholder.
                ++placeholders_;
                tokens_.emplace_back(std::string());
                ++charIndex;
            }
        } else {
            word.push_back(c);
            ++charIndex;
        }
    }
    // Flush the current word.
    if (not word.empty()) {
        tokens_.push_back(word);
        word.clear();
    }
}

std::string LanguageFormatter::format() const {
    return format({});
}

std::string
LanguageFormatter::format(const std::vector<std::string>& args) const {
    if (placeholders_ != args.size()) {
        CC_ASSERT(false);
        return "_invalid_argument";
    }

    auto size = raw_.size();
    for (auto&& arg : args) {
        size += arg.size();
    }
    size -= placeholders_;

    std::string result;
    result.reserve(size);

    std::size_t i = 0;
    for (auto&& token : tokens_) {
        if (token.empty()) {
            result.append(args.at(i++));
        } else {
            result.append(token);
        }
    }
    return result;
}
} // namespace ee
