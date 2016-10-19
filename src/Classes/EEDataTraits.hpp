//
//  EEDataTraits.hpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#ifndef EE_DATA_TRAITS_HPP_
#define EE_DATA_TRAITS_HPP_

#include <string>
#include <type_traits>

#include "EEForward.hpp"

namespace ee {
template <class T>
struct DataTraits<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
    static std::string set(T value) { return std::to_string(value); };

    static T get(const std::string& value);
};

template <> struct DataTraits<std::string> {
    static const std::string& set(const std::string& value);
    static const std::string& get(const std::string& value);
};

template <class T>
struct DataTraits<T, std::enable_if_t<std::is_enum<T>::value>> {
private:
    using U = std::underlying_type_t<T>;

public:
    static std::string set(T value) {
        return DataTraits<U>::set(static_cast<U>(value));
    }

    static T get(const std::string& value) {
        return static_cast<T>(DataTraits<U>::get(value));
    }
};
} // namespace ee

#endif /* EE_DATA_TRAITS_HPP_ */
