//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP_
#define EE_LIBRARY_DATA_INFO_HPP_

#include <functional>
#include <tuple>

#include "EEUtils.hpp"

namespace ee {
namespace detail {
class DataInfoBase {};
} // namespace detail

template <std::size_t DataId, class Value, class... Keys>
class DataInfo final : public detail::DataInfoBase {
public:
    static_assert(not std::is_reference<Value>::value,
                  "Value type must not be a reference type!");

    static_assert(std::is_same<std::decay_t<Value>, Value>::value,
                  "Value type must be a decayed type!");

    using ValueType = Value;
    using KeyTypes = std::tuple<Keys...>;

    using SetterType = std::function<void(ValueType& value)>;

    enum { Id = DataId };

    static std::string createKey(const Keys&... keys) {
        return toString(toString("___", keys)...);
    }
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_INFO_HPP_ */
