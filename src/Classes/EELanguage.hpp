//
//  EELanguage.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_HPP
#define EE_LIBRARY_LANGUAGE_HPP

#include <string>

namespace ee {
class Language {
public:
    const std::string& getCode() const;

private:
    explicit Language();
};
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_HPP */
