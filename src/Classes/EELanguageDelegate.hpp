//
//  EELanguageDelegate.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_DELEGATE_HPP
#define EE_LIBRARY_LANGUAGE_DELEGATE_HPP

#include <functional>
#include <string>
#include <vector>

namespace ee {
class Language;

class LanguageDelegate : public std::enable_shared_from_this<LanguageDelegate> {
private:
    using Self = LanguageDelegate;

public:
    using TextCallback = std::function<void(const std::string& text)>;

    /// Creates a language delegate.
    static std::shared_ptr<Self> create();

    /// Sets the text using the specified key and the current language.
    /// @param[in] key The multilingual key.
    /// @return Instance to this for chaining.
    Self* setText(const std::string& key);

    /// Sets the text using the specified key, format arguments and the current
    /// language.
    /// @param[in] key The multilingual key.
    /// @param[in] args The format arguments.
    /// @return Instance to this for chaining.
    Self* setText(const std::string& key, const std::vector<std::string>& args);

    /// Sets the multilingual key.
    /// @param[in] key The multilingual key.
    /// @return Instance to this for chaining.
    Self* setKey(const std::string& key);

    /// Sets the format arguments.
    /// @param[in] args The format arguments.
    /// @return Instance to this for chaining.
    Self* setFormat(const std::vector<std::string>& args);

    /// Sets the display language.
    /// @param[in] language The desired language.
    /// @return Instance to this for chaining.
    Self* setLanguage(const Language& language);

    /// Sets the text callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self* setTextCallback(const TextCallback& callback);

protected:
    class Deleter;

    LanguageDelegate();
    ~LanguageDelegate();

private:
    std::unique_ptr<Language> language_;
    std::unique_ptr<std::string> key_;
    std::vector<std::string> args_;

    TextCallback textCallback_;

    void updateText();
};
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_DELEGATE_HPP */
