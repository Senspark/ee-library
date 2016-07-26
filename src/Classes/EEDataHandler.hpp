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
    using SavingCallback = std::function<void(
        int dataId, const std::string& key, const cocos2d::Value& value)>;

    using LoadingCallback = std::function<bool(
        int dataId, const std::string& key, cocos2d::Value& result)>;

    DataHandler();
    ~DataHandler();

    template <class DataType, class Value, class... Keys>
    void set(Value&& value, Keys&&... keys) {
        using ValueType = typename DataType::ValueType;
        save(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
             DataTraits<ValueType>::save(std::forward<Value>(value)));
    }

    template <class DataType, class... Keys>
    decltype(auto) get(Keys&&... keys) {
        using ValueType = typename DataType::ValueType;
        cocos2d::Value result{};
        load(DataType::Id, DataType::createKey(std::forward<Keys>(keys)...),
             result);
        return DataTraits<ValueType>::load(result);
    }

    template <class DataType, class Function, class... Keys>
    void getAndSet(Function&& f, Keys&&... keys) {
        auto current = get<DataType>(keys...);
        f(current);
        set<DataType>(current, std::forward<Keys>(keys)...);
    }

    void setSavingCallback(const SavingCallback& callback);
    void setLoadingCallback(const LoadingCallback& callback);

private:
    void save(int dataId, const std::string& key,
              const cocos2d::Value& value) const;

    bool load(int dataId, const std::string& key, cocos2d::Value& result) const;

    SavingCallback savingCallback_;
    LoadingCallback loadingCallback_;

    static std::unordered_set<DataHandler*> handlers_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
