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

#include <base/CCValue.h>

namespace ee {
class DataHandler {
public:
    using NotifyCallback = std::function<void(
        int dataId, const std::string& key, const cocos2d::Value& value)>;

    using RequestCallback = std::function<bool(
        int dataId, const std::string& key, cocos2d::Value& result)>;

    DataHandler();
    ~DataHandler();

    template <class DataType, class Value, class... Keys>
    void set(Value&& value, Keys&&... keys) {
        notify(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
               cocos2d::Value{std::forward<Value>(value)});
    }

    template <class DataType, class Value = typename DataType::ValueType,
              class... Keys>
    Value get(Keys&&... keys) {
        return request<Value>(DataType::Id,
                              DataType::createKey(std::forward<Keys>(keys)...));
    }

    template <class DataType, class Function, class... Keys>
    void getAndSet(Function&& f, Keys&&... keys) {
        auto current = get<DataType>(keys...);
        f(current);
        set<DataType>(current, std::forward<Keys>(keys)...);
    }

    void setNotifyCallback(const NotifyCallback& callback);
    void setRequestCallback(const RequestCallback& callback);

private:
    void notify(int dataId, const std::string& key,
                const cocos2d::Value& value) const;

    template <class T> T request(int dataId, const std::string& key) const;

    bool request(int dataId, const std::string& key,
                 cocos2d::Value& result) const;

    NotifyCallback notifyCallback_;
    RequestCallback requestCallback_;

    static std::unordered_set<DataHandler*> handlers_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
