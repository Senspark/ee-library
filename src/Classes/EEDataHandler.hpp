//
//  EEDataHandler.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_HANDLER_HPP_
#define EE_LIBRARY_DATA_HANDLER_HPP_

#include <unordered_set>

#include "EEForward.hpp"
#include "EEDataTraits.hpp"

#include <base/CCValue.h>

namespace ee {
class DataHandler {
public:
    using SetCallback = std::function<void(int dataId, const std::string& key,
                                           const cocos2d::Value& value)>;

    using GetCallback = std::function<bool(int dataId, const std::string& key,
                                           cocos2d::Value& result)>;

    using RemoveCallback =
        std::function<void(int dataId, const std::string& key)>;

    DataHandler();
    ~DataHandler();

    DataHandler(const DataHandler& other);
    DataHandler& operator=(const DataHandler& other) = default;

    DataHandler(DataHandler&& other) = delete;
    DataHandler& operator=(DataHandler&& other) = delete;

    template <class DataType, class Value, class... Keys>
    void set(Value&& value, Keys&&... keys) {
        using ValueType = typename DataType::ValueType;
        set0(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
             DataTraits<ValueType>::set(std::forward<Value>(value)));
    }

    template <class DataType, class... Keys>
    decltype(auto) get(Keys&&... keys) {
        using ValueType = typename DataType::ValueType;
        cocos2d::Value result{};
        get0(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
             result);
        return DataTraits<ValueType>::get(result);
    }

    template <class DataType, class... Keys>
    void getAndSet(const typename DataType::SetterType& f, Keys&&... keys) {
        auto current = get<DataType>(keys...);
        f(current);
        set<DataType>(current, std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys> void remove(Keys&&... keys) {
        remove0(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...));
    }

    void setCallback(const SetCallback& callback);
    void setCallback(const GetCallback& callback);
    void setCallback(const RemoveCallback& callback);

private:
    void set0(int dataId, const std::string& key,
              const cocos2d::Value& value) const;

    bool get0(int dataId, const std::string& key, cocos2d::Value& result) const;

    void remove0(int dataId, const std::string& key) const;

    SetCallback setCallback_;
    GetCallback getCallback_;
    RemoveCallback removeCallback_;

    static std::unordered_set<DataHandler*> handlers_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
