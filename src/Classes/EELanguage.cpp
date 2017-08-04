//
//  EELanguage.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguage.hpp"

namespace ee {
const Language Language::English("en");
const Language Language::Thailand("th");
const Language Language::Vietnamese("vi");
const Language Language::Russian("ru");

Language Language::create(const std::string& code) {
    return Language(code);
}

Language::Language(const std::string& code)
    : code_(code) {
}

const std::string& Language::getCode() const {
    return code_;
}

bool Language::operator==(const Self& other) const {
    return code_ == other.code_;
}
} // namespace ee
