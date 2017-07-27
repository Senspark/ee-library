//
//  LanguageFormatter.hpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_FORMATTER_HPP
#define EE_LIBRARY_LANGUAGE_FORMATTER_HPP

#include <string>
#include <vector>

namespace ee {
class LanguageFormatter {
private:
    using Self = LanguageFormatter;

public:
    explicit LanguageFormatter(const std::string& format);

    void print();

    void print(const std::vector<std::string>& args);

private:
    std::string format_;
};
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_FORMATTER_HPP */
