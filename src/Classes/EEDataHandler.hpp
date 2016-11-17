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
#include "EEDataTraits.hpp"

namespace ee {
namespace detail {
void set0(std::size_t dataId, const std::string& key, const std::string& value);
bool get0(std::size_t dataId, const std::string& key, std::string& result);
void remove0(std::size_t dataId, const std::string& key);
} // namespace detail

template <class DataType,
          class Traits = DataTraits<typename DataType::ValueType>, class Value,
          class... Keys>
void set(Value&& value, Keys&&... keys) {
    detail::set0(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
                 Traits::set(std::forward<Value>(value)));
}

template <class DataType,
          class Traits = DataTraits<typename DataType::ValueType>,
          class... Keys>
auto get(Keys&&... keys) {
    std::string result;
    detail::get0(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
                 result);
    return Traits::get(result);
}

template <class DataType,
          class Traits = DataTraits<typename DataType::ValueType>,
          class... Keys>
void getAndSet(const typename DataType::SetterType& f, Keys&&... keys) {
    auto current = get<DataType, Traits>(keys...);
    f(current);
    set<DataType, Traits>(current, std::forward<Keys>(keys)...);
}

template <class DataType, class... Keys> void remove(Keys&&... keys) {
    detail::remove0(DataType::Id,
                    DataType::createKey(std::forward<Keys>(keys)...));
}

class DataHandler {
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

    DataHandler(const DataHandler& other);
    DataHandler& operator=(const DataHandler& other) = default;

    DataHandler(DataHandler&& other) = delete;
    DataHandler& operator=(DataHandler&& other) = delete;

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

    void setCallback(const SetCallback& callback);
    void setCallback(const GetCallback& callback);
    void setCallback(const RemoveCallback& callback);

private:
    friend void detail::set0(std::size_t dataId, const std::string& key,
                             const std::string& value);
    friend bool detail::get0(std::size_t dataId, const std::string& key,
                             std::string& result);
    friend void detail::remove0(std::size_t dataId, const std::string& key);

    int priority_;

    SetCallback setCallback_;
    GetCallback getCallback_;
    RemoveCallback removeCallback_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
