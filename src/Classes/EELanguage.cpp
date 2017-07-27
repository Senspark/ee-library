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

Language::Language(const std::string& code)
    : code_(code) {
}

const std::string& Language::getCode() const {
    return code_;
}
} // namespace ee
