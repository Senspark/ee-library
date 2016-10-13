//
//  EEDataTraits.cpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#include <stdexcept>

#include "EEDataTraits.hpp"

namespace ee {
template <> bool DataTraits<bool>::get(const std::string& value) {
    if (value == "1") {
        return true;
    }
    if (value == "0") {
        return false;
    }
    throw std::invalid_argument("DataTraits<bool>");
}

template <> int DataTraits<int>::get(const std::string& value) {
    return std::stoi(value);
}

template <>
unsigned long DataTraits<unsigned long>::get(const std::string& value) {
    return std::stoul(value);
}

template <> long long DataTraits<long long>::get(const std::string& value) {
    return std::stoll(value);
}

template <>
unsigned long long
DataTraits<unsigned long long>::get(const std::string& value) {
    return std::stoull(value);
}

template <> float DataTraits<float>::get(const std::string& value) {
    return std::stof(value);
}

template <> double DataTraits<double>::get(const std::string& value) {
    return std::stod(value);
}

const std::string& DataTraits<std::string>::set(const std::string& value) {
    return value;
}

const std::string& DataTraits<std::string>::get(const std::string& value) {
    return value;
}
} // namespace ee
