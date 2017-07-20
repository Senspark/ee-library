//
//  EEDataHandler.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_HANDLER_HPP_
#define EE_LIBRARY_DATA_HANDLER_HPP_

#include "EEForward.hpp"
#include "EEMeta.hpp"
#include "EEDataTraits.hpp"
#include "EEDataInfo.hpp"

namespace ee {
namespace detail {
/// Globally sets a data value.
/// @param[in] dataId The data unique ID.
/// @param[in] key The data key.
/// @param[in] value The data value.
void set0(std::size_t dataId, const std::string& key, const std::string& value);

/// Globally gets a data value.
/// @param[in] dataId The data unique ID.
/// @param]in] key The data key.
/// @param[out] result The data value.
/// @return True if result is assigned, false otherwise.
bool get0(std::size_t dataId, const std::string& key, std::string& result);

/// Globally removes a data value.
/// @param[in] dataId The data unique ID.
/// @param[in] key The data key.
void remove0(std::size_t dataId, const std::string& key);
} // namespace detail

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType, class Value,
          class... Keys, EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::is_settable_v<Traits, Value>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>)>
void set(Value&& value, Keys&&... keys) {
    detail::set0(DataType::Id,
                 Formatter::createKey(std::forward<Keys>(keys)...),
                 Traits::set(std::forward<Value>(value)));
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType,
          class Value = typename DataType::ValueType, class... Keys,
          EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::is_gettable_v<Traits, Value>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>)>
decltype(auto) get(Keys&&... keys) {
    std::string result;
    detail::get0(DataType::Id,
                 Formatter::createKey(std::forward<Keys>(keys)...), result);
    return Traits::get(result);
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType,
          class Value = typename DataType::ValueType, class... Keys,
          EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>),
          EE_REQUIRES(detail::is_traits_v<Traits, Value>)>
void getAndSet(const typename DataType::SetterType& setter, Keys&&... keys) {
    auto current = get<DataType, Traits, Formatter>(keys...);
    setter(current);
    set<DataType, Traits, Formatter>(current, std::forward<Keys>(keys)...);
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType, class Function,
          class... Keys, class = std::enable_if_t<std::is_same<
                             bool, typename DataType::ValueType>::value>>
void getAndSetIf(bool conditionalValue, Function&& setter, Keys&&... keys) {
    auto current = get<DataType, Traits>(keys...);
    if (current == conditionalValue) {
        current = not conditionalValue;
        setter();
        set<DataType, Traits>(current, std::forward<Keys>(keys)...);
    }
}

template <class DataType, class... Keys> void remove(Keys&&... keys) {
    detail::remove0(DataType::Id,
                    DataType::createKey(std::forward<Keys>(keys)...));
}

class DataHandler final {
private:
    using Self = DataHandler;

public:
    using SetCallback = std::function<void(
        std::size_t dataId, const std::string& key, const std::string& value)>;

    using GetCallback = std::function<bool(
        std::size_t dataId, const std::string& key, std::string& result)>;

    using RemoveCallback =
        std::function<void(std::size_t dataId, const std::string& key)>;

    static const int LowestPriority;

    DataHandler(int priority = LowestPriority);

    ~DataHandler();

    DataHandler(const DataHandler& other) = delete;
    DataHandler& operator=(const DataHandler& other) = delete;

    DataHandler(DataHandler&& other) = delete;
    DataHandler& operator=(DataHandler&& other) = delete;

    /// Sets the dispatch priority for this handler.
    /// Lower-value priority handlers will dispatch first.
    /// @param[in] priority The desired priority.
    /// @return Instance to this.
    Self& setPriority(int priority);

    template <class DataType, class Value, class... Keys>
    [[deprecated]] void set(Value&& value, Keys&&... keys) {
        using ee::set;
        set<DataType>(std::forward<Value>(value), std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] auto get(Keys&&... keys) {
        using ee::get;
        return get<DataType>(std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] void getAndSet(const typename DataType::SetterType& f,
                                  Keys&&... keys) {
        using ee::getAndSet;
        return getAndSet<DataType>(f, std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] void remove(Keys&&... keys) {
        using ee::remove;
        return remove<DataType>(std::forward<Keys>(keys)...);
    }

    /// Sets the SET callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const SetCallback& callback);

    /// Sets the GET callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const GetCallback& callback);

    /// Sets the REMOVE callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const RemoveCallback& callback);

private:
    friend void detail::set0(std::size_t dataId, const std::string& key,
                             const std::string& value);
    friend bool detail::get0(std::size_t dataId, const std::string& key,
                             std::string& result);
    friend void detail::remove0(std::size_t dataId, const std::string& key);

    void insertHandler();
    void eraseHandler();

    bool inserted_;
    int priority_;

    SetCallback setCallback_;
    GetCallback getCallback_;
    RemoveCallback removeCallback_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
