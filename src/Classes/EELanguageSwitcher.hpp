//
//  EELanguageSwitcher.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_SWITCHER_HPP
#define EE_LIBRARY_LANGUAGE_SWITCHER_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#include <EECocos2dxFwd.hpp>

namespace ee {
class Language;
class LanguageDelegate;
class LanguageFormatter;

class LanguageSwitcher {
private:
    using Self = LanguageSwitcher;

public:
    LanguageSwitcher();
    virtual ~LanguageSwitcher();

    /// Gets the current language.
    virtual const Language& getCurrentLanguage() const;

    /// Changes the current language to the specified language.
    /// @param[in] language The desired language.
    virtual void changeLanguage(const Language& language);

    /// Gets the formatter for the current language and the specified key.
    /// @param[in] key The multilingual key.
    /// @return The formatter.
    const LanguageFormatter& getFormatter(const std::string& key) const;

    /// Gets the formatter for the specified language and key.
    /// @param[in] language The desired language.
    /// @param[in] key The multilingual key.
    /// @return The formatter.
    const LanguageFormatter& getFormatter(const Language& language,
                                          const std::string& key) const;

    /// Gets the format string for the current language and the specified key.
    /// @param[in] key The desired key.
    /// @return The format string.
    const std::string& getFormat(const std::string& key) const;

    /// Gets the format string for the specified language and key.
    /// @param[in] language The desired language.
    /// @param[in] key The desired key.
    /// @return The format string.
    const std::string& getFormat(const Language& language,
                                 const std::string& key) const;

    /// Gets the translated text in the current language for the specified key.
    /// @param[in] key The text's key.
    /// @return The translated text.
    std::string getText(const std::string& key) const;

    std::string getText(const std::string& key,
                        const std::vector<std::string>& args) const;

    /// Gets the translated text in the specified language for the specified
    /// key.
    /// @param[in] language The desired language.
    /// @param[in] key The text's key.
    /// @return The translated text.
    std::string getText(const Language& language, const std::string& key) const;

    std::string getText(const Language& language, const std::string& key,
                        const std::vector<std::string>& args) const;

    /// Loads the specified language from the specified map.
    /// @param[in] language The language to load.
    /// @param[in] map The map which contains the language dictionary.
    void loadLanguage(const Language& language, const cocos2d::ValueMap& map);

    /// Loads the specified language from the specified file.
    /// @param[in] language The language to load.
    /// @param[in] filename The filename.
    void loadLanguage(const Language& language, const std::string& filename);

private:
    friend LanguageDelegate;

    LanguageSwitcher(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void addDelegate(LanguageDelegate* delegate);
    void removeDelegate(LanguageDelegate* delegate);

    std::unique_ptr<Language> currentLanguage_;

    struct LanguageComparator {
        bool operator()(const Language& lhs, const Language& rhs) const;
    };

    std::map<Language, std::map<std::string, LanguageFormatter>,
             LanguageComparator>
        dictionaries_;

    bool locked_;
    std::set<LanguageDelegate*> delegates_;
};
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_SWITCHER_HPP */
